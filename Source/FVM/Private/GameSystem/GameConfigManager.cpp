// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/GameConfigManager.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

void UGameConfigManager::SetCurrentLoginPlayerName(FString CurPlayerAccountName)
{
	this->CurrentSelectPlayerAccountName = CurPlayerAccountName;
}

FString UGameConfigManager::GetCurrentLoginPlayerName()
{
	return this->CurrentSelectPlayerAccountName;
}

void UGameConfigManager::GetLoginPlayerNameList(TMap<FString, FPlayerLoginBaseData>& OutData)
{
	OutData = this->M_LoginPlayerNames;
}

void UGameConfigManager::AddPlayerLoginName(FString NewPlayerName, FPlayerLoginBaseData PlayerLoginData)
{
	this->M_LoginPlayerNames.Emplace(NewPlayerName, PlayerLoginData);
}

void UGameConfigManager::SetCurrentLonginAccount(const FString& NewPlayerName)
{
	this->CurrentSelectPlayerAccountName = NewPlayerName;
}

bool UGameConfigManager::RemovePlayerLoginName(FString NewPlayerName)
{
	if (this->M_LoginPlayerNames.Remove(NewPlayerName) != -1)
	{
		return true;
	}

	return false;
}

bool UGameConfigManager::CheckPlayerLoginNameIsValid(FString NewPlayerName)
{
	FPlayerLoginBaseData* LocalFindData = this->M_LoginPlayerNames.Find(NewPlayerName);
	if (LocalFindData)
	{
		return true;
	}

	return false;
}

FPlayerLoginBaseData UGameConfigManager::GetPlayerLoginCacheData()
{
	FPlayerLoginBaseData* Data = this->M_LoginPlayerNames.Find(this->CurrentSelectPlayerAccountName);
	if (Data)
	{
		return *Data;
	}

	return FPlayerLoginBaseData();
}
