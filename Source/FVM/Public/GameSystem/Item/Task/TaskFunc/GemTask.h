// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"
#include "GemTask.generated.h"


//宝石任务类型
UENUM(BlueprintType)
enum class EGemTaskType : uint8
{
	GemAttach UMETA(DisplayName = "GemAttach"),
	GemUpGrade UMETA(DisplayName = "GemUpGrade"),
	GemSplit UMETA(DisplayName = "GemSplit")
};

//宝石任务表结构
USTRUCT(BlueprintType)
struct FGemTaskDataTable : public FTaskDataTable {
	GENERATED_USTRUCT_BODY()
public:
	//宝石任务类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGemTaskType GemTaskType = EGemTaskType::GemAttach;
	//需要操作的宝石名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString GemName = TEXT("");
	//需要强化的等级【0表示任意等级】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 GemUpGrade = 0;
};


/**
 *宝石任务
 */
UCLASS()
class FVM_API UGemTask : public UTaskFunction
{
	GENERATED_BODY()
public:
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) override;
	virtual UTaskFunction* MakeNewClass() override;
protected:
	bool GetData(FGemTaskDataTable& OutData, const FString& TaskName);
};
