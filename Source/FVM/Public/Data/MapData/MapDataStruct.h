// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/GameMapStructManager.h"
#include "MapDataStruct.generated.h"

/**
 * 老鼠结构  地图结构
 */

 //地图数据结构
USTRUCT(BlueprintType)
struct FGameMapData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	//地图结构
	UPROPERTY(EditDefaultsOnly)
		FGameMapStruct M_FGameMapStruct;

	//关卡配置
	UPROPERTY(EditDefaultsOnly)
		FLevelConfig M_FLevelConfig;

	//老鼠配置
	UPROPERTY(EditDefaultsOnly)
		FMouseConfig M_FMouseConfig;
};

UCLASS()
class FVM_API UMapDataStruct : public UObject
{
	GENERATED_BODY()
};
