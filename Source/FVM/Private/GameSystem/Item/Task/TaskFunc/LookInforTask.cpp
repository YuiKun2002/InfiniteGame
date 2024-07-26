// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Task/TaskFunc/LookInforTask.h"
#include "GameSystem/TaskSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "Game/UI/UI_GameInformation.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

bool ULookInforTask::ExeTask(const FString& TaskName, UObject* TargetObject)
{
	if (UUI_GameInformationBox* CurUI = Cast<UUI_GameInformationBox>(TargetObject))
	{
		//初始化数据表
		UTaskSubsystem::InitTaskData(
			FSoftObjectPath(TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_情报岛任务.DT_情报岛任务'")),
			UTaskSubsystem::GetTaskSubsystemStatic()->LookInforTaskDataTable
		);

		bool bRe;
		//拿到任务数据
		const FLookInforTaskDataTable& Cur = UTaskSubsystem::FindTaskData(TaskName,
			UTaskSubsystem::GetTaskSubsystemStatic()->LookInforTaskDataTable, bRe
		);

		if (!bRe)
		{
			return false;
		}

		for (const auto& CurName : Cur.CurLook)
		{
			//查询历史记录
			if (!UPlayerRecord::GetDaySeeInfor(CurName))
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

UTaskFunction* ULookInforTask::MakeNewClass()
{
	return NewObject<ULookInforTask>();
}
