// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueSubsystem.generated.h"

class UDataTable;
class UDialogueDataAsset;
class UUI_Dialogue;

/**
 * 对话子系统
 */
UCLASS()
class FVM_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/*
		执行对话系统
	@Param1 主线剧情ID，每次完成一个对话，ID自增。必须确保ID唯一
	@Param2 对话剧情文本
	*/
	UFUNCTION(BlueprintCallable)
		static UUI_Dialogue* RunDialogueByID(
			int32 DialogueID,
			TSoftObjectPtr<UDialogueDataAsset> Dialogue
		);
	
	//插曲简章剧情【如果已经有简章剧情则无法加入新剧情】
	UFUNCTION(BlueprintCallable)
		static UUI_Dialogue* JoinDialogue(TSoftObjectPtr<UDialogueDataAsset> Dialogue);
	//获取上一次的对话ID
	UFUNCTION(BlueprintCallable)
		static int32 GetLastDialogueID();
	//保存当前剧情对话进度
	UFUNCTION(BlueprintCallable)
		static void SaveCurrentDialogue();
private:
	static UUI_Dialogue* ShowDialogueWidget(int32 ID,const TSoftObjectPtr<UDialogueDataAsset>& Datas);
private:
	//对话UI
	UPROPERTY()
		UUI_Dialogue* UI_Dialogue = nullptr;
};
