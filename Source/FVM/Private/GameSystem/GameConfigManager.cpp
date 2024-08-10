// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/GameConfigManager.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

void UGameConfigManager::SetCurrentLoginPlayerName(FString CurPlayerAccountName)
{
	this->CurrentSelectPlayerAccountName = CurPlayerAccountName;
}

void UGameConfigManager::LoginFaild()
{
	this->CurrentSelectPlayerAccountName = TEXT("");
}

FString UGameConfigManager::GetCurrentLoginPlayerName()
{
	return this->CurrentSelectPlayerAccountName;
}

void UGameConfigManager::GetLoginPlayerNameList(TMap<FString, FPlayerLoginBaseData>& OutData)
{
	OutData = this->M_LoginPlayerNames;
}

void UGameConfigManager::GetPlayerLoginCache(bool& Result, FString& PlayerAccount, FString& PlayerPassword)
{
	FPlayerLoginBaseData* Data = this->M_LoginPlayerNames.Find(this->LastLoginPlayerAccountName);

	for (auto& Cache : this->M_LoginPlayerNames)
	{
		UE_LOG(LogTemp,Error,TEXT("[%s] [%s]"),*Cache.Value.PlayerAccount,*Cache.Value.PlayerPassword);
	}

	UE_LOG(LogTemp,Error,TEXT("当前账户[%s]"),*this->LastLoginPlayerAccountName);

	if (Data)
	{
		if (Data->PlayerAccount.IsEmpty())
		{
			Result = false;
			return;
		}

		if (Data->PlayerPassword.IsEmpty())
		{
			Result = false;
			return;
		}

		Result = true;
		PlayerAccount = Data->PlayerAccount;
		PlayerPassword = Data->PlayerPassword;

		return;
	}

	Result = false;
}

void UGameConfigManager::AddPlayerLoginName(FString NewPlayerName, FPlayerLoginBaseData PlayerLoginData)
{
	//清理之前的缓存
	for (auto& Cache : this->M_LoginPlayerNames)
	{
		Cache.Value.PlayerAccount = TEXT("");
		Cache.Value.PlayerPassword = TEXT("");
	}

	this->LastLoginPlayerAccountName = NewPlayerName;
	this->M_LoginPlayerNames.Emplace(this->LastLoginPlayerAccountName, PlayerLoginData);	
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
