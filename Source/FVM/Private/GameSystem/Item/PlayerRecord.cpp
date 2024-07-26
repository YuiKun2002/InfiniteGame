// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/PlayerRecord.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

UPlayerStructManager* UPlayerRecord::GetPlayer()
{
	return UFVMGameInstance::GetPlayerStructManager_Static();
}

void UPlayerRecord::InitDayRecord()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.DayLevelWinCount = 0;
		Player->PlayerRecord.DayCardUpCount.Empty();
		Player->PlayerRecord.DayCardMakeCount.Empty();
		Player->PlayerRecord.DaySeeInforCount.Empty();
		Player->PlayerRecord.DayUseCardSkillBooks.Empty();
		Player->PlayerRecord.DayCardCount.Empty();
		Player->PlayerRecord.DayKillMouseCount.Empty();
	}
}

int32 UPlayerRecord::AddDayLevelWinCount()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.DayLevelWinCount = Player->PlayerRecord.DayLevelWinCount + 1;
		return Player->PlayerRecord.DayLevelWinCount;
	}
	return 0;
}

void UPlayerRecord::AddDayCardCount(const TMap<FString, int32>& InputCards)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		for (const auto& Cur : InputCards)
		{
			int32* Count = Player->PlayerRecord.DayCardCount.Find(Cur.Key);
			if (Count)
			{
				*Count = *Count + Cur.Value;
			}
			else {
				Player->PlayerRecord.DayCardCount.Emplace(Cur.Key, Cur.Value);
			}
		}
	}
}

void UPlayerRecord::GetDayCardCount(TMap<FString, int32>& OutCards)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		OutCards.Append(Player->PlayerRecord.DayCardCount);
	}
}

void UPlayerRecord::AddDayKillMouseCount(const TMap<FString, int32>& InputMouses)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		for (const auto& Cur : InputMouses)
		{
			int32* Count = Player->PlayerRecord.DayKillMouseCount.Find(Cur.Key);
			if (Count)
			{
				*Count = *Count + Cur.Value;
			}
			else {
				Player->PlayerRecord.DayKillMouseCount.Emplace(Cur.Key, Cur.Value);
			}
		}
	}
}

void UPlayerRecord::GetDayKillMouseCount(TMap<FString, int32>& OutMouses)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		OutMouses.Append(Player->PlayerRecord.DayKillMouseCount);
	}
}

void UPlayerRecord::AddDayCardSkillBook(FString CardSkillName)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.DayUseCardSkillBooks.Emplace(CardSkillName);
	}
}

bool UPlayerRecord::GetDayCardSkillBook(FString CardSkillName)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		return Player->PlayerRecord.DayUseCardSkillBooks.Contains(CardSkillName);
	}
	return false;
}

void UPlayerRecord::AddDayCardUpCount(int32 CurUpGrade, FString UpCardName)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.DayCardUpCount.Emplace(CurUpGrade, UpCardName);
	}
}

FString UPlayerRecord::GetDayCardUpCount(int32 CurUpGrade)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		FString* CurCardName = Player->PlayerRecord.DayCardUpCount.Find(CurUpGrade);
		if (CurCardName)
		{
			return *CurCardName;
		}
	}

	return FString();
}

void UPlayerRecord::AddDayCardMakeCount(FString CardName)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		int32* CurCount = Player->PlayerRecord.DayCardMakeCount.Find(CardName);
		if (CurCount)
		{
			*CurCount = *CurCount + 1;
		}
		else {
			Player->PlayerRecord.DayCardMakeCount.Emplace(CardName, 1);
		}
	}
}

int32 UPlayerRecord::GetDayCardMakeCount(FString CardName)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		if (CardName.Equals(TEXT("")))
		{
			int32 TempCount = 0;

			for (const auto& Cur : Player->PlayerRecord.DayCardMakeCount)
			{
				TempCount += Cur.Value;
			}

			return TempCount;
		}

		int32* CurCount = Player->PlayerRecord.DayCardMakeCount.Find(CardName);
		if (CurCount)
		{
			return *CurCount;
		}
	}
	return 0;
}

void UPlayerRecord::AddDaySeeInforCount(FString InforName)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.DaySeeInforCount.Emplace(InforName);
	}
}

bool UPlayerRecord::GetDaySeeInfor(FString InforName)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		return Player->PlayerRecord.DaySeeInforCount.Contains(InforName);
	}
	return false;
}

void UPlayerRecord::AddDayDayNewGemAttachName(FString Name)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.DayNewGemAttachName = Name;
	}
}

FString UPlayerRecord::GetDayDayNewGemAttachName()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		return Player->PlayerRecord.DayNewGemAttachName;
	}

	return FString();
}

int32 UPlayerRecord::AddLevelWinCount()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.LevelWinCount = Player->PlayerRecord.LevelWinCount + 1;
		return Player->PlayerRecord.LevelWinCount;
	}
	return 0;
}

void UPlayerRecord::AddCardUpGradeCount(int32 Grade)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		int32* Count = Player->PlayerRecord.CardUpCount.Find(Grade);
		if (Count)
		{
			*Count = *Count + 1;
		}
		else {
			Player->PlayerRecord.CardUpCount.Emplace(Grade, 1);
		}
	}
}

void UPlayerRecord::AddGemAttachCount()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.GemSetWeaponCount += 1;
	}
}

void UPlayerRecord::AddGemUpGrade(FString GemName, int32 GemGrade)
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		int32* Grade = Player->PlayerRecord.GemsGradeCount.Find(GemName);
		if (Grade)
		{
			*Grade = (*Grade <= GemGrade ? GemGrade : *Grade);
		}
		else {
			Player->PlayerRecord.GemsGradeCount.Emplace(GemName, GemGrade);
		}
	}
}

void UPlayerRecord::AddGemSplitCount()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		Player->PlayerRecord.GemsSplitCount += 1;
	}
}
