// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/NoExportTypes.h"
#include "TaskInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UTaskInterface : public UInterface
{
	GENERATED_BODY()
};

class FVM_API ITaskInterface
{
	GENERATED_BODY()
public:
	//执行任务
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) = 0;
};

//任务功能
UCLASS()
class FVM_API UTaskFunction : public UObject, public ITaskInterface
{
	GENERATED_BODY()
public:
	virtual bool ExeTask(const FString& TaskName, UObject* TargetObject) override;
	virtual UTaskFunction* MakeNewClass();
};

