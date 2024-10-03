// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MouseTab/MouseViewEditorRoundNodeGrid.h"
#include "Data/MapData/Editor/MouseTab/MouseViewSelectMousePanel.h"
#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseViewEditor.h"
#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseTab.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"

#include "Data/CardData/MouseDataStruct.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/Image.h>


bool UMouseViewEditorRoundNodeGrid::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	this->GenerateMouse = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("Create"))));
	this->ShowMouseImg = Cast<UImage>(this->GetWidgetFromName(FName(TEXT("ShowMouse"))));

	return true;
}

void UMouseViewEditorRoundNodeGrid::GenerateNewMouse()
{
	bool bIsGen = false;
	//当选择了一个老鼠项时
	if (IsValid(this->GameMapUI_MouseViewEditor->MouseViewSelectMousePanel->GetMouseViewSelectMouseItem()))
	{
		//得到老鼠配置节点
		FMouseConfigNode CurSelectNode = this->GameMapUI_MouseViewEditor->MouseViewSelectMousePanel->GetMouseNode();

		//得到当前最小子节点的行
		CurSelectNode.CurMouseLine.Row = RowIndex;
		CurSelectNode.CurMouseLine.Col = -1;

		//覆盖当前最小字节点数据
		this->MouseConfigNode = CurSelectNode;
		bIsGen = true;
	}

	//更新选择节点
	if (this->GameMapUI_MouseViewEditor->MouseViewEditorRoundNodeGrid != this)
	{
		this->GameMapUI_MouseViewEditor->SelectRoundNodeWithMouseNode(this);
	}

	if (bIsGen && !this->GameMapUI_MouseViewEditor->bEnableMouseNodeRemove)
	{
		//获取全部老鼠列表
		FMouseConfig& MouseRef = this->GameMapUI_MouseViewEditor->GetMouseTab()->GetConfigRef();
		//赋予ID号
		int32* ID = MouseRef.AllMouseListMap.Find(this->MouseConfigNode.CurMouseName);
		FString TargetName = FString();
		if (ID)
		{
			TargetName = FString::FromInt(*ID);
			MouseRef.AllMouseKeyListMap.Emplace(*ID, this->MouseConfigNode.CurMouseName);
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
			MouseRef.AllMouseListMap.Emplace(this->MouseConfigNode.CurMouseName, NewID);
			MouseRef.AllMouseKeyListMap.Emplace(NewID, this->MouseConfigNode.CurMouseName);
			MouseRef.UseMouseKeyListCountMap.Emplace(NewID, 1);
			TargetName = FString::FromInt(NewID);
		}
		this->MouseConfigNode.CurMouseName = TargetName;
		this->GameMapUI_MouseViewEditor->UpdateCurRoundNodeWidthMouseNode(this->MouseConfigNode);
	}else if (this->GameMapUI_MouseViewEditor->bEnableMouseNodeRemove)
	{
		FMouseConfigNode CurSelectNode;
		CurSelectNode.CurMouseName = TEXT("");
		CurSelectNode.CurMouseLine.Row = -1;
		CurSelectNode.CurMouseLine.Col = -1;
		CurSelectNode.IgnoreRows.Empty();
		CurSelectNode.LevelItems.Empty();

		this->MouseConfigNode = CurSelectNode;
		this->GameMapUI_MouseViewEditor->SelectRoundNodeWithMouseNode(this);
		this->GameMapUI_MouseViewEditor->UpdateCurRoundNodeWidthMouseNode(CurSelectNode);
	}
	else {
		this->GameMapUI_MouseViewEditor->UpdateCurRoundNodeWidthMouseNode(this->MouseConfigNode);
	}
}

void UMouseViewEditorRoundNodeGrid::InitEditorRoundNodeGrid(const int32& Row, const int32& Col, UGameMapUI_MouseViewEditor* GameMapUIMouseViewEditor)
{
	this->GameMapUI_MouseViewEditor = GameMapUIMouseViewEditor;
	this->RowIndex = Row;
	this->ColIndex = Col;
}

void UMouseViewEditorRoundNodeGrid::CheckMouseNode()
{
	FMouseConfig& CurMouseConfig = this->GameMapUI_MouseViewEditor->GetGameMapUIMouseTab()->GetConfigRef();

	//老鼠节点检查，如果老鼠节点长度不匹配地图的行，则要就行扩充
	{
		const FGameMapStruct& CurGameMapData = this->GameMapUI_MouseViewEditor->FVMEditUI_GameMapEdit->GetCurEditData().M_FGameMapStruct;
		//当前老鼠节点个数
		int32 CurMouseNodeCount = CurMouseConfig.CurRoundConfig[
			this->GameMapUI_MouseViewEditor->GetCurRoundIndex() //回合
		].CurNode[
			this->GameMapUI_MouseViewEditor->GetCurRoundNodeIndex()//回合节点
		].CurNode[
			this->ColIndex //通过列得到时间节点【x轴】
		].CurMouseNode.Num();

			if (CurMouseNodeCount < CurGameMapData.M_Meshe.Num())
			{
				TArray<FMouseConfigNode> NewCurMouseNode;
				NewCurMouseNode.SetNum(CurGameMapData.M_Meshe.Num() - CurMouseNodeCount);

				CurMouseConfig.CurRoundConfig[
					this->GameMapUI_MouseViewEditor->GetCurRoundIndex() //回合
				].CurNode[
					this->GameMapUI_MouseViewEditor->GetCurRoundNodeIndex()//回合节点
				].CurNode[
					this->ColIndex //通过列得到时间节点【x轴】
				].CurMouseNode.Append(NewCurMouseNode);

					this->GameMapUI_MouseViewEditor->GetGameMapUIMouseTab()->UpdateView();
			}
	}
}

void UMouseViewEditorRoundNodeGrid::UpdateView()
{
	//检查老鼠节点
	this->CheckMouseNode();

	//获取老鼠配置
	const FMouseConfig& CurMouseConfig = this->GameMapUI_MouseViewEditor->GetGameMapUIMouseTab()->GetConfigRef();

	//获得当前节点的老鼠
	const FMouseConfigNode& CurMouseNode = CurMouseConfig.CurRoundConfig[
		this->GameMapUI_MouseViewEditor->GetCurRoundIndex() //回合
	].CurNode[
		this->GameMapUI_MouseViewEditor->GetCurRoundNodeIndex()//回合节点
	].CurNode[
		this->ColIndex //通过列得到时间节点【x轴】
	].CurMouseNode[
		this->RowIndex//通过行得到老鼠节点【Y轴】
	];

		//更新当前节点数据
		this->MouseConfigNode = CurMouseNode;

		//名称判空
		if (CurMouseNode.CurMouseName.Equals(TEXT("")))
		{
			this->ShowMouseImg->SetVisibility(ESlateVisibility::Collapsed);

			return;
		}

		FMouseConfig& Configref = this->GameMapUI_MouseViewEditor->GetMouseTab()->GetConfigRef();

		FString* FindName = Configref.AllMouseKeyListMap.Find(FCString::Atoi(*CurMouseNode.CurMouseName));
		if (!FindName)
		{
			return;
		}

		//获取全部老鼠的数据表
		UDataTable* const CurAllMouseDataTable = this->GameMapUI_MouseViewEditor->GameMapUI_MouseTab->GetMouseData();
		//得到老鼠数据
		FMouse_Data* const CurMouseData = CurAllMouseDataTable->FindRow<FMouse_Data>(FName(FString(
			*FindName
		)), TEXT("AllMouseData"));
		if (CurMouseData)
		{
			//设置图像显示
			this->ShowMouseImg->SetBrushFromTexture(Cast<UTexture2D>(CurMouseData->M_Mouse.MouseHead.TryLoad()), true);
			this->ShowMouseImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			this->ShowMouseImg->SetVisibility(ESlateVisibility::Collapsed);
		}

}

void UMouseViewEditorRoundNodeGrid::Enable()
{
	this->GenerateMouse->SetIsEnabled(true);
	this->ShowMouseImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//更新选择节点
	if (this->GameMapUI_MouseViewEditor->MouseViewEditorRoundNodeGrid != this)
	{
		this->GameMapUI_MouseViewEditor->SelectRoundNodeWithMouseNode(this);
	}
}

void UMouseViewEditorRoundNodeGrid::DisEnable()
{
	this->GenerateMouse->SetIsEnabled(false);
	this->ShowMouseImg->SetVisibility(ESlateVisibility::Collapsed);

	if (this->GameMapUI_MouseViewEditor->MouseViewEditorRoundNodeGrid == this)
	{
		this->GameMapUI_MouseViewEditor->OnSelectMouseNodeIsNull();
	}
}
