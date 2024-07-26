// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Task/TaskFunc/SynthesisTask.h"
#include "GameSystem/TaskSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "Game/UI/Synthesis/SynModel_MakeCard.h"
#include "Game/UI/Synthesis/SynModel_CardUpgrade.h"
#include "GameSystem/Tools/GameSystemFunction.h"

bool USynthesisTask::ExeTask(const FString& TaskName, UObject* TargetObject)
{
	//卡片制作
	if (USynModel_MakeCard* MakeCard = Cast<USynModel_MakeCard>(TargetObject))
	{
		this->InitData();

		bool bRe;

		//拿到任务数据
		const FSynMakeCardTaskDataTable& Cur = UTaskSubsystem::FindTaskData(TaskName,
			UTaskSubsystem::GetTaskSubsystemStatic()->SynMakeCardTaskDataTable, bRe
		);

		if (bRe)
		{
			//查询历史记录[卡片制作次数]
			int32 MakeCardCount = UPlayerRecord::GetDayCardMakeCount(Cur.MakeCardName);
			if (MakeCardCount < Cur.MakeCount)
			{
				return false;
			}

			//如果是任意卡片
			if (Cur.MakeCardName.Equals(TEXT("")))
			{
				return true;
			}
			else {
				if (Cur.MakeCardName.Equals(MakeCard->GetCurrentCardName()))
				{
					return true;
				}
				return false;
			}
		}
	}

	if (USynModel_CardUpgrade* CardUp = Cast<USynModel_CardUpgrade>(TargetObject))
	{
		this->InitData2();

		bool bRe;

		//拿到任务数据
		const FSynCardUpGradeDataTable& Cur = UTaskSubsystem::FindTaskData(TaskName,
			UTaskSubsystem::GetTaskSubsystemStatic()->SynCardUpGradeDataTable, bRe
		);

		if (bRe)
		{
			//任意卡片等级强化
			if (Cur.UpGradeCardName.Equals(TEXT("")))
			{
				if (Cur.CardUpGrade == 0)
				{
					return true;
				}
				else {
					//获取当前对应强化等级的卡片
					const FString& CurCardName = UPlayerRecord::GetDayCardUpCount(Cur.CardUpGrade);
					//如果匹配强化卡片名称则完成任务
					if (CurCardName.Equals(CardUp->GetCurrentCardName()))
					{
						return true;
					}
				}
				return false;
			}
			else {
				//获取当前对应强化等级的卡片
				const FString& CurCardName = UPlayerRecord::GetDayCardUpCount(Cur.CardUpGrade);
				if (CurCardName.Equals(Cur.UpGradeCardName))
				{
					return true;
				}
				return false;
			}
		}
	}

	return false;
}

UTaskFunction* USynthesisTask::MakeNewClass()
{
	return NewObject<USynthesisTask>();
}

void USynthesisTask::InitData()
{
	//初始化数据表
	UTaskSubsystem::InitTaskData(
		FSoftObjectPath(TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_卡片制作任务.DT_卡片制作任务'")),
		UTaskSubsystem::GetTaskSubsystemStatic()->SynMakeCardTaskDataTable
	);
}

void USynthesisTask::InitData2()
{
	//初始化数据表
	UTaskSubsystem::InitTaskData(
		FSoftObjectPath(TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_卡片强化任务.DT_卡片强化任务'")),
		UTaskSubsystem::GetTaskSubsystemStatic()->SynCardUpGradeDataTable
	);
}
