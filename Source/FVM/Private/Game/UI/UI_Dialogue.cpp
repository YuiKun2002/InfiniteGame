// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Dialogue.h"
#include <Components/CanvasPanel.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/ScrollBox.h>
#include <Components/TextBlock.h>
#include "GameSystem/FVMGameInstance.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/DialogueSubsystem.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

bool UUI_DialogueSelect::Initialize()
{
	Super::Initialize();

	this->Content = this->GetWidgetComponent<UTextBlock>(this, TEXT("Content_1"));

	return true;
}

void UUI_DialogueSelect::Select()
{
	if (IsValid(this->UI))
	{
		this->UI->OnDialogueSelection.Broadcast(this->DiaSelectIndex);
		this->UI->SelectPanel->SetVisibility(ESlateVisibility::Collapsed);
		this->UI->SetNextDialogue(this->JumpIndex);
		this->UI->ContineDialogue();
	}
}

void UUI_DialogueSelect::Init(UUI_Dialogue* Class, const FString& Cont, const int32& Index, const int32& SelectIndex)
{
	this->UI = Class;
	this->Content->SetText(FText::FromString(Cont));
	this->JumpIndex = Index;
	this->DiaSelectIndex = SelectIndex;
}

bool UUI_Dialogue::Initialize()
{
	Super::Initialize();

	this->MainPanel = this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Main"));
	this->SelectPanel = this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Select"));
	this->ChangePanel = this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Change"));

	this->LeftChar_Img = this->GetWidgetComponent<UImage>(this, TEXT("LeftHead"));
	this->RightChar_Img = this->GetWidgetComponent<UImage>(this, TEXT("RightHead"));

	this->CharName_Txt = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_108"));
	this->TextContent_Txt = this->GetWidgetComponent<UTextBlock>(this, TEXT("Content"));

	this->SelectDialogueList = this->GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_30"));

	this->ChangeText = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_349"));

	return true;
}

void UUI_Dialogue::NativeDestruct()
{
	Super::NativeDestruct();

	//执行多播委托
	this->OnDialogueComplete.Broadcast();
	//解除所有绑定
	this->OnDialogueComplete.RemoveAll(this);
	this->OnDialogueSelection.RemoveAll(this);
}

void UUI_Dialogue::SetDialogue(int32 ID, const TSoftObjectPtr<UDialogueDataAsset>& Datas)
{
	this->CurDiaDataTable = Datas.Get()->DialogueDataTable;
	if (!IsValid(this->CurDiaDataTable.Get()))
	{
		this->CurDiaDataTable.LoadSynchronous();
	}

	//加载数据表
	this->CurDiaData = Datas;
	this->CurDialogueDataTable = this->CurDiaDataTable.Get();
	this->CurIndex = 0;

	//赋值信息
	if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
	{
		UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueInfor.DialogueTitle =
			Datas.Get()->DialogueTitle;
		UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueInfor.DialogueTarget =
			Datas.Get()->DialogueTarget;
		//赋值数据表
		UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueDataAsset = Datas;
	}
	else {
		UFVMGameInstance::GetPlayerStructManager_Static()->CurrentDialogueInfor.DialogueTitle =
			Datas.Get()->DialogueTitle;
		UFVMGameInstance::GetPlayerStructManager_Static()->CurrentDialogueInfor.DialogueTarget =
			Datas.Get()->DialogueTarget;

		this->CurID = ID;
		UFVMGameInstance::GetPlayerStructManager_Static()->PlayerCurrentDialogueID = ID;
	}

	//加载数据
	this->CurDialogue.Empty();
	TArray<FTableRowBase*> Dialogues;
	this->CurDialogueDataTable->GetAllRows(TEXT("PlayerDialogue"), Dialogues);
	for (FTableRowBase* CurDia : Dialogues)
	{
		FDialogueDataTable Temp = *((FDialogueDataTable*)(CurDia));
		this->CurDialogue.Emplace(Temp);
	}

	UFVMGameInstance::GetPlayerStructManager_Static()->Save();

	//开始对话
	this->ExeDialogueFirst();
}

void UUI_Dialogue::ContineDialogue()
{
	if (this->CurIndex < this->CurDialogue.Num())
	{
		this->CurDialogueSturct = this->CurDialogue[this->CurIndex];
		this->ExeDialogue(this->CurDialogueSturct.Type, this->CurIndex, this->CurDialogueSturct);
		this->CurIndex++;
	}
	else {
		this->DialogueComplete();
	}
}

void UUI_Dialogue::DialogueComplete()
{
	this->ExeDialogueFinished(this->CurDialogueSturct);

	//触发剧情
	UFVMGameInstance::GetPlayerStructManager_Static()->bShowDialogue = true;

	//关卡外自动保存，关卡内手动保存
	if (!IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
		{
			//赋值数据表
			UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueDataAsset = this->CurDiaData;
		}

		UDialogueSubsystem::SaveCurrentDialogue();
	}
	else {
		//如果在关卡内，不会保存上一次的对话ID【方便回溯】
		//更新当前的对话ID进度
		if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
		{
			//赋值数据表
			UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueDataAsset = this->CurDiaData;
		}
		else {
			UFVMGameInstance::GetPlayerStructManager_Static()->PlayerCurrentDialogueID = this->CurID;
		}
		UFVMGameInstance::GetPlayerStructManager_Static()->Save();
	}

	//释放当前加载的数据表
	if (IsValid(this->CurDiaData.Get()))
	{
		//释放数据表
		this->CurDiaDataTable.Reset();
		//释放对话内容
		this->CurDiaData.Reset();
	}
}

void UUI_Dialogue::SetNextDialogue(const int32& Index)
{
	this->CurIndex = Index;
}

void UUI_Dialogue::AddSelect(const TMap<FString, int32>& SelectItems)
{
	if (IsValid(this->SelectPanel))
	{
		//委托解绑
		
		//显示界面
		this->SelectPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		//添加缓存UI
		if (IsValid(this->SelectDialogueList))
		{
			int32 Count = this->SelectDialogueList->GetChildrenCount();
			if (SelectItems.Num() > Count)
			{
				int32 NewCount = SelectItems.Num() - Count;
				for (int32 i = 0; i < NewCount; i++) {
					UUI_DialogueSelect* NewSelect = CreateWidget<UUI_DialogueSelect>(this,
						LoadClass<UUI_DialogueSelect>(nullptr,
							TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/Dialogue/BP_DialogueSelect.BP_DialogueSelect_C'")));
					this->SelectDialogueList->AddChild(NewSelect);
				}
			}
		}

		int32 i = 0;
		for (const auto& CurItem : SelectItems)
		{
			UUI_DialogueSelect* Cur = Cast<UUI_DialogueSelect>(this->SelectDialogueList->GetChildAt(i));
			if (IsValid(Cur))
			{
				Cur->Init(this, CurItem.Key, CurItem.Value, i);
			}
			i++;
		}
	}
}
