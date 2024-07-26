// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/PlayerDataSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include <Kismet/GameplayStatics.h>

bool IPlayerDataInterface::SavePlayerData_Implementation(const FString& SaveLogMsg) { return false; }

bool IPlayerDataInterface::LoadPlayerData_Implementation(const FString& LoadLogMsg) { return false; }

bool IPlayerDataInterface::CreateNewPlayerData_Implementation() { return false; }

bool IPlayerDataInterface::RemovePlayerData_Implementation() { return false; }

UPlayerStructManager* IPlayerDataInterface::GetPlayerData_Implementation() { return nullptr; }

ULocalPlayerDataHandle* ULocalPlayerDataHandle::MakeLocalPlayerDataHandle(FString PlayerName, int32 PlayerSex)
{
	ULocalPlayerDataHandle* NewHandle = NewObject<ULocalPlayerDataHandle>();
	NewHandle->LocalPlayerName = PlayerName;
	NewHandle->LocalPlayerSex = PlayerSex;
	return NewHandle;
}

bool ULocalPlayerDataHandle::SavePlayerData_Implementation(const FString& SaveLogMsg)
{
	this->SaveLocalSaveGame(this->LocalData, LocalPlayerName + TEXT("P"), SaveLogMsg);

	return true;
}


bool ULocalPlayerDataHandle::LoadPlayerData_Implementation(const FString& LoadLogMsg)
{
	this->LocalData = Cast<UPlayerStructManager>(
		this->LoadLocalSaveGame(LocalPlayerName + TEXT("P"), LoadLogMsg)
		);

	if (IsValid(this->LocalData))
	{
		return true;
	}

	return false;
}

bool ULocalPlayerDataHandle::CreateNewPlayerData_Implementation()
{
	//输入的文字是空的
	if (LocalPlayerName.IsEmpty())
	{
		UWidgetBase::CreateTipWidget(TEXT("名称不能为空"), FVector(0.f, 1.f, 1.f));
		return false;
	}

	if (LocalPlayerName.Len() > 8)
	{
		UWidgetBase::CreateTipWidget(TEXT("名字长度不能超过8位"), FVector(0.f, 1.f, 1.f));
		return false;
	}

	//获取游戏数据
	if (!UFVMGameInstance::GetFVMGameInstance())
	{
		return false;
	}

	//查询本地存档是否存在
	FString LocalPlayerFileName = LocalPlayerName + FString("P");
	if (this->LocalFileIsExist(LocalPlayerFileName))
	{
		UPlayerStructManager* PlayerInstance = Cast<UPlayerStructManager>(
			this->LoadLocalSaveGame(LocalPlayerFileName, __FUNCTION__ + FString("加载角色存档"))
			);

		if (IsValid(PlayerInstance))
		{
			this->LocalData = PlayerInstance;

			//设置角色数据实例
			UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetPlayerDataInstance(PlayerInstance);
			//设置本地角色句柄
			UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetLocalPlayerDataHandle(this);

			//加载成功
			return true;
		}
	}
	else {
		//创建新的角色存档
		UPlayerStructManager* NewPlayerIns = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->CreateEmptyPlayerInstance();
		NewPlayerIns->M_PlayerName = LocalPlayerName;
		NewPlayerIns->M_PlayerSex = LocalPlayerSex;
		NewPlayerIns->M_GameVersion = UFVMGameInstance::GetGameVersion();
		//更新准备ID数量
		NewPlayerIns->UpdateEquipID(150);
		//设置本地游戏实例对象
		this->LocalData = NewPlayerIns;

		//设置角色数据实例
		UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetPlayerDataInstance(NewPlayerIns);
		//设置本地角色句柄
		UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetLocalPlayerDataHandle(this);

		//发送数据
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("小笼包")), 0);
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("小火炉")), 0);
		UGameSystemFunction::SendCardToPlayerBag(FString(TEXT("土司面包")), 0);

		//保存存档
		UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SavePlayerData(this, __FUNCTION__ + FString("保存角色存档"));

		return true;
	}

	return false;
}

bool ULocalPlayerDataHandle::RemovePlayerData_Implementation()
{
	return UGameplayStatics::DeleteGameInSlot(LocalPlayerName + FString("P"), 0);
}

UPlayerStructManager* ULocalPlayerDataHandle::GetPlayerData_Implementation()
{
	return this->LocalData;
}

bool ULocalPlayerDataHandle::LocalFileIsExist(FString FileName)
{
	return UGameplayStatics::DoesSaveGameExist(FileName, 0);
}

USaveGame* ULocalPlayerDataHandle::LoadLocalSaveGame(const FString& FileName, const FString& LogMsg)
{
	USaveGame* SaveIns = UGameplayStatics::LoadGameFromSlot(FileName, 0);

	if (IsValid(SaveIns))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(FileName + TEXT("[加载]{")) + LogMsg + TEXT("}"));
		}
	}

	return SaveIns;
}

void ULocalPlayerDataHandle::SaveLocalSaveGame(USaveGame* SaveGameObject, const FString& SlotName, const FString& LogMsg)
{
	if (UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(SlotName + TEXT("[存储]{")) + LogMsg + TEXT("}"));
		}
	}
}


UPlayerDataSubsystem* UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetSubsystem<UPlayerDataSubsystem>();
	}

	return nullptr;
}

UPlayerStructManager* UPlayerDataSubsystem::CreateEmptyPlayerInstance()
{
	return Cast<UPlayerStructManager>(
		UGameplayStatics::CreateSaveGameObject(
			LoadClass<UPlayerStructManager>(0, TEXT("Class'/Script/FVM.PlayerStructManager'"))));
}

UPlayerStructManager* UPlayerDataSubsystem::GetPlayerDataInstance()
{
	return this->CurrentPlayerDataInstance;
}

void UPlayerDataSubsystem::SetPlayerDataInstance(UPlayerStructManager* NewPlayerIns)
{
	this->CurrentPlayerDataInstance = NewPlayerIns;
}

void UPlayerDataSubsystem::SetLocalPlayerDataHandle(ULocalPlayerDataHandle* NewPlayerHandle)
{
	this->CurrentLocalPlayerDataHandle = NewPlayerHandle;
}

ULocalPlayerDataHandle* UPlayerDataSubsystem::GetLocalPlayerDataHandle()
{
	return this->CurrentLocalPlayerDataHandle;
}

bool UPlayerDataSubsystem::SavePlayerData(UObject* Inter, const FString& SaveLogMsg)
{
	if (Cast<IPlayerDataInterface>(Inter))
	{
		return Cast<IPlayerDataInterface>(Inter)->Execute_SavePlayerData(Inter, SaveLogMsg);
	}

	return false;
}

bool UPlayerDataSubsystem::LoadPlayerData(UObject* Inter, const FString& LoadLogMsg)
{
	if (Cast<IPlayerDataInterface>(Inter))
	{
		return Cast<IPlayerDataInterface>(Inter)->Execute_LoadPlayerData(Inter, LoadLogMsg);
	}

	return false;
}

bool UPlayerDataSubsystem::CreateNewLocalPlayerData(ULocalPlayerDataHandle* LocalPlayerHandle)
{
	if (Cast<IPlayerDataInterface>(LocalPlayerHandle))
	{
		if (Cast<IPlayerDataInterface>(LocalPlayerHandle)->Execute_CreateNewPlayerData(LocalPlayerHandle))
		{
			//构造本地角色存档
			this->CurrentLocalPlayerDataHandle = LocalPlayerHandle;

			return true;
		}
	}
	return false;
}

ULocalPlayerDataHandle* UPlayerDataSubsystem::LoadLocalPlayerData(FString LocalPlayerName, const FString& LoadLogMsg)
{
	ULocalPlayerDataHandle* NewHandle = ULocalPlayerDataHandle::MakeLocalPlayerDataHandle(LocalPlayerName, 0);

	if (UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->LoadPlayerData(NewHandle, TEXT("本地加载UPlayerDataSubsystem::LoadLocalPlayerData：") + LoadLogMsg))
	{
		return NewHandle;
	}

	return nullptr;
}

bool UPlayerDataSubsystem::SaveLocalPlayerData(ULocalPlayerDataHandle* LocalPlayerHandle, const FString& SaveLogMsg)
{
	if (IsValid(LocalPlayerHandle))
	{
		return UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SavePlayerData(LocalPlayerHandle, TEXT("本地保存UPlayerDataSubsystem::SaveLocalPlayerData：") + SaveLogMsg);
	}

	return false;
}

bool UPlayerDataSubsystem::RemoveLocalPlayerData(ULocalPlayerDataHandle* LocalPlayerHandle)
{
	if (Cast<IPlayerDataInterface>(LocalPlayerHandle))
	{
		return Cast<IPlayerDataInterface>(LocalPlayerHandle)->Execute_RemovePlayerData(LocalPlayerHandle);
	}

	return false;
}

void UPlayerDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPlayerDataSubsystem::Deinitialize()
{
	Super::Deinitialize();

}
