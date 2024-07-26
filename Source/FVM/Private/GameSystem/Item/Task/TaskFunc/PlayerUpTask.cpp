// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#include "GameSystem/Item/Task/TaskFunc/PlayerUpTask.h"
#include "GameSystem/TaskSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

bool UPlayerUpTaskFunc::ExeTask(const FString& TaskName, UObject* TargetObject)
{
	if (UPlayerStructManager* CurPlayer = Cast<UPlayerStructManager>(TargetObject))
	{
		//初始化数据表
		UTaskSubsystem::InitTaskData(
			FSoftObjectPath(TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_角色升级任务.DT_角色升级任务'")),
			UTaskSubsystem::GetTaskSubsystemStatic()->PlayerUpTaskDataTable
		);

		bool bRe;
		//拿到任务数据
		const FPlayerUpTaskDataTable& Cur = UTaskSubsystem::FindTaskData(TaskName,
			UTaskSubsystem::GetTaskSubsystemStatic()->PlayerUpTaskDataTable, bRe
		);

		if (!bRe)
		{
			return false;
		}

		if (CurPlayer->M_FPlayerEx.M_PlayerGrade >= Cur.Level)
		{
			return true;
		}
	}

	return false;
}

UTaskFunction* UPlayerUpTaskFunc::MakeNewClass()
{
	return NewObject<UPlayerUpTaskFunc>();
}
