// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"
#include "SynthesisTask.generated.h"

/**
 * 合成屋任务
 */

 //卡片制作
USTRUCT(BlueprintType)
struct FSynMakeCardTaskDataTable : public FTaskDataTable {
	GENERATED_USTRUCT_BODY()
public:
	//制作的卡片名称，不填则是任意卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MakeCardName;
	//制作次数，制作卡片的次数。【通过历史记录判断是否完成】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MakeCount = 1;
};

//卡片强化
USTRUCT(BlueprintType)
struct FSynCardUpGradeDataTable : public FTaskDataTable {
	GENERATED_USTRUCT_BODY()
public:
	//强化的卡片名称，不填则是任意卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString UpGradeCardName = TEXT("");
	//强化等级【等级0 卡片任意等级】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CardUpGrade = 1;
};

/**
 * 合成屋任务
 */
UCLASS()
class FVM_API USynthesisTask : public UTaskFunction
{
	GENERATED_BODY()
public:
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) override;
	virtual UTaskFunction* MakeNewClass() override;
private:
	void InitData();
	void InitData2();
};
