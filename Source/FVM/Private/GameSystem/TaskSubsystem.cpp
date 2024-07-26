// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/TaskSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Item/PlayerRecord.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"

UTaskSubsystem* UTaskSubsystem::GetTaskSubsystemStatic()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetSubsystem<UTaskSubsystem>();
	}

	return nullptr;
}

void UTaskSubsystem::MoveTaskIndexSingle(
	const FTaskIndex& TaskIndex,
	TArray<FTaskIndex>& CurTaskIndexArray,
	TArray<FTaskIndex>& MoveTargetTaskIndex
)
{
	int32 i = 0;
	for (const FTaskIndex& CurTaskIndex : CurTaskIndexArray)
	{
		if (CurTaskIndex.ID == TaskIndex.ID && CurTaskIndex.TaskType == TaskIndex.TaskType)
		{
			MoveTargetTaskIndex.Emplace(TaskIndex);
			break;
		}
		i++;
	}
	CurTaskIndexArray.RemoveAtSwap(i);
}

void UTaskSubsystem::SetTaskStateByTaskIndexSingle(
	uint8 TaskState,
	const FTaskIndex& CurTaskIndex,
	TArray<FTaskBase>& InputTaskArray
)
{
	for (FTaskBase& TaskIndex : InputTaskArray)
	{
		if (CurTaskIndex.ID == TaskIndex.TaskID && CurTaskIndex.TaskType == TaskIndex.TaskType)
		{
			TaskIndex.TaskState = 3;
			return;
		}
	}
}

void UTaskSubsystem::LoadTaskSystem()
{
	//加载角色数据
	this->Player = UFVMGameInstance::GetPlayerStructManager_Static();

	/*int32 i = 0;
	for (auto& Cur : this->Player->CurFinishTask)
	{
		if (Cur.ID == 4 && Cur.TaskType == EGameTaskType::Challenge)
		{
			this->Player->CurFinishTask.RemoveAt(i);
			break;
		}
		i++;
	}
	this->Player->Save();*/


	//加载系统数据
	this->PlayerName = this->Player->M_PlayerName;
	this->TasksHandle.Empty();
	this->TasksIndex.Empty();
	this->bUpdateUI = true;

	//初始化主线任务
	this->InitMainTask();
	//初始化每日任务
	this->InitDayTask();
	//初始化支线任务
	this->InitPlayerUpTask();
	this->InitChallengeTask();
}

void UTaskSubsystem::ExecuteTasks(UObject* ObjectClass)
{
	//需要移动的任务
	TArray<FTaskIndex> FinishIns;
	TMap<class UTaskFunction*, class UTaskStructBase*> FinishHandle;
	//当前已经完成的任务类型
	bool bFinish = false;

	//执行所有任务
	for (const auto& Task : this->TasksHandle)
	{
		if (IsValid(Task.Key) && IsValid(Task.Value))
		{
			//符合等级条件即可执行任务
			if (this->Player->M_FPlayerEx.M_PlayerGrade >= Task.Value->GetData().TaskLevel)
			{
				if (Task.Key->ExeTask(Task.Value->GetTaskName(), ObjectClass))
				{
					//添加已经完成的任务类型
					bFinish = true;

					//更新显示UI
					this->bUpdateUI = true;

					FTaskIndex TempIndex;
					TempIndex.ID = Task.Value->GetTaskID();
					TempIndex.TaskType = Task.Value->GetTaskType();

					//将完成的任务移动
					FinishIns.Emplace(TempIndex);
					FinishHandle.Emplace(Task.Key, Task.Value);

					UGameUserInterfaceSubsystem::ShowTaskFinishTipUISub(
						nullptr,
						UFVMGameInstance::GetFVMGameInstance()
					);
				}
			}
		}
	}

	if (bFinish)
	{
		//重新赋值角色
		this->Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

		//移除任务
		if (FinishIns.Num() == 1)
		{
			this->RemoveSingleTask(FinishIns[0]);
		}
		else {
			this->RemoveTask(FinishHandle, this->TasksHandle);
		}

		//添加完成的任务
		for (const FTaskIndex& CurIndex : FinishIns)
		{
			if (CurIndex.TaskType == EGameTaskType::DayTask)
			{
				Player->CurFinishDayTaskNotClaimed.Emplace(CurIndex);

				/*FTaskBase Base;
				if (this->FindTask(CurIndex.ID, CurIndex.TaskType, Base))
				{
					UE_LOG(LogTemp, Warning, TEXT("向玩家添加完成任务：%s"), *Base.TaskName);
				}*/
			}
			else {
				Player->CurFinishTaskNotClaimed.Emplace(CurIndex);

				/*FTaskBase Base;
				if (this->FindTask(CurIndex.ID, CurIndex.TaskType, Base))
				{
					UE_LOG(LogTemp, Warning, TEXT("向玩家添加完成任务：%s"), *Base.TaskName);
				}*/
			}
		}

		UGameSystemFunction::SaveCurrentPlayerData();
	}
}

void UTaskSubsystem::GetTasksList(TArray<FTaskBase>& Out)
{
	//当前正在执行的任务
	for (const auto& Task : this->TasksHandle)
	{
		if (this->Player->M_FPlayerEx.M_PlayerGrade >= Task.Value->GetData().TaskLevel)
		{
			//UE_LOG(LogTemp, Error, TEXT("需要执行的任务：%s"), *Task.Value->GetTaskName());
			FTaskBase Cur = Task.Value->GetData();
			Cur.TaskType = Task.Value->GetTaskType();
			Out.Emplace(Cur);
		}
	}

	//添加玩家未领取的任务[任务]
	for (const FTaskIndex& CurIndex : this->Player->CurFinishTaskNotClaimed)
	{
		FTaskBase Cur;
		if (this->FindTask(CurIndex.ID, CurIndex.TaskType, Cur))
		{
			//UE_LOG(LogTemp, Warning, TEXT("已经完成的任务：%s"), *Cur.TaskName);
			Cur.TaskState = 2;
			Cur.TaskType = CurIndex.TaskType;
			Out.Emplace(Cur);
		}
		else {
			//UE_LOG(LogTemp, Error, TEXT("查询任务失败"));
		}
	}

	//添加玩家未领取的任务[特殊刷新任务，每日]
	for (const FTaskIndex& CurIndex : this->Player->CurFinishDayTaskNotClaimed)
	{
		FTaskBase Cur;
		if (this->FindTask(CurIndex.ID, EGameTaskType::DayTask, Cur))
		{
			//UE_LOG(LogTemp, Warning, TEXT("已经完成的任务：%s"), *Cur.TaskName);

			Cur.TaskState = 2;
			Cur.TaskType = CurIndex.TaskType;
			Out.Emplace(Cur);
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("查询任务失败"));
		}
	}

	/*for (const FTaskIndex& CurIndex : this->Player->CurFinishTask)
	{
		FTaskBase Cur;
		if (this->FindTask(CurIndex.ID, CurIndex.TaskType, Cur))
		{
			UE_LOG(LogTemp, Error, TEXT("已经领取的任务：%s"), *Cur.TaskName);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("查询任务失败"));
		}
	}

	for (const FTaskIndex& CurIndex : this->Player->CurFinishDayTask)
	{
		FTaskBase Cur;
		if (this->FindTask(CurIndex.ID, EGameTaskType::DayTask, Cur))
		{
			UE_LOG(LogTemp, Error, TEXT("已经领取的每日任务：%s"), *Cur.TaskName);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("查询任务失败"));
		}
	}*/
}

FString UTaskSubsystem::GetPlayerName()
{
	return this->PlayerName;
}

bool UTaskSubsystem::FindTask(int32 TaskID, EGameTaskType TaskType, FTaskBase& Out)
{
	switch (TaskType)
	{
	case EGameTaskType::MainTask:return this->FindTask<FMainTask>(TaskID, this->MainTask, Out);
	case EGameTaskType::DayTask:return this->FindTask<FDayTask>(TaskID, this->DayTask, Out);
	case EGameTaskType::PlayerUp:return this->FindTask<FPlayerUpTask>(TaskID, this->PlayerUpTask, Out);
	case EGameTaskType::Challenge:return this->FindTask<FChallengeTask>(TaskID, this->ChallengeTask, Out);
	default:
		break;
	}

	return false;
}

bool UTaskSubsystem::FindFinishedTask(int32 TaskID, EGameTaskType TaskType)
{
	//重新赋值角色
	this->Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	if (IsValid(this->Player))
	{
		if (TaskType == EGameTaskType::DayTask)
		{
			for (const FTaskIndex& CurTask : this->Player->CurFinishDayTask)
			{
				if (TaskID == CurTask.ID)
				{
					return true;
				}
			}
		}
		else {
			for (const FTaskIndex& CurTask : this->Player->CurFinishTask)
			{
				if (TaskID == CurTask.ID && CurTask.TaskType == TaskType)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool UTaskSubsystem::GetUpdate()
{
	bool bTemp = this->bUpdateUI;

	this->bUpdateUI = false;

	return bTemp;
}

bool UTaskSubsystem::GetHasFinishTask()
{
	if (this->Player)
	{
		if (this->Player->CurFinishDayTaskNotClaimed.Num())
		{
			return true;
		}
		if (this->Player->CurFinishTaskNotClaimed.Num())
		{
			return true;
		}
	}

	return false;
}

void UTaskSubsystem::UpdateTask(const EGameTaskType& NewTaskType)
{
	switch (NewTaskType)
	{
	case EGameTaskType::MainTask: {
		this->InitMainTask();
	}; break;
	case EGameTaskType::DayTask:
	{
		this->InitDayTask();
	}; break;
	case EGameTaskType::PlayerUp:
	{
		this->InitPlayerUpTask();
	}; break;
	case EGameTaskType::Challenge:
	{
		this->InitChallengeTask();
	}; break;
	default:
		break;
	}
}

void UTaskSubsystem::AddNewTask(const FTaskBase& Task)
{

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			FString(TEXT("添加新任务：")) + Task.GetTaskName() + TEXT("  Type:") +
			FString::FromInt(((int32)Task.GetTaskType()))
		);
	}

	//查询是否存在重复的ID
	for (const auto& CurIndex : this->TasksIndex)
	{
		if (Task.TaskID == CurIndex.ID && Task.GetTaskType() == CurIndex.TaskType)
		{
			return;
		}
	}

	//添加任务句柄
	FTaskIndex TaskIndex;
	TaskIndex.ID = Task.TaskID;
	TaskIndex.TaskType = Task.GetTaskType();

	UTaskFunction* CurNewTaskHandle = nullptr;
	UClass* CurClass = Task.TaskFunctionClass.LoadSynchronous();
	if (IsValid(CurClass))
	{
		TSubclassOf<UTaskFunction> CurFunClass(CurClass);
		if (CurFunClass.GetDefaultObject())
		{
			CurNewTaskHandle = CurFunClass.GetDefaultObject()->MakeNewClass();
		}
	}

	this->TasksHandle.Emplace(CurNewTaskHandle, UTaskStructBase::MakeTaskObject(Task));
	this->TasksIndex.Emplace(TaskIndex);
}

bool UTaskSubsystem::RemoveSingleTask(const FTaskIndex& NewTaskIndex)
{
	int32 Index = 0;

	for (const FTaskIndex& CurIndex : this->TasksIndex)
	{
		if (CurIndex.ID == NewTaskIndex.ID && CurIndex.TaskType == NewTaskIndex.TaskType)
		{
			this->TasksIndex.RemoveAt(Index);

			TArray<UTaskFunction*> CurKeys;
			this->TasksHandle.GetKeys(CurKeys);
			this->TasksHandle.Remove(CurKeys[Index]);

			return true;
		}

		Index += 1;
	}

	return true;
}

void UTaskSubsystem::RemoveTask(
	const TMap<class UTaskFunction*, class UTaskStructBase*>& Tasks,
	TMap<class UTaskFunction*, class UTaskStructBase*>& OutTasks
)
{
	TMap<class UTaskFunction*, class UTaskStructBase*> Temp = OutTasks;
	OutTasks.Empty();

	int32 Index = 0;
	for (const auto& CurTask : Tasks)
	{
		Temp.Remove(CurTask.Key);
		this->TasksIndex.RemoveAt(Index);
		Index += 1;
	}

	OutTasks.Append(Temp);
}

void UTaskSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	////加载关卡任务数据表
	//UGameSystemFunction::GetDataTableRows<FLevelTaskDataTable>(
	//	TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_关卡通关任务.DT_关卡通关任务'"),
	//	this->LevelTaskDataTable
	//	);

	/*

	//任务线数据表，必定是要加载的。

	*/

	//加载主线任务数据表
	UGameSystemFunction::GetDataTableRows<FMainTask>(
		TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_MainTask.DT_MainTask'"),
		this->MainTask
		);
	//加载每日任务数据表
	UGameSystemFunction::GetDataTableRows<FDayTask>(
		TEXT("DataTable'/Game/Resource/BP/Data/Task/Day/DT_DayTask.DT_DayTask'"),
		this->DayTask
		);
	//加载支线任务数据表
	UGameSystemFunction::GetDataTableRows<FPlayerUpTask>(
		TEXT("DataTable'/Game/Resource/BP/Data/Task/Other/DT_PlayerUpTask.DT_PlayerUpTask'"),
		this->PlayerUpTask
		);
	UGameSystemFunction::GetDataTableRows<FChallengeTask>(
		TEXT("DataTable'/Game/Resource/BP/Data/Task/Other/DT_ChallengeTask.DT_ChallengeTask'"),
		this->ChallengeTask
		);
}

void UTaskSubsystem::Deinitialize()
{
	//卸载
	this->LevelTaskDataTable.Empty();
	this->MainTask.Empty();
	this->TasksHandle.Empty();
	this->DayTask.Empty();
	this->PlayerUpTask.Empty();
	this->ChallengeTask.Empty();
	this->SynMakeCardTaskDataTable.Empty();
	this->SynCardUpGradeDataTable.Empty();
	this->LookInforTaskDataTable.Empty();
	this->CardSkillTaskDataTable.Empty();
	this->GemTaskDataTable.Empty();
}

void UTaskSubsystem::InitMainTask()
{
	this->AddLineTask(this->MainTask, EGameTaskType::MainTask, Player->CurFinishTask, Player->CurFinishTaskNotClaimed);
}

void UTaskSubsystem::InitPlayerUpTask()
{
	this->AddLineTask(this->PlayerUpTask, EGameTaskType::PlayerUp, Player->CurFinishTask, Player->CurFinishTaskNotClaimed);
}

void UTaskSubsystem::InitChallengeTask()
{
	this->AddLineTask(this->ChallengeTask, EGameTaskType::Challenge, Player->CurFinishTask, Player->CurFinishTaskNotClaimed);
}

void UTaskSubsystem::InitDayTask()
{
	if (this->Player->M_FPlayerEx.M_PlayerGrade >= 3)
	{
		FDateTime GameSystem_Time_Now = FDateTime::Now();
		FString Year = FString::FromInt(GameSystem_Time_Now.GetYear());
		FString Month = FString::FromInt(GameSystem_Time_Now.GetMonth());
		FString Day = FString::FromInt(GameSystem_Time_Now.GetDay());
		FString Target = Year + TEXT("-") + Month + TEXT("-") + Day;

		if (!this->Player->DayTaskTime.Equals(Target))
		{
			//设置时间
			this->Player->DayTaskTime = Target;
			//任务刷新
			this->Player->CurFinishDayTask.Empty();
			this->Player->CurFinishDayTaskNotClaimed.Empty();
			//直接新增登陆完成任务
			FTaskIndex LoginGameDayTask;
			LoginGameDayTask.ID = 6;
			LoginGameDayTask.TaskType = EGameTaskType::DayTask;
			this->Player->CurFinishDayTaskNotClaimed.Emplace(LoginGameDayTask);
			//清理每日累积
			UPlayerRecord::InitDayRecord();
			//保存
			UGameSystemFunction::SaveCurrentPlayerData();
		}

		//更新任务

		//当前完成的每日任务
		TArray<FTaskIndex> CurFinish = this->Player->CurFinishDayTask;
		CurFinish.Append(this->Player->CurFinishDayTaskNotClaimed);

		//可以更新到列表的任务
		TArray<FDayTask> TargetDayTask;
		//当前全部每日任务
		for (const FDayTask& CurDayTask : this->DayTask)
		{
			bool bResult = true;


			int32 i = 0;
			for (const FTaskIndex& TaskIndex : CurFinish)
			{
				if (TaskIndex.ID == CurDayTask.TaskID)
				{
					bResult = false;
					CurFinish.RemoveAtSwap(i);
					break;
				}
				i++;
			}

			if (bResult)
			{
				//添加任务
				TargetDayTask.Emplace(CurDayTask);
			}
		}

		for (const FDayTask& CurDayTask : TargetDayTask)
		{
			this->AddNewTask(CurDayTask);
		}
	}
	else {
		this->Player->DayTaskTime = TEXT("");
	}
}