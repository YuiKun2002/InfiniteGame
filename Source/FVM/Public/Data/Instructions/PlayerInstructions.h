// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Data/GameLogSubsystem.h"
#include "PlayerInstructions.generated.h"

/**
 * 角色指令集合
 */
UCLASS()
class FVM_API UPlayerInstructions : public ULogInstructionSystemBase
{
	GENERATED_BODY()
public:
	virtual bool ReadInstruction(const TArray<FString>& Instruction) override;
private:
	//send指令
	UFUNCTION()
		bool SendIns(const TArray<FString>& Instruction);
	//发生装备指令
	UFUNCTION()
		bool SendEquip(const TArray<FString>& Instruction);
	//发送卡片指令
	UFUNCTION()
		bool SendCard(const TArray<FString>& Instruction);
	//发生材料指令
	UFUNCTION()
		bool SendMaterial(const TArray<FString>& Instruction);
	//--------------------------------------------------------------------------------
		//clear指令
	UFUNCTION()
		bool ClearIns(const TArray<FString>& Instruction);
	//--------------------------------------------------------------------------------
	//设置指令
	UFUNCTION()
		bool SetIns(const TArray<FString>& Instruction);
	//设置技能书
	UFUNCTION()
		bool SetSKIns(const TArray<FString>& Instruction);
	//设置角色等级
	UFUNCTION()
		bool SetPGIns(const TArray<FString>& Instruction);
	//设置货币
	UFUNCTION()
		bool SetCoinIns(const TArray<FString>& Instruction);
	//--------------------------------------------------------------------------------

	//回退指令
	UFUNCTION()
		bool ReturnIns(const TArray<FString>& Instruction);
	//回退剧情
	UFUNCTION()
		bool ReturnDialogue(const TArray<FString>& Instruction);
	//--------------------------------------------------------------------------------

};
