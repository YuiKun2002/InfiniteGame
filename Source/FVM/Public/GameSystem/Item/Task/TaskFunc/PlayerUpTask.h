// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除
#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"
#include "PlayerUpTask.generated.h"

//角色升级
USTRUCT(BlueprintType)
struct FPlayerUpTaskDataTable : public FTaskDataTable {
	GENERATED_USTRUCT_BODY()
public:
	//角色需要达到的等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Level = 1;
};

/**
 * 升级任务
 */
UCLASS()
class FVM_API UPlayerUpTaskFunc : public UTaskFunction
{
	GENERATED_BODY()
public:
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) override;
	virtual UTaskFunction* MakeNewClass() override;
};