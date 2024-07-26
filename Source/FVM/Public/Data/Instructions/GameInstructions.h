// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Data/GameLogSubsystem.h"
#include "GameInstructions.generated.h"

/**
 * 游戏指令
 */

 class AGameMapInstance;

UCLASS()
class FVM_API UGameInstructions : public ULogInstructionSystemBase
{
	GENERATED_BODY()

public:
	virtual bool ReadInstruction(const TArray<FString>& Instruction) override;
protected:
	UFUNCTION()
	AGameMapInstance* GetMap();
private:
	UFUNCTION()
		bool SingleIns(const TArray<FString>& Instruction);
	UFUNCTION()
		bool DoubleIns(const TArray<FString>& Instruction);
	UFUNCTION()
		bool ThreeIns(const TArray<FString>& Instruction);
private:
	UFUNCTION()
		bool PlayCard(FString CardName,FString CardGrade,int32 Row,int32 Col);
};
