// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"
#include "CardSkillTask.generated.h"


//卡片技能书
USTRUCT(BlueprintType)
struct FCardSkillTaskDataTable : public FTaskDataTable {
	GENERATED_USTRUCT_BODY()
public:
	//指定的卡片技能书【不填则是任意】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CardSkillName = TEXT("");
};


/**
 * 卡片技能书任务
 */
UCLASS()
class FVM_API UCardSkillTask : public UTaskFunction
{
	GENERATED_BODY()
public:
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) override;
	virtual UTaskFunction* MakeNewClass() override;
};
