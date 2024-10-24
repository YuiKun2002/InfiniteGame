﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/GameAssetSubsystem.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"
#include "Engine/AssetManager.h"
#include "GameSystem/FVMGameInstance.h"

UGameAssetSubsystem* NGASub::Get()
{
	return UGameAssetSubsystem::GetGameAssetSubsystemStatic();
}

UGameAssetDataAsset* UGameAssetSubsystem::GetMainGameAsset()
{
	return this->MainAssetIns;
}

FStreamableManager& UGameAssetSubsystem::GetStreamableManager()
{
	return UAssetManager::GetStreamableManager();
}

UGameAssetSubsystem* UGameAssetSubsystem::GetGameAssetSubsystemStatic()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetSubsystem<UGameAssetSubsystem>();
	}

	return nullptr;
}

void UGameAssetSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	this->MainAssetIns = LoadObject<UGameAssetDataAsset>(
		nullptr,
		TEXT("GameAssetDataAsset'/Game/Resource/BP/Data/DA_GameMainData.DA_GameMainData'")
	);
}

void UGameAssetSubsystem::Deinitialize()
{
	Super::Deinitialize();

	this->MainAssetIns = nullptr;
}
