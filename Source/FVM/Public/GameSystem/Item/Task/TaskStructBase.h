// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "TaskStructBase.generated.h"

/**
 * 任务结构
 */

class UTaskFunction;

//任务类型
UENUM(BlueprintType)
enum class EGameTaskType : uint8 {
	//主线任务
	MainTask UMETA(DisplayName = "MainTask"),
	//每日任务
	DayTask UMETA(DisplayName = "DayTask"),
	//升级任务
	PlayerUp UMETA(DisplayName = "PlayerUp"),
	//挑战任务
	Challenge UMETA(DisplayName = "Challenge")

	//注意，每一次新加类型后，需要去任务子系统更新任务的加载方式
};

//任务奖励结构
USTRUCT(BlueprintType)
struct FTaskIndex {
	GENERATED_USTRUCT_BODY()
public:
	//任务ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ID = 0;
	//任务类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGameTaskType TaskType = EGameTaskType::MainTask;
};

//任务奖励结构
USTRUCT(BlueprintType)
struct FTaskItemStruct {
	GENERATED_USTRUCT_BODY()
public:
	//奖励名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;
	//奖励个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemCount;
	//奖励指令
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemIns = TEXT("player send equip");
	//奖励图标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath ImagePath;
};

//任务表结构
USTRUCT(BlueprintType)
struct FTaskTableRowBase : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	//获取任务名称
	virtual	FString GetTaskName() const { return FString(); };
	//获取任务经验值
	virtual int32 GetTaskEx()const { return 0; };
	//获取任务货币
	virtual int32 GetTaskCoin() const { return 0; };
	//获取任务奖励
	virtual TArray<FTaskItemStruct> GetTaskItems() const { return TArray<FTaskItemStruct>(); };
	//获取任务类型
	virtual EGameTaskType GetTaskType() const { return EGameTaskType::MainTask; };
};


//任务完成后执行的对象
UCLASS(Blueprintable,BlueprintType)
class FVM_API UTaskFuncObjectExe : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnExe();
public:
	virtual	void OnInit();
};

//任务基本结构
USTRUCT(BlueprintType)
struct FTaskBase : public FTaskTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	//任务类型
	UPROPERTY()
		EGameTaskType TaskType = EGameTaskType::MainTask;
	//任务状态[新，进行中，未领取，完成]
	UPROPERTY()
		uint8 TaskState = 0;
	//任务ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TaskID = 0;
	//任务开启等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TaskLevel = 1;
	//任务类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<UTaskFunction> TaskFunctionClass = TSoftClassPtr<UTaskFunction>(FSoftObjectPath(TEXT("/Script/FVM.TaskFunction")));
	//任务名称{ID}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskName;
	//任务副标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskOtherTitle;
	//任务描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskContent;
	//任务目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskTarget;
	//任务经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TaskEx = 10;
	//任务金币资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TaskCoin = 100;
	//任务奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTaskItemStruct> TaskItems;
	//任务完成需要执行的对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<UTaskFuncObjectExe> ExeObject;
public:
	//获取任务名称
	virtual	FString GetTaskName() const override { return this->TaskName; };
	//获取任务经验值
	virtual int32 GetTaskEx()const override { return this->TaskEx; };
	//获取任务货币
	virtual int32 GetTaskCoin()const override { return this->TaskCoin; };
	//获取任务奖励
	virtual TArray<FTaskItemStruct> GetTaskItems()const override { return this->TaskItems; };
	//获取任务类型
	virtual EGameTaskType GetTaskType()const override { return this->TaskType; };
};

//关卡任务表结构
USTRUCT(BlueprintType)
struct FTaskDataTable : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskName;
};

//主线任务结构【线性】
USTRUCT(BlueprintType)
struct FMainTask : public FTaskBase {
	GENERATED_USTRUCT_BODY()
public:
	FMainTask() {
		this->TaskType = EGameTaskType::MainTask;
	};
};

//角色升级任务结构【线性】
USTRUCT(BlueprintType)
struct FPlayerUpTask : public FTaskBase {
	GENERATED_USTRUCT_BODY()
public:
	FPlayerUpTask() {
		this->TaskType = EGameTaskType::PlayerUp;
	};
};

//挑战任务结构【线性】
USTRUCT(BlueprintType)
struct FChallengeTask : public FTaskBase {
	GENERATED_USTRUCT_BODY()
public:
	FChallengeTask() {
		this->TaskType = EGameTaskType::Challenge;
	};
};

//每日任务结构【非线性】
USTRUCT(BlueprintType)
struct FDayTask : public FTaskBase {
	GENERATED_USTRUCT_BODY()
public:
	FDayTask() {
		this->TaskType = EGameTaskType::DayTask;
	};
};



UCLASS()
class FVM_API UTaskStructBase : public UObject
{
	GENERATED_BODY()
public:
	template<class T = FTaskBase>
	static UTaskStructBase* MakeTaskObject(const T& Data)
	{
		UTaskStructBase* Cur = NewObject<UTaskStructBase>();
		Cur->Data = Data;
		Cur->TaskType = Data.GetTaskType();
		return Cur;
	}
public:
	//获取任务名称
	const FString& GetTaskName() const;
	//获取任务ID
	const int32& GetTaskID() const;
	//获取任务类型
	const EGameTaskType& GetTaskType() const;
	//获取数据
	const FTaskBase& GetData() const;
private:
	UPROPERTY()
		FTaskBase Data;
	UPROPERTY()
		EGameTaskType TaskType = EGameTaskType::MainTask;
};
