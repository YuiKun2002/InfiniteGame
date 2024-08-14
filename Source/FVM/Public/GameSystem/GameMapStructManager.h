// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/MouseStructManager.h"
#include "GameMapStructManager.generated.h"

//网格列
USTRUCT(BlueprintType)
struct FMesheColBg
{
	GENERATED_USTRUCT_BODY()
public:
	//是否显示背景
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableBG = false;
	//背景需要的路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_BG_FullPath;
	//偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_Offset = FVector(0.F);
};

//网格列预放置卡片
USTRUCT(BlueprintType)
struct FMesheColPrePlayCard
{
	GENERATED_USTRUCT_BODY()
public:
	//卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CardName;
	//卡片优先级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CardLayer = 0;
	//卡片对应的蓝图实例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftClassPath CardActorResource;
};

//网格列
USTRUCT(BlueprintType)
struct FMesheCol
{
	GENERATED_USTRUCT_BODY()
public:
	//网格列预放置卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMesheColPrePlayCard> MesheColPrePlayCard;
	//当前网格类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_ELineType = ELineType::OnGround;
	//移动状态是否开启
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bMoveActive = false;
	//移动所需时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MoveingTime = 2.f;
	//移动完成之后再一次移动的冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MoveingCloudDownTime = 2.f;
	//移动网格偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLine M_MoveOffset = FLine(0, 0);
	//设置网格背景
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMesheColBg M_MeshBG;
	//内列间距
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_PaddingCol = 60.f;
};


//网格行
USTRUCT(BlueprintType)
struct FMesheRow
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMesheCol> M_Col;
	//内行间距
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_PaddingRow = 64.f;
};


//游戏地图结构
USTRUCT(BlueprintType)
struct FGameMapStruct
{
	GENERATED_USTRUCT_BODY()
public:
	//网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMesheRow> M_Meshe;
	//每一行的动物
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSoftClassPtr<class AVSAnimalBase>> RowAnimalClass;
	//第一行网格的坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D M_FirstRowMesheLocation = FVector2D(-140.8f, 269.f);
};

//地图结构管理器
UCLASS()
class FVM_API UGameMapStructManager : public USaveGame
{
	GENERATED_BODY()
public:
	//地图配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameMapStruct GameMapStruct;
	//关卡配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLevelConfig LevelConfig;
};