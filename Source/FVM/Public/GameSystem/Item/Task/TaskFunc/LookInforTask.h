// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"
#include "LookInforTask.generated.h"

 //查看内容
USTRUCT(BlueprintType)
struct FLookInforTaskDataTable : public FTaskDataTable {
	GENERATED_USTRUCT_BODY()
public:
	//查看情报岛内容【不填则不会完成任务】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FString> CurLook;
};

/**
 * 情报岛任务
 */
UCLASS()
class FVM_API ULookInforTask : public UTaskFunction
{
	GENERATED_BODY()
public:
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) override;
	virtual UTaskFunction* MakeNewClass() override;
};
