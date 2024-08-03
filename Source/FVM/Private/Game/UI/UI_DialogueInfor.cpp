// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_DialogueInfor.h"
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/UniformGridPanel.h>

#include "Game/UI/UI_Dialogue.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

bool UUI_DialogueListItem::Initialize()
{
	Super::Initialize();

	this->Title = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_42"));
	this->Content = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock"));

	return true;
}

void UUI_DialogueListItem::Init(const FString& T1, const FString& T2, UUI_DialogueInfor* TUI)
{
	this->Title->SetText(FText::FromString(T1));
	this->Content->SetText(FText::FromString(T2));
	this->UI = TUI;
}

void UUI_DialogueListItem::Cancel()
{
	if (IsValid(this->UI))
	{
		UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue = false;

		UFVMGameInstance::GetPlayerStructManager_Static()->Save(__FUNCTION__ + FString(TEXT("取消间章剧情操作")));

		this->UI->Load();
	}
}

bool UUI_DialogueInfor::Initialize()
{
	Super::Initialize();

	this->UniformDia = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_35"));
	this->ScrollBoxDia = this->GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_39"));

	return true;
}

void UUI_DialogueInfor::Load()
{
	if (!IsValid(this->TimeLoad_Dia_Manager))
	{
		this->TimeLoad_Dia_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Dia_Manager_T"));
		this->TimeLoad_Dia_Manager->InitWidget_First(
			this->UniformDia,
			this->ScrollBoxDia,
			0.001f);
		this->TimeLoad_Dia_Manager->InitRange_Second(11, 1,
			9999,
			1467.f, 70.f);
		this->TimeLoad_Dia_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Dia"));
		this->TimeLoad_Dia_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefreshDia"));
	}

	//
	this->DialogueShowInforDataTable.Empty();

	//获取当前是否有“间章”
	if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
	{
		this->DialogueShowInforDataTable.Emplace(
			FDialogueShowInforDataTable(
				UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueInfor.DialogueTitle,
				UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueInfor.DialogueTarget
			)
		);
	}
	else {
		//获取主线最新进度
		int32 CurIndex = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerCurrentDialogueID;

		//加载数据表
		TSoftObjectPtr<UDataTable> DatasPtr(
			FSoftObjectPath(
				TEXT("DataTable'/Game/Resource/BP/Data/Dialogue/DT_MainDialogueList.DT_MainDialogueList'"))
		);
		DatasPtr.LoadSynchronous();
		UDataTable* NewData = DatasPtr.Get();
		if (IsValid(NewData))
		{
			TArray<FTableRowBase*> SourceDatas;
			NewData->GetAllRows(TEXT("DialogueDatas"), SourceDatas);
			if (CurIndex < SourceDatas.Num())
			{
				FTableRowBase* CurDia = SourceDatas[CurIndex];
				FDialogueDataAssetDataTable* Cur = (FDialogueDataAssetDataTable*)(CurDia);

				TSoftObjectPtr<UDialogueDataAsset> CopyDia = Cur->DialogueDataAsset;

				CopyDia.LoadSynchronous();
				UDialogueDataAsset* TargetDia = CopyDia.Get();

				if (IsValid(TargetDia))
				{
					this->DialogueShowInforDataTable.Emplace(
						FDialogueShowInforDataTable(
							TargetDia->DialogueTitle,
							TargetDia->DialogueTarget)
					);
				}

				//释放
				CopyDia.Reset();
			}

			//释放
			DatasPtr.Reset();
		}
	}

	//常规添加已经完成的剧情
	for (
		const auto& CurPlayerFinishDia :
		UFVMGameInstance::GetPlayerStructManager_Static()->PlayerFinishedDialogue
		)
	{
		this->DialogueShowInforDataTable.Emplace(
			FDialogueShowInforDataTable(
				CurPlayerFinishDia.Key,
				TEXT("已观看此剧情"))
		);
	}

	this->TimeLoad_Dia_Manager->UpdateDatatable(this->DialogueShowInforDataTable);
	this->TimeLoad_Dia_Manager->SetLoadItemMaxCount(this->DialogueShowInforDataTable.Num());
	this->TimeLoad_Dia_Manager->ContinueRun();

	//UFVMGameInstance::GetPlayerStructManager_Static()->PlayerCurrentDialogueID = 10;
	//UFVMGameInstance::GetPlayerStructManager_Static()->Save();
}

UWidget* UUI_DialogueInfor::WidgetCreate_Dia(UItemDataTable* _Data, int32 _Index)
{
	UUI_DialogueListItem* Wid = CreateWidget<UUI_DialogueListItem>(this,
		LoadClass<UUI_DialogueListItem>(nullptr,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/Dialogue/BP_DialogueTitleItem.BP_DialogueTitleItem_C'")));

	FDialogueShowInforDataTable* CurD = ((FDialogueShowInforDataTable*)(_Data->GetValue()));

	Wid->Init(CurD->DialogueTitle, CurD->DialogueTarget, this);

	Wid->MainBG();

	if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
	{
		if (_Index == 0)
		{
			Wid->OtherBG();
		}
	}
	else {
		if (_Index == 0)
		{
			Wid->PlayerMainBG();
		}
	}

	return Wid;
}

void UUI_DialogueInfor::WidgetRefreshDia(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	UUI_DialogueListItem* Wid = Cast<UUI_DialogueListItem>(_UWidget);

	FDialogueShowInforDataTable* CurD = ((FDialogueShowInforDataTable*)(_Data->GetValue()));

	Wid->Init(CurD->DialogueTitle, CurD->DialogueTarget, this);

	Wid->MainBG();

	if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
	{
		if (_Index == 0)
		{
			Wid->OtherBG();
		}
	}
	else {
		if (_Index == 0)
		{
			Wid->PlayerMainBG();
		}
	}
}