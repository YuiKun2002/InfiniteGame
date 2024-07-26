// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"

//关卡任务
#include "GameSystem/Item/Task/TaskFunc/LevelTask.h"
//合成屋-卡片制作
#include "GameSystem/Item/Task/TaskFunc/SynthesisTask.h"
//情报岛
#include "GameSystem/Item/Task/TaskFunc/LookInforTask.h"
//角色升级
#include "GameSystem/Item/Task/TaskFunc/PlayerUpTask.h"
//卡片技能书
#include "GameSystem/Item/Task/TaskFunc/CardSkillTask.h"
//宝石
#include "GameSystem/Item/Task/TaskFunc/GemTask.h"

#include "TaskSubsystem.generated.h"

class UPlayerStructManager;

/**
 * 任务子系统
 */
UCLASS()
class FVM_API UTaskSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//获取子系统
	UFUNCTION(BlueprintCallable, Category = "任务子系统")
		static UTaskSubsystem* GetTaskSubsystemStatic();

	/*
	* MoveTaskIndexSingle
	@param1 当前需要移动的任务索引
	@param2 当前移动的索引存放在那个位置
	@param3 需要将当前索引移动到目标位置
	*/
	UFUNCTION(BlueprintCallable, Category = "任务子系统 | 移动任务索引")
		static void MoveTaskIndexSingle(
			const FTaskIndex& TaskIndex,
			TArray<FTaskIndex>& CurTaskIndexArray,
			TArray<FTaskIndex>& MoveTargetTaskIndex
		);
	/*
	* SetTaskStateByTaskIndexSingle
	@param1 需要设置的任务状态【0-3】
	@param2 需要设置状态的任务索引
	@param3 设置到任务组
	*/
	UFUNCTION(BlueprintCallable, Category = "任务子系统 | 设置任务状态")
		static void SetTaskStateByTaskIndexSingle(
			uint8 TaskState,
			const FTaskIndex& CurTaskIndex,
			TArray<FTaskBase>& InputTaskArray
		);
public:
	//加载任务系统
	UFUNCTION(BlueprintCallable)
		void LoadTaskSystem();
	//执行系统任务
	UFUNCTION(BlueprintCallable)
		void ExecuteTasks(UObject* ObjectClass);
	//获取任务条目
	UFUNCTION(BlueprintCallable)
		void GetTasksList(TArray<FTaskBase>& Out);
	//获取玩家名称
	UFUNCTION(BlueprintCallable)
		FString GetPlayerName();
	//查询任务
	UFUNCTION(BlueprintCallable)
		bool FindTask(int32 TaskID, EGameTaskType TaskType, FTaskBase& Out);
	//查询已经完成的任务
	UFUNCTION(BlueprintCallable)
		bool FindFinishedTask(int32 TaskID, EGameTaskType TaskType);
	//是否更新UI刷新任务条目
	UFUNCTION(BlueprintCallable)
		bool GetUpdate();
	//是否有完成的任务
	UFUNCTION(BlueprintCallable)
		bool GetHasFinishTask();
public:
	//初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//取消初始化
	virtual void Deinitialize() override;
public:
	//关卡任务数据表
	UPROPERTY()
		TArray<FLevelTaskDataTable> LevelTaskDataTable;
	//角色升级
	UPROPERTY()
		TArray<FPlayerUpTaskDataTable> PlayerUpTaskDataTable;
	//情报岛
	UPROPERTY()
		TArray<FLookInforTaskDataTable> LookInforTaskDataTable;
	//合成屋，卡片制作任务数据表
	UPROPERTY()
		TArray<FSynMakeCardTaskDataTable> SynMakeCardTaskDataTable;
	//合成屋，卡片强化
	UPROPERTY()
		TArray<FSynCardUpGradeDataTable> SynCardUpGradeDataTable;
	//卡片技能书
	UPROPERTY()
		TArray<FCardSkillTaskDataTable> CardSkillTaskDataTable;
	//宝石
	UPROPERTY()
		TArray<FGemTaskDataTable> GemTaskDataTable;
protected:
	//角色管理器
	UPROPERTY()
		UPlayerStructManager* Player = nullptr;
	//角色名称
	UPROPERTY()
		FString PlayerName;
	//UI更新
	UPROPERTY()
		bool bUpdateUI = false;
	//任务句柄
	UPROPERTY()
		TMap<class UTaskFunction*, class UTaskStructBase*> TasksHandle;
	//任务索引
	UPROPERTY()
		TArray<FTaskIndex> TasksIndex;
protected:
	//主线任务
	UPROPERTY()
		TArray<FMainTask> MainTask;
	//支线任务
	UPROPERTY()
		TArray<FPlayerUpTask> PlayerUpTask;
	//支线任务
	UPROPERTY()
		TArray<FChallengeTask> ChallengeTask;
	//每日任务
	UPROPERTY()
		TArray<FDayTask> DayTask;
private:
	//更新任务
	void UpdateTask(const EGameTaskType& NewTaskType);
	//添加任务
	void AddNewTask(const FTaskBase& Task);
	//移除单个任务
	bool RemoveSingleTask(const FTaskIndex& NewTaskIndex);
	//批量移除任务
	void RemoveTask(
		const TMap<class UTaskFunction*, class UTaskStructBase*>& Tasks,
		TMap<class UTaskFunction*, class UTaskStructBase*>& OutTasks
	);

	//初始化主线任务
	void InitMainTask();
	//初始化角色升级任务
	void InitPlayerUpTask();
	//初始化挑战任务
	void InitChallengeTask();
	//初始化每日任务
	void InitDayTask();

private:
	//添加线性任务功能
	template<class T = FTaskBase>
	void AddLineTask(
		const TArray<T>& Tasks,
		const EGameTaskType& TaskType,
		const TArray<FTaskIndex>& Tasks1,
		const TArray<FTaskIndex>& Tasks2
	)
	{
		//已经完成并且领取的任务
		TArray<FTaskIndex> CurFinish = Tasks1;
		//任务完成但是未领取
		CurFinish.Append(Tasks2);

		for (const T& CurTask : Tasks)
		{
			bool bCurrent = false;

			//遍历已经完成的任务
			int32 TasksIndexI = 0;
			for (const FTaskIndex& TaskIndex : CurFinish)
			{
				if (TaskIndex.TaskType == TaskType)
				{
					if (TaskIndex.ID == CurTask.TaskID)
					{
						bCurrent = true;
						CurFinish.RemoveAtSwap(TasksIndexI);
						break;
					}
				}
				TasksIndexI++;
			}

			if (!bCurrent)
			{
				//添加主线任务句柄
				this->AddNewTask(CurTask);
				return;
			}
		}
	}

	template<class T = FTaskBase>
	bool FindTask(int32 TaskID, TArray<T> TaskDatas, FTaskBase& Out)
	{
		for (const T& Task : TaskDatas)
		{
			if (Task.TaskID == TaskID)
			{
				Out = Task;
				return true;
			}
		}

		return false;
	}

public:
	//查询任务数据表
	template<class T = FTaskDataTable>
	static T FindTaskData(FString TaskName, const TArray<T>& TaskDatas, bool& Result)
	{
		for (const T& Cur : TaskDatas)
		{
			if (Cur.TaskName.Equals(TaskName))
			{
				Result = true;
				return Cur;
			}
		}

		Result = false;
		return T();
	}

	//初始化任务数据表
	template<class T = FTaskDataTable>
	static void InitTaskData(FSoftObjectPath DataTablePath, TArray<T>& OutTaskDatas)
	{
		//如果当前有数据，将不会进行初始化
		if (OutTaskDatas.Num())
		{
			return;
		}

		//加载关卡任务数据表
		UGameSystemFunction::GetDataTableRows<T>(DataTablePath.ToString(), OutTaskDatas);
	}
};
