// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseViewEditor.h"
#include "Data/MapData/Editor/MouseTab/MouseViewEditorRoundNodeGrid.h"
#include "Data/MapData/Editor/MouseTab/MouseViewSelectMousePanel.h"


#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/UniformGridPanel.h>
#include <Components/HorizontalBox.h>

#include <Kismet/KismetMathLibrary.h>

bool UGameMapUI_MouseViewEditor::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->MapBg = Cast<UImage>(this->GetWidgetFromName(FName(TEXT("CurMapBg"))));
	this->RoundNodeGridPanel = Cast<UUniformGridPanel>(this->GetWidgetFromName(FName(TEXT("GridPanel"))));

	this->Text_TotalRoundCount = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("TotalRoundCount"))));
	this->Text_CurRoundIndex = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("CurRoundIndex"))));
	this->Text_TotalRoundNodeCount = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("TotalRoundNodeCount"))));
	this->Text_CurRoundNodeIndex = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("CurRoundNodeIndex"))));

	this->LevelItemsGridPanel = Cast<UUniformGridPanel>(this->GetWidgetFromName(FName(TEXT("UniformGridPanel_94"))));
	this->CurShowLevelItemListPanel = Cast<UHorizontalBox>(this->GetWidgetFromName(FName(TEXT("HorizontalBox_164"))));

	return true;
}

void UGameMapUI_MouseViewEditor::InitMouseViewEditor(UFVMEditUI_GameMapEdit* Class, UGameMapUI_MouseTab* OwnerClass)
{
	this->FVMEditUI_GameMapEdit = Class;
	this->GameMapUI_MouseTab = OwnerClass;

	this->MouseViewSelectMousePanel = Cast<UMouseViewSelectMousePanel>(this->GetWidgetFromName(FName(TEXT("UMouseViewSelectMousePanel"))));
	this->MouseViewSelectMousePanel->GameMapUI_MouseViewEditor = this;
	//初始化格子节点
	this->InitRoundNodeGrid();
	//初始化老鼠选择项
	this->MouseViewSelectMousePanel->InitMouseViewSelectMousePanel();
	//初始化掉落物选项
	this->InitLevelItems();
}

UFVMEditUI_GameMapEdit* UGameMapUI_MouseViewEditor::GetEditor()
{
	return this->FVMEditUI_GameMapEdit;
}

UGameMapUI_MouseTab* UGameMapUI_MouseViewEditor::GetMouseTab()
{
	return this->GameMapUI_MouseTab;
}

void UGameMapUI_MouseViewEditor::InitRoundNodeGrid()
{
	//得到地图配置
	const FGameMapStruct& CurGameMapData = this->FVMEditUI_GameMapEdit->GetCurEditData().M_FGameMapStruct;

	//得到行，得到列
	for (int32 Row = 0; Row < CurGameMapData.M_Meshe.Num(); Row++)
	{
		for (int32 Col = 0; Col < 20; Col++)
		{
			UMouseViewEditorRoundNodeGrid* CurGrid = CreateWidget<UMouseViewEditorRoundNodeGrid>(this,
				LoadClass<UMouseViewEditorRoundNodeGrid>(
					nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MouseConfig/BP_MouseEditorGrid.BP_MouseEditorGrid_C'")
				));

			//初始化数据
			CurGrid->InitEditorRoundNodeGrid(Row, Col, this);

			this->RoundNodeGridPanel->AddChildToUniformGrid(CurGrid, Row, Col);
		}
	}

	//更新总波数
	this->TotalRound = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num();
	this->CurRoundIndex = 0;
	this->TotalRoundNode = 0;
	this->CurRoundNodeIndex = 0;

	//初始化老鼠视图
	if (this->TotalRound == 0)
	{
		this->DisEnableView();
	}
	else {
		//更新节点
		if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
		{
			this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();

			if (this->TotalRoundNode == 0)
			{
				this->DisEnableView();
			}
			else {
				//检查回合节点
				this->CheckTimeNodeCount();
				//更新【编辑器视图】
				this->UpdateView();
			}
		}
	}

	this->OnInit();
}

void UGameMapUI_MouseViewEditor::UpdateText()
{
	if (IsValid(this->Text_TotalRoundCount))
	{
		this->Text_TotalRoundCount->SetText(FText::FromString(FString::FromInt(this->TotalRound)));
	}

	if (IsValid(this->Text_CurRoundIndex))
	{
		this->Text_CurRoundIndex->SetText(FText::FromString(FString::FromInt(this->CurRoundIndex + 1)));
	}

	if (IsValid(this->Text_TotalRoundNodeCount))
	{
		this->Text_TotalRoundNodeCount->SetText(FText::FromString(FString::FromInt(this->TotalRoundNode)));
	}

	if (IsValid(this->Text_CurRoundNodeIndex))
	{
		this->Text_CurRoundNodeIndex->SetText(FText::FromString(FString::FromInt(this->CurRoundNodeIndex + 1)));
	}
}

void UGameMapUI_MouseViewEditor::CheckTimeNodeCount()
{
	if (this->TotalRound == 0 || this->TotalRoundNode == 0)
	{
		return;
	}

	//当前回合节点【10】在个中的其中一个
	FTimeNodeWidthRound& CurTimeNode = this->GameMapUI_MouseTab->GetConfigRef().
		CurRoundConfig[
			this->CurRoundIndex
		].CurNode[
			this->CurRoundNodeIndex
		];

	//当前回合节点中的最小子节点个数是否有20个
	if (CurTimeNode.CurNode.Num() < 20)
	{
		const FGameMapStruct& CurGameMapData = this->FVMEditUI_GameMapEdit->GetCurEditData().M_FGameMapStruct;

		//差多少补多少
		int32 Result = 20 - CurTimeNode.CurNode.Num();
		for (int32 i = 0; i < Result; i++)
		{
			FTimeNode CurLocalTimeNode;

			TArray<FMouseConfigNode> CurMouseNodes;
			CurMouseNodes.SetNum(CurGameMapData.M_Meshe.Num());

			CurLocalTimeNode.CurMouseNode = CurMouseNodes;

			CurTimeNode.CurNode.Emplace(CurLocalTimeNode);
		}

		//更新数据表视图
		this->GameMapUI_MouseTab->UpdateView();
	}
}

void UGameMapUI_MouseViewEditor::UpdateView()
{
	if (this->TotalRound == 0 || this->TotalRoundNode == 0)
	{
		this->DisEnableView();

		return;
	}
	else {
		this->EnableView();
	}

	for (auto ViewGrid : this->RoundNodeGridPanel->GetAllChildren())
	{
		Cast<UMouseViewEditorRoundNodeGrid>(ViewGrid)->UpdateView();
	}
}

void UGameMapUI_MouseViewEditor::EnableView()
{
	for (auto ViewGrid : this->RoundNodeGridPanel->GetAllChildren())
	{
		Cast<UMouseViewEditorRoundNodeGrid>(ViewGrid)->Enable();
	}
}

void UGameMapUI_MouseViewEditor::DisEnableView()
{
	for (auto ViewGrid : this->RoundNodeGridPanel->GetAllChildren())
	{
		Cast<UMouseViewEditorRoundNodeGrid>(ViewGrid)->DisEnable();
	}
}

void UGameMapUI_MouseViewEditor::Node_AddNewCurRound()
{
	bool Result = this->TotalRound == 0 ? true : false;

	FRondWidthMouseConfig NewConfig;
	this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Emplace(NewConfig);
	this->TotalRound = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num();
	this->GameMapUI_MouseTab->UpdateView();

	if (Result)
	{
		//开启视图
		this->EnableView();
		//更新【编辑器视图】
		this->UpdateView();
		this->UpdateLevelItems();
	}
}

void UGameMapUI_MouseViewEditor::Node_RemoveCurRound()
{

	//移除回合数据
	if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num()
		&&
		this->CurRoundIndex >= 0
		)
	{
		FRondWidthMouseConfig& Config = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[
			this->CurRoundIndex
		];
		for (FTimeNodeWidthRound& Round : Config.CurNode)
		{
			this->RemoveNodeData(Round.CurNode);
		}
	}

	//移除波
	this->Remove(this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig, this->CurRoundIndex);

	this->GameMapUI_MouseTab->UpdateView();

	this->TotalRound = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num();

	//如果当前回合索引小于总回合数
	if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		//更新最小子节点
		this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();
		this->CurRoundNodeIndex = 0;

		this->UpdateView();
		this->UpdateLevelItems();
	}
	else
	{
		//关闭老鼠视图
		this->TotalRoundNode = 0;
		this->CurRoundNodeIndex = 0;
		this->TotalRound = 0;
		this->CurRoundIndex = 0;
		this->DisEnableView();
	}
}

void UGameMapUI_MouseViewEditor::LastCurRound()
{
	//返回上一波
	this->Last(this->CurRoundIndex);

	//更新最小子节点
	if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();
		this->CurRoundNodeIndex = 0;
		//检查子节点
		this->CheckTimeNodeCount();
		//更新【编辑器视图】
		this->UpdateView();
		this->UpdateLevelItems();
	}

	this->OnInit();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::NextCurRound()
{
	this->InNext(this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num(), this->CurRoundIndex);

	//更新节点
	if (this->CurRoundIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		this->TotalRoundNode = this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();
		this->CurRoundNodeIndex = 0;
		//检查子节点
		this->CheckTimeNodeCount();
		//更新【编辑器视图】
		this->UpdateView();
		this->UpdateLevelItems();
	}

	this->OnInit();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::Node_AddNewRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	if (this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num() == 10)
	{
		return;
	}

	//得到地图配置
	const FGameMapStruct& CurGameMapData = this->FVMEditUI_GameMapEdit->GetCurEditData().M_FGameMapStruct;

	FTimeNodeWidthRound NewRoundNode;
	for (int32 Row = 0; Row < 20; Row++)
	{
		FTimeNode CurTimeNode;
		for (int32 Col = 0; Col < CurGameMapData.M_Meshe.Num(); Col++)
		{
			FMouseConfigNode CurNewMouseNode;
			CurNewMouseNode.CurMouseName = FString();
			CurTimeNode.CurMouseNode.Emplace(CurNewMouseNode);
		}
		NewRoundNode.CurNode.Emplace(CurTimeNode);
	}

	this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Emplace(NewRoundNode);

	this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();

	this->GameMapUI_MouseTab->UpdateView();

	if (this->CurRoundNodeIndex == 0)
	{
		//更新【编辑器视图】
		this->EnableView();
		this->UpdateView();
		this->UpdateLevelItems();
	}
}

void UGameMapUI_MouseViewEditor::Node_RemoveCurRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	//移除节点数据
	if (this->CurRoundNodeIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[
		this->CurRoundIndex
	].CurNode.Num()
			&&
			this->CurRoundNodeIndex >= 0
			)
	{
		//移除当前节点全部的外星人
		FTimeNodeWidthRound& Node = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[
			this->CurRoundIndex
		].CurNode[this->CurRoundNodeIndex];
		//移除节点数据	
		this->RemoveNodeData(Node.CurNode);
	}

	//移除回合节点
	this->Remove(
		this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode,
		this->CurRoundNodeIndex
	);

	this->GameMapUI_MouseTab->UpdateView();

	this->TotalRoundNode = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num();

	//更新【编辑器视图】
	if (this->TotalRoundNode == 0)
	{
		this->DisEnableView();
	}
	else {
		this->EnableView();
		this->UpdateView();
		this->UpdateLevelItems();
	}
}

void UGameMapUI_MouseViewEditor::LastCurRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	this->Last(this->CurRoundNodeIndex);

	//检查最小子节点
	this->CheckTimeNodeCount();
	//更新【编辑器视图】
	this->UpdateView();
	this->UpdateLevelItems();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::NextCurRoundNode()
{
	if (this->CurRoundIndex >= this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig.Num())
	{
		return;
	}

	this->InNext(this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex].CurNode.Num(), this->CurRoundNodeIndex);

	//检查最小子节点
	this->CheckTimeNodeCount();
	//更新【编辑器视图】
	this->UpdateView();
	this->UpdateLevelItems();

	this->OnSelectMouseNodeIsNull();
}

void UGameMapUI_MouseViewEditor::SetAllMouseSpawnFinishAutoNext(bool Enable)
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		LRoundConfig.AllMouseSpawnFinishAutoNext = Enable;
		this->GameMapUI_MouseTab->UpdateView();
	}
}

bool UGameMapUI_MouseViewEditor::GetAllMouseSpawnFinishAutoNext()
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		return LRoundConfig.AllMouseSpawnFinishAutoNext;
	}

	return false;
}

void UGameMapUI_MouseViewEditor::SetInNextRoundTime(float InNdexTime)
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		LRoundConfig.InNextRoundTime = InNdexTime;
		this->GameMapUI_MouseTab->UpdateView();
	}
}

float UGameMapUI_MouseViewEditor::GetInNextRoundTime()
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		return LRoundConfig.InNextRoundTime;
	}

	return 0.f;
}

void UGameMapUI_MouseViewEditor::SetCurRoundKey(FRoundKey Key)
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		LRoundConfig.RoundKey = Key;
		this->GameMapUI_MouseTab->UpdateView();
	}
}

FRoundKey UGameMapUI_MouseViewEditor::GetCurRoundKey()
{
	if (this->TotalRound != 0)
	{
		FRondWidthMouseConfig& LRoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[this->CurRoundIndex];
		return LRoundConfig.RoundKey;
	}

	return FRoundKey();
}

void UGameMapUI_MouseViewEditor::SetbEnableMouseNodeRandom(bool Value)
{
	this->bEnableMouseNodeRandom = Value;
}

void UGameMapUI_MouseViewEditor::SelectRoundNodeWithMouseNode(UMouseViewEditorRoundNodeGrid* CurMouseViewEditorRoundNodeGrid)
{
	this->MouseViewEditorRoundNodeGrid = CurMouseViewEditorRoundNodeGrid;
	this->OnSelectMouseNode(this->MouseViewEditorRoundNodeGrid->MouseConfigNode);
}

void UGameMapUI_MouseViewEditor::UpdateCurRoundNodeWidthMouseNode(FMouseConfigNode NewNode, bool bRemove)
{
	if (IsValid(this->MouseViewEditorRoundNodeGrid))
	{

		//写入数据
		{
			//获取老鼠配置
			FMouseConfig& CurMouseConfig = this->GetGameMapUIMouseTab()->GetConfigRef();

			//覆写当前节点的老鼠
			FMouseConfigNode& ConfigNode = CurMouseConfig.CurRoundConfig[
				this->GetCurRoundIndex() //回合
			].CurNode[
				this->GetCurRoundNodeIndex()//回合节点
			].CurNode[
				this->MouseViewEditorRoundNodeGrid->ColIndex //通过列得到时间节点【x轴】
			].CurMouseNode[
				this->MouseViewEditorRoundNodeGrid->RowIndex//通过行得到老鼠节点【Y轴】
			];

				if (bRemove)
				{
					//判断之前是否有数据
					if (!ConfigNode.CurMouseName.Equals(TEXT("")))
					{
						//添加老鼠数量
						int32 ID = FCString::Atoi(*ConfigNode.CurMouseName);
						int32* Count = CurMouseConfig.UseMouseKeyListCountMap.Find(ID);
						if (Count)
						{
							int32 TCount = (*Count - 1);
							if (TCount <= 0)
							{
								CurMouseConfig.ValidKeyID.Emplace(ID);
								CurMouseConfig.AllMouseListMap.Remove(*CurMouseConfig.AllMouseKeyListMap.Find(ID));
								CurMouseConfig.AllMouseKeyListMap.Remove(ID);
								CurMouseConfig.UseMouseKeyListCountMap.Remove(ID);

							}
							else {
								CurMouseConfig.UseMouseKeyListCountMap.Emplace(ID, TCount);
							}
						}
					}
				}

				if (this->bEnableMouseNodeRandom)
				{
					NewNode.CurMouseLine.Col = -1;
					NewNode.CurMouseLine.Row = -1;
				}

				ConfigNode = NewNode;
		}

		this->MouseViewEditorRoundNodeGrid->MouseConfigNode = NewNode;

		//覆盖数据
		this->GameMapUI_MouseTab->UpdateView();

		//显示内容
		this->MouseViewEditorRoundNodeGrid->UpdateView();
	}
}

void UGameMapUI_MouseViewEditor::EnableMouseNodeRemove()
{
	if (this->bEnableMouseNodeRemove)
	{
		this->bEnableMouseNodeRemove = false;

		this->OnMouseNodeRemoveClose();
	}
	else {
		this->bEnableMouseNodeRemove = true;

		this->OnMouseNodeRemoveOpen();
	}
}

void UGameMapUI_MouseViewEditor::RandomGeneratedAlien(int32 Col)
{
	if (Col == 0)
	{
		Col = 1;
	}

	if (Col > 20)
	{
		Col = 20;
	}

	TArray<int32> Cols;
	for (int32 i = 0; i < 20; i++)
	{
		Cols.Emplace(i);
	}

	int32 NewCount = this->LevelItemAlienList.Num();
	if (NewCount <= 0)
	{
		return;
	}

	TArray<FString> Names;
	this->LevelItemAlienList.GenerateKeyArray(Names);

	//移除当前节点全部
	//移除节点数据
	if (this->CurRoundNodeIndex < this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[
		this->CurRoundIndex
	].CurNode.Num()
			&&
			this->CurRoundNodeIndex >= 0
			)
	{
		//移除当前节点全部的外星人
		FTimeNodeWidthRound& Node = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[
			this->CurRoundIndex
		].CurNode[this->CurRoundNodeIndex];
		//移除节点数据	
		this->RemoveNodeData(Node.CurNode);
	}

	//重新生成外星人
	auto NewGen = [&]() ->int32 {
		if (Cols.Num() == 0)
		{
			return -1;
		}
		int32 CurRandom = UKismetMathLibrary::RandomIntegerInRange(0, Cols.Num() - 1);
		int32 Cur = Cols[CurRandom];
		Cols.RemoveAt(CurRandom);
		return Cur;
		};


	FMouseConfig& MouseRef = this->GetMouseTab()->GetConfigRef();
	for (int32 i = 0; i < Col; i++)
	{
		//生产随机最小时间节点位置
		int32 NewCol = NewGen();
		if (NewCol == -1)
		{
			return;
		}

		//生成随机外星人
		int32 CurRandom = UKismetMathLibrary::RandomIntegerInRange(0, NewCount - 1);
		for (int32 j = 0; j < MouseRef.CurRoundConfig[
			this->GetCurRoundIndex() //回合
		].CurNode[
			this->GetCurRoundNodeIndex()//回合节点
		].CurNode[
			0
		].CurMouseNode.Num(); j++)
		{
			//生成
			this->GenAlien(Names[CurRandom], j, NewCol);
		}
	}


	//更新【编辑器视图】
	if (this->TotalRoundNode == 0)
	{
		this->DisEnableView();
	}
	else {
		this->EnableView();
		this->UpdateView();
		this->UpdateLevelItems();
	}
}

void UGameMapUI_MouseViewEditor::GenAlien(const FString& Name, int32 Row, int32 Col)
{
	//获取全部老鼠列表
	FMouseConfig& MouseRef = this->GetMouseTab()->GetConfigRef();
	//赋予ID号
	int32* ID = MouseRef.AllMouseListMap.Find(Name);
	FString TargetName = FString();
	if (ID)
	{
		TargetName = FString::FromInt(*ID);
		MouseRef.AllMouseKeyListMap.Emplace(*ID, Name);
		//添加老鼠数量
		int32* Count = MouseRef.UseMouseKeyListCountMap.Find(*ID);
		if (Count)
		{
			MouseRef.UseMouseKeyListCountMap.Emplace(*ID, (*Count + 1));
		}
	}
	else {
		//获取可用ID
		int32 NewID = 0;
		if (MouseRef.ValidKeyID.Num())
		{
			NewID = MouseRef.ValidKeyID[0];
			MouseRef.ValidKeyID.RemoveAtSwap(0);
		}
		else {
			NewID = MouseRef.AllMouseListMap.Num();
		}
		MouseRef.AllMouseListMap.Emplace(Name, NewID);
		MouseRef.AllMouseKeyListMap.Emplace(NewID, Name);
		MouseRef.UseMouseKeyListCountMap.Emplace(NewID, 1);
		TargetName = FString::FromInt(NewID);
	}
	FMouseConfigNode Node;
	Node.CurMouseName = TargetName;

	{
		//获取老鼠配置
		FMouseConfig& CurMouseConfig = this->GetGameMapUIMouseTab()->GetConfigRef();

		//覆写当前节点的老鼠
		FMouseConfigNode& ConfigNode = CurMouseConfig.CurRoundConfig[
			this->GetCurRoundIndex() //回合
		].CurNode[
			this->GetCurRoundNodeIndex()//回合节点
		].CurNode[
			Col //通过列得到时间节点【x轴】
		].CurMouseNode[
			Row//通过行得到老鼠节点【Y轴】
		];

			ConfigNode = Node;
	}
}

//------------------------------------随机
void UGameMapUI_MouseViewEditor::InitLevelItems()
{
	UDataTable* Tabs = LoadObject<UDataTable>(
		this,
		TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'")
	);

	TArray<FMouse_Data*> Datas;
	Tabs->GetAllRows<FMouse_Data>(TEXT("MouseDatas"), Datas);



	//获取掉落物配置
	const FGameMapData& Data = this->FVMEditUI_GameMapEdit->GetCurEditData();

	//得到全部的

	int32 i = 0;
	for (const auto& CurItem : Datas)
	{
		if (CurItem)
		{

			UGameMapUI_MouseViewLevelItem* CurNewItem =
				CreateWidget<UGameMapUI_MouseViewLevelItem>(this,
					LoadClass<UGameMapUI_MouseViewLevelItem>(0,
						TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MouseConfig/BP_MouseViewLevelItem.BP_MouseViewLevelItem_C'")));

			CurNewItem->Init(this, CurItem->M_Mouse.M_MouseName, CurItem->M_Mouse.MouseHead);
			this->LevelItemsGridPanel->AddChildToUniformGrid(CurNewItem, i / 2, i % 2);
			i++;
		}
	}

	//更新显示
	this->UpdateLevelItems();
}

void UGameMapUI_MouseViewEditor::UpdateLevelItems()
{
	if (TotalRound == 0)
		return;

	//隐藏所有
	for (auto Widget : this->CurShowLevelItemListPanel->GetAllChildren())
	{
		Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	//FRondWidthMouseConfig& RoundConfig = this->GameMapUI_MouseTab->GetConfigRef().CurRoundConfig[
	//	this->CurRoundIndex
	//];
	//const FGameMapData& Data = this->FVMEditUI_GameMapEdit->GetCurEditData();

	//更新显示
	int32 i = 0;
	for (auto Name : this->LevelItemAlienList)
	{
		//查询
		if (i < this->CurShowLevelItemListPanel->GetAllChildren().Num())
		{
			this->CurShowLevelItemListPanel->GetChildAt(i)->SetVisibility(
				ESlateVisibility::SelfHitTestInvisible
			);

			Cast<UGameMapUI_MouseViewLevelItemView>(
				this->CurShowLevelItemListPanel->GetChildAt(i))->Init(Name.Key,
					Name.Value, this);
		}
		else {

			UGameMapUI_MouseViewLevelItemView* CurNewItem =
				CreateWidget<UGameMapUI_MouseViewLevelItemView>(this,
					LoadClass<UGameMapUI_MouseViewLevelItemView>(0,
						TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MouseConfig/BP_MouseViewLevelItemEditor.BP_MouseViewLevelItemEditor_C'")));

			CurNewItem->Init(Name.Key,
				Name.Value, this);

			this->CurShowLevelItemListPanel->AddChildToHorizontalBox(CurNewItem);
		}

		i++;
	}
}

void UGameMapUI_MouseViewEditor::RemoveNodeData(TArray<FTimeNode>& InputFTimeNodes)
{
	FMouseConfig& CurMouseConfig = this->GetGameMapUIMouseTab()->GetConfigRef();

	//遍历时间轴节点
	for (FTimeNode& Node : InputFTimeNodes)
	{
		//遍历外星人节点
		for (FMouseConfigNode& ConfigNode : Node.CurMouseNode)
		{
			//判断之前是否有数据【是否存放外星人】
			if (!ConfigNode.CurMouseName.Equals(TEXT("")))
			{
				//将名称转换为ID
				int32 ID = FCString::Atoi(*ConfigNode.CurMouseName);
				int32* Count = CurMouseConfig.UseMouseKeyListCountMap.Find(ID);
				if (Count)
				{
					int32 TCount = (*Count - 1);
					if (TCount <= 0)
					{
						CurMouseConfig.ValidKeyID.Emplace(ID);
						CurMouseConfig.AllMouseListMap.Remove(*CurMouseConfig.AllMouseKeyListMap.Find(ID));
						CurMouseConfig.AllMouseKeyListMap.Remove(ID);
						CurMouseConfig.UseMouseKeyListCountMap.Remove(ID);
					}
					else {
						CurMouseConfig.UseMouseKeyListCountMap.Emplace(ID, TCount);
					}
				}
			}

			ConfigNode.CurMouseName = TEXT("");
			ConfigNode.CurMouseLine.Row = -1;
			ConfigNode.CurMouseLine.Col = -1;
			ConfigNode.IgnoreRows.Empty();
			ConfigNode.LevelItems.Empty();
		}
	}
}

bool UGameMapUI_MouseViewLevelItem::Initialize()
{
	Super::Initialize();

	this->ItemNameText = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("TextBlock_48"))));
	this->Butt = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("Button_115"))));

	return true;
}

void UGameMapUI_MouseViewLevelItem::Init(UGameMapUI_MouseViewEditor* OWner, const FString& ItemName, FSoftObjectPath Img)
{
	this->GameMapUI_MouseViewEditor = OWner;
	this->ImgRes = Img;

	this->ItemNameText->SetText(FText::FromString(ItemName));

	FButtonStyle Style;
	Style.Normal.SetResourceObject(Img.TryLoad());
	Style.Normal.TintColor = FLinearColor(FVector(1.f, 1.f, 1.f));
	Style.Hovered.SetResourceObject(Img.TryLoad());
	Style.Hovered.TintColor = FLinearColor(FVector(7.f, 7.f, 7.f));
	Style.Pressed.SetResourceObject(Img.TryLoad());
	Style.Pressed.TintColor = FLinearColor(FVector(5.f, 5.f, 5.f));

	Style.Disabled.SetResourceObject(Img.TryLoad());
	this->Butt->SetStyle(Style);
}

void UGameMapUI_MouseViewLevelItem::AddLevelItem()
{
	if (this->GameMapUI_MouseViewEditor->TotalRound == 0)
	{
		return;
	}

	this->GameMapUI_MouseViewEditor->LevelItemAlienList.Emplace(
		this->ItemNameText->GetText().ToString(),
		this->ImgRes
	);

	/*FRondWidthMouseConfig& RoundConfig =
		this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->GetConfigRef().
		CurRoundConfig[this->GameMapUI_MouseViewEditor->CurRoundIndex];

	RoundConfig.LevelItems.Emplace();*/

	this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->UpdateView();
	this->GameMapUI_MouseViewEditor->UpdateLevelItems();
}

bool UGameMapUI_MouseViewLevelItemView::Initialize()
{
	Super::Initialize();

	this->Butt = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("Button_115"))));

	return true;
}

void UGameMapUI_MouseViewLevelItemView::Init(FString Name, FSoftObjectPath Img, UGameMapUI_MouseViewEditor* Class)
{
	this->GameMapUI_MouseViewEditor = Class;
	this->ItemName = Name;

	FButtonStyle Style;
	Style.Normal.SetResourceObject(Img.TryLoad());
	Style.Normal.TintColor = FLinearColor(FVector(1.f, 1.f, 1.f));
	Style.Hovered.SetResourceObject(Img.TryLoad());
	Style.Hovered.TintColor = FLinearColor(FVector(7.f, 7.f, 7.f));
	Style.Pressed.SetResourceObject(Img.TryLoad());
	Style.Pressed.TintColor = FLinearColor(FVector(5.f, 5.f, 5.f));

	Style.Disabled.SetResourceObject(Img.TryLoad());
	this->Butt->SetStyle(Style);

	this->Butt->OnClicked.Clear();
	this->Butt->OnClicked.AddDynamic(this, &UGameMapUI_MouseViewLevelItemView::Remove);
}

void UGameMapUI_MouseViewLevelItemView::Remove()
{
	this->GameMapUI_MouseViewEditor->LevelItemAlienList.Remove(this->ItemName);
	this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->UpdateView();
	this->GameMapUI_MouseViewEditor->UpdateLevelItems();
}
