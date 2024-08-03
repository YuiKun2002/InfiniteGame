// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/DialogueSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "Engine/DataTable.h"
#include "Game/UI/UI_Dialogue.h"

UUI_Dialogue* UDialogueSubsystem::RunDialogueByID(
	int32 DialogueID,
	TSoftObjectPtr<UDialogueDataAsset> Dialogue
)
{
	//如果有简章加入，则优先执行。除非手动取消，或者主动完成！
	if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
	{
		return nullptr;
	}

	//剧情ID判断【回溯到上次的进度】
	if (UDialogueSubsystem::GetLastDialogueID() != DialogueID)
	{
		//判断是否在游戏中
		if (!IsValid(AGameMapInstance::GetGameMapInstance()))
		{
			return nullptr;
		}
		else {
			//如果在游戏中，上一次保存的ID,大于新传入的ID，则表示这个剧情已经通过！
			if (UDialogueSubsystem::GetLastDialogueID() >= DialogueID)
			{
				return nullptr;
			}
		}
	}

	if (!IsValid(Dialogue.Get()))
	{
		Dialogue.LoadSynchronous();
	}

	UDialogueDataAsset* NewDia = Dialogue.Get();
	if (IsValid(NewDia))
	{
		return UDialogueSubsystem::ShowDialogueWidget(DialogueID, Dialogue);
	}

	return nullptr;
}

UUI_Dialogue* UDialogueSubsystem::JoinDialogue(TSoftObjectPtr<UDialogueDataAsset> Dialogue)
{
	//如果已经有间章，则无法触发
	if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
	{
		return nullptr;
	}

	//获取游戏配置
	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
	UPlayerStructManager* CurPlayer = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	if (!IsValid(Dialogue.Get()))
	{
		Dialogue.LoadSynchronous();
	}

	UDialogueDataAsset* NewDia = Dialogue.Get();

	if (IsValid(NewDia))
	{
		if (CurPlayer->FinishOtherDialogueID.Contains(NewDia->DialogueID))
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue = false;
			return nullptr;
		}
		else {
			UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue = true;
			return UDialogueSubsystem::ShowDialogueWidget(-1, Dialogue);
		}
	}

	UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue = false;
	return nullptr;
}

int32 UDialogueSubsystem::GetLastDialogueID()
{
	return UFVMGameInstance::GetPlayerStructManager_Static()->PlayerLastDialogueID;
}

void UDialogueSubsystem::SaveCurrentDialogue()
{
	if (UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue)
	{
		if (IsValid(
			UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueDataAsset.LoadSynchronous()
		))
		{
			//获取游戏配置
			UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
			UPlayerStructManager* CurPlayer = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

			//保存剧情
			if (IsValid(CurPlayer))
			{
				//保存
				CurPlayer->FinishOtherDialogueID.Emplace(
					UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueDataAsset.Get()->DialogueID
				);

				//完成对话,获取当前剧情的标题
				UFVMGameInstance::GetPlayerStructManager_Static()->PlayerFinishedDialogue.Emplace(
					UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueInfor.DialogueTitle,
					UFVMGameInstance::GetPlayerStructManager_Static()->JoinDialogueInfor.DialogueTarget
				);

				UFVMGameInstance::GetPlayerStructManager_Static()->bShowDialogue = false;
				UFVMGameInstance::GetPlayerStructManager_Static()->bJoinDialogue = false;
				UFVMGameInstance::GetPlayerStructManager_Static()->Save(__FUNCTION__ + FString(TEXT("{间章}剧情完成")));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetPlayerStructManager_Static()->bShowDialogue)
		{
			//更新当前对话ID
			UFVMGameInstance::GetPlayerStructManager_Static()->PlayerCurrentDialogueID += 1;

			//完成对话,获取当前剧情的标题
			UFVMGameInstance::GetPlayerStructManager_Static()->PlayerFinishedDialogue.Emplace(
				UFVMGameInstance::GetPlayerStructManager_Static()->CurrentDialogueInfor.DialogueTitle,
				UFVMGameInstance::GetPlayerStructManager_Static()->CurrentDialogueInfor.DialogueTarget
			);

			//更新上一次的对话ID
			UFVMGameInstance::GetPlayerStructManager_Static()->PlayerLastDialogueID
				= UFVMGameInstance::GetPlayerStructManager_Static()->PlayerCurrentDialogueID;

			//赋值信息
			UFVMGameInstance::GetPlayerStructManager_Static()->LastDialogueInfor =
				UFVMGameInstance::GetPlayerStructManager_Static()->CurrentDialogueInfor;

			UFVMGameInstance::GetPlayerStructManager_Static()->bShowDialogue = false;

			UFVMGameInstance::GetPlayerStructManager_Static()->Save(__FUNCTION__ + FString(TEXT("剧情完成")));
		}
	}
}

UUI_Dialogue* UDialogueSubsystem::ShowDialogueWidget(int32 ID, const TSoftObjectPtr<UDialogueDataAsset>& Datas)
{
	UDialogueSubsystem* Dia = UFVMGameInstance::GetFVMGameInstance()->GetSubsystem<UDialogueSubsystem>();
	if (IsValid(Dia))
	{
		UUI_Dialogue*& Cur = Dia->UI_Dialogue;

		if (!IsValid(Cur))
		{
			//初始化UI
			Cur = CreateWidget<UUI_Dialogue>(UFVMGameInstance::GetFVMGameInstance(),
				LoadClass<UUI_Dialogue>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/BP_Dialogue.BP_Dialogue_C'")));
		}

		if (!Cur->IsInViewport())
		{
			//添加到视口
			Cur->AddToViewport(500);
			//设置剧本
			Cur->SetDialogue(ID, Datas);
			//返回剧本
			return Cur;
		}

		return nullptr;
	}

	return nullptr;
}
