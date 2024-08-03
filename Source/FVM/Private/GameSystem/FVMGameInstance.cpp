// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/FVMGameInstance.h" 

#include "Data/GameLogSubsystem.h"
#include "Data/MapData/MapDataStruct.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "GameSystem/GameConfigSubsystem.h"
#include "GameSystem/PlayerDataSubsystem.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameFramework/GameUserSettings.h"

FString UFVMGameInstance::GetGameVersion()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion;
	}

	return TEXT("");
}

void UFVMGameInstance::SetUpdateGame()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate = true;
	}
}

void UFVMGameInstance::SetGameDebug(bool _Value)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UFVMGameInstance::GetFVMGameInstance()->M_bShowDebugText = _Value;
	}
}

bool UFVMGameInstance::GetDebug()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->M_bShowDebugText;
	}
	return false;
}

void UFVMGameInstance::SetGameVersion(FString _Version)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion = _Version;
	}
}

bool UFVMGameInstance::GetUpdateGame()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate;
	}

	return false;
}

UGameMapStructManager* UFVMGameInstance::LoadGameMapStructManager(const FString& _FileName)
{
	UDataTable* GameMapData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/MapData/GameMapData.GameMapData'"));
	TArray<FGameMapData*> M_GameMapData;
	GameMapData->GetAllRows("GameMapList", M_GameMapData);

	for (auto Datas : M_GameMapData)
	{
		//从数据表寻找指定的地图名称
		if ((*Datas).M_FLevelConfig.LevelName.Equals(_FileName))
		{
			//创建新的地图结构并且将数据表的数据赋予给新创建的地图结构
			this->M_UGameMapStructManager = Cast<UGameMapStructManager>(
				UGameplayStatics::CreateSaveGameObject(
					LoadClass<UGameMapStructManager>(0, TEXT("Class'/Script/FVM.GameMapStructManager'"))
				));
			this->M_UGameMapStructManager->GameMapStruct = (*Datas).M_FGameMapStruct;
			this->M_UGameMapStructManager->LevelConfig = (*Datas).M_FLevelConfig;


			//创建新的老鼠结构并且把数据表的数据赋予给新的老鼠结构
			this->M_MouseStructManager = Cast<UMouseStructManager>(
				UGameplayStatics::CreateSaveGameObject(
					LoadClass<UMouseStructManager>(0, TEXT("Class'/Script/FVM.MouseStructManager'"))
				));
			//设置老鼠配置
			this->M_MouseStructManager->SetMouseConfig((*Datas).M_FMouseConfig);
			//设置行数量
			this->M_MouseStructManager->SetRowCount(this->M_UGameMapStructManager->GameMapStruct.M_Meshe.Num());

			//返回数据
			return this->M_UGameMapStructManager;
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Log, TEXT("地图配置管理器加载失败!"));
	}

	return nullptr;
}

UGameConfigManager* UFVMGameInstance::GetGameConfigManager()
{
	if (UGameConfigSubsystem::GetGameConfigSubsystemStatic())
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();
	}

	return nullptr;
}

UGameMapStructManager* UFVMGameInstance::GetGameMapStructManager()
{
	return this->M_UGameMapStructManager;
}

UPlayerStructManager* UFVMGameInstance::GetPlayerStructManager()
{
	if (IsValid(UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()))
	{
		return UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance();
	}

	return nullptr;
}

bool UFVMGameInstance::GetGameConfig_GameEFEnabled()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameEF.M_bEnable;
	}

	return false;
}

void UFVMGameInstance::UpdateBGMVolume()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->UpdateGameBGMVolume();
	}
}

void UFVMGameInstance::SetBGMScale(float Value)
{
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->SetGameBGMVolume(Value);
	}
}

UMouseStructManager* UFVMGameInstance::GetMouseStructManager()
{
	return this->M_MouseStructManager;
}

UPlayerStructManager* UFVMGameInstance::GetPlayerStructManager_Static()
{
	if (IsValid(UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()))
	{
		return UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance();
	}
	return nullptr;
}

UGameConfigManager* UFVMGameInstance::GetGameConfigManager_Static()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();
	}

	return nullptr;
}

UFVMGameInstance* UFVMGameInstance::GetFVMGameInstance()
{
	if (IsValid(GEngine))
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		if (context)
		{
			return Cast<UFVMGameInstance>(context->OwningGameInstance);
		}
	}

	return nullptr;
}

void UFVMGameInstance::PlayBGM_Static(const FSoftObjectPath& _MusicName, bool _Loop)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->PlayGameBGM(_MusicName, _Loop);
	}
}

FSoftObjectPath UFVMGameInstance::GetPlayBGMName()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetCurrentPlayGameBgmPath();
	}

	return FSoftObjectPath();
}

void UFVMGameInstance::PlayBGM_S_Static(const FString& _MusicName, const FString& _MusicPath)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->PlayGameSoundEffects(_MusicName, _MusicPath);
	}
}

float UFVMGameInstance::GetBGMS_Volume()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameSoundEffectsVolume();
	}

	return 0.f;
}