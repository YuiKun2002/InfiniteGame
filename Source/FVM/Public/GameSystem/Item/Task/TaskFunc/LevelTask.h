// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"
#include "LevelTask.generated.h"


//关卡任务表结构
USTRUCT(BlueprintType)
struct FLevelTaskDataTable : public FTaskDataTable {
	GENERATED_USTRUCT_BODY()
public:
	//需要通关的地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MapName;
	//需要放置的卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> PlayCards;
	//需要击杀的老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> KillMouses;
	//需要收集的材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FString> MaterialName;
	//累计通关次数【单独数据，如果设置次数之后，上面数据将不会被使用】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WinCount = 0;
	//累计[只会争对卡片和老鼠]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Total = false;
};

//关卡任务
UCLASS()
class FVM_API ULevelTask : public UTaskFunction
{
	GENERATED_BODY()
public:
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) override;
	virtual UTaskFunction* MakeNewClass() override;
};
