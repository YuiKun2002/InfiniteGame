﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "UObject/Interface.h"
#include "Engine/StreamableManager.h"
#include "GameSystem/DataTableAssetData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameAssetSubsystem.generated.h"

class UGameAssetSubsystem;
/**
 * 游戏资产子系统
 */
namespace NGASub {
	UGameAssetSubsystem* Get();
}

/**
 * 游戏资产
 */
UCLASS(BlueprintType, Blueprintable)
class UGameAssetDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	//游戏数据表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏数据")
	TSoftObjectPtr<UDataAsset> GameData_DataTable;
};

/**
 * 游戏资产子系统
 */
UCLASS()
class FVM_API UGameAssetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//获取主要的游戏资产
	UFUNCTION(BlueprintPure, Category = "主要资产")
	UGameAssetDataAsset* GetMainGameAsset();
	//获取加载管理器
	FStreamableManager& GetStreamableManager();
public:
	//获取子系统
	UFUNCTION(BlueprintPure, Category = "游戏资产子系统")
	static UGameAssetSubsystem* GetGameAssetSubsystemStatic();
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
private:
	UPROPERTY()
	UGameAssetDataAsset* MainAssetIns = nullptr;
};
