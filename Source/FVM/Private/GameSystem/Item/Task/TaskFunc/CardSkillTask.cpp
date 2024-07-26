// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Task/TaskFunc/CardSkillTask.h"
#include "GameSystem/TaskSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "GameSystem/Tools/GameSystemFunction.h"

bool UCardSkillTask::ExeTask(const FString& TaskName, UObject* TargetObject)
{
	if (UUI_PlayerBagMaterialGrid* Grid = Cast<UUI_PlayerBagMaterialGrid>(TargetObject))
	{
		//初始化数据表
		UTaskSubsystem::InitTaskData(
			FSoftObjectPath(TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_卡片技能书任务.DT_卡片技能书任务'")),
			UTaskSubsystem::GetTaskSubsystemStatic()->CardSkillTaskDataTable
		);

		bool bRe;
		//拿到任务数据
		const FCardSkillTaskDataTable& Cur = UTaskSubsystem::FindTaskData(TaskName,
			UTaskSubsystem::GetTaskSubsystemStatic()->CardSkillTaskDataTable, bRe
		);

		if (!bRe)
		{
			return false;
		}

		if (Cur.CardSkillName.Equals(TEXT("")))
		{
			return true;
		}
		else {
			return UPlayerRecord::GetDayCardSkillBook(Cur.CardSkillName);
		}

	}
	return false;
}

UTaskFunction* UCardSkillTask::MakeNewClass()
{
	return NewObject<UCardSkillTask>();
}
