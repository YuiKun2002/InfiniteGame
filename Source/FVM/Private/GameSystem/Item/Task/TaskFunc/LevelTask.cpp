// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Task/TaskFunc/LevelTask.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameStart/VS/UI/UI_GameOver.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/Item/PlayerRecord.h"
#include "GameSystem/PlayerStructManager.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"

#include "GameSystem/TaskSubsystem.h"

bool ULevelTask::ExeTask(const FString& TaskName, UObject* TargetObject)
{
	if (UUI_GameOver* CurGame = Cast<UUI_GameOver>(TargetObject))
	{
		//初始化数据表
		UTaskSubsystem::InitTaskData(
			FSoftObjectPath(TEXT("DataTable'/Game/Resource/BP/Data/Task/Main/DT_关卡通关任务.DT_关卡通关任务'")),
			UTaskSubsystem::GetTaskSubsystemStatic()->LevelTaskDataTable
		);

		bool bRe;
		const FLevelTaskDataTable& CurData = UTaskSubsystem::FindTaskData(TaskName,
			UTaskSubsystem::GetTaskSubsystemStatic()->LevelTaskDataTable, bRe);

		if (!bRe)
		{
			return false;
		}

		auto Mouses = [&]()->bool {
			if (CurData.KillMouses.Num())
			{
				//老鼠
				TMap<FString, int32> CurMouses;
				if (CurData.Total)
				{
					UPlayerRecord::GetDayKillMouseCount(CurMouses);
				}
				else {
					CurMouses = AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->KillMouses;
				}

				for (const auto& Cur : CurData.KillMouses)
				{
					int32* CurCount = CurMouses.Find(Cur.Key);
					if (CurCount)
					{
						if (Cur.Value > *CurCount)
						{

							return false;
						}
					}
					else {
						return false;
					}
				}
			}
			return true;
		};

		auto Cards = [&]()->bool {
			if (CurData.PlayCards.Num())
			{
				//卡片
				TMap<FString, int32> CurCards;

				if (CurData.Total)
				{
					UPlayerRecord::GetDayCardCount(CurCards);
				}
				else {
					CurCards = AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->PlayCards;
				}

				for (const auto& Cur : CurData.PlayCards)
				{
					int32* CurCount = CurCards.Find(Cur.Key);
					if (CurCount)
					{
						if (Cur.Value > *CurCount)
						{
							return false;
						}
					}
					else {
						return false;
					}
				}
			}

			return true;
		};

		//材料收集
		auto Materials = [&]()->bool {
			if (CurData.MaterialName.Num())
			{
				for (const FString& CurName : CurData.MaterialName)
				{
					if (!CurGame->FindMaterial(CurName))
					{
						return false;
					}
				}

				return true;
			}

			return true;
		};

		//关卡胜利
		if (Cast<UUI_GameOver>(TargetObject)->bLevelWin)
		{
			if (CurData.WinCount == 0)
			{
				const FLevelConfig& LevelConfig =
					UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;

				//关卡胜利，指定关卡，指定卡片和老鼠【指定通关关卡】
				if (CurData.MapName.Equals(LevelConfig.LevelName))
				{
					if (Mouses() && Cards() && Materials())
					{
						return true;
					}

					return false;
				}
				else {
					//关卡胜利【未指定关卡】老鼠消灭任务，卡片放置任务
					if (CurData.MapName.Equals(""))
					{
						if (Mouses() && Cards() && Materials())
						{
							return true;
						}

						return false;
					}
				}
			}
			else {
				//查询累计记录
				if (CurData.WinCount >= UPlayerRecord::GetPlayer()->PlayerRecord.DayLevelWinCount)
				{
					return true;
				}
			}
		}
		else {
			//失败
			//关卡失败【未指定关卡】 老鼠消灭任务，卡片放置任务
			if (CurData.MapName.Equals(""))
			{
				if (Mouses() && Cards() && Materials())
				{
					return true;
				}

				return false;
			}
		}
	}

	return false;
}

UTaskFunction* ULevelTask::MakeNewClass()
{
	return NewObject<ULevelTask>();
}
