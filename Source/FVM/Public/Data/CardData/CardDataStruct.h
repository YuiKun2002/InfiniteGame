// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "CardDataStruct.generated.h"


//卡片关键字映射默认动画
USTRUCT(BlueprintType)
struct FCard_KeyIdleAnim_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//卡片动画映射存储
	UPROPERTY(EditDefaultsOnly)
	FCardKeyIdleAnimation M_CardKeyIdleAnim;
};

//卡片关键字映射默认动画
USTRUCT(BlueprintType)
struct FCard_FCardpreViewHead_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//卡片预览图存储
	UPROPERTY(EditDefaultsOnly)
	FCardpreViewHead M_CardpreViewHead;
};

//卡片等级动画
USTRUCT(BlueprintType)
struct FCard_GradeAnim_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//卡片动画映射存储
	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath CardGradeResource;
	//基础偏移
	UPROPERTY(EditDefaultsOnly)
	FVector2D RelativeLocation;
	//缩放
	UPROPERTY(EditDefaultsOnly)
	FVector2D Scale;
};

//攻击类型卡片结构
USTRUCT(BlueprintType)
struct FCard_ATK_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
	FItemCardATK M_FItemCard;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FItemCard;
		return true;
	}
};

//范围攻击类型卡片结构
USTRUCT(BlueprintType)
struct FCard_RangeATK_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//范围攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
	FItemCardRangeATK M_FItemCard;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FItemCard;
		return true;
	}
};

//生产类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Spawn_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//生产类型的卡片
	UPROPERTY(EditDefaultsOnly)
	FItemCardSpawn M_FItemCard;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FItemCard;
		return true;
	}
};

//防御类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Defence_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//防御类型的卡片
	UPROPERTY(EditDefaultsOnly)
	FItemCardDefence M_FItemCard;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FItemCard;
		return true;
	}
};

//辅助类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Aid_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//辅助类型的卡片
	UPROPERTY(EditDefaultsOnly)
	FItemCardAid M_FItemCard;
};

//功能类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Function_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//功能类型的卡片
	UPROPERTY(EditDefaultsOnly)
	FItemCardFunction M_FItemCard;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FItemCard;
		return true;
	}
};

//卡片数据资产缓存
UCLASS()
class FVM_API UCardDataAssetCache : public UGameDataAssetCache
{
	GENERATED_BODY()
public:
	virtual void Unload_Implementation() override;

	TArray<FCard_ATK_Data>& GetCardATK();
	TArray<FCard_Spawn_Data>& GetCardSpawn();
	TArray<FCard_Defence_Data>& GetCardDefence();
	TArray<FCard_Function_Data>& GetCardFunction();
	TArray<FCard_RangeATK_Data>& GetCardRangeATK();
private:
	DataTableAssetData<FCard_ATK_Data> CardATK;
	UPROPERTY()
	TArray<FCard_ATK_Data> CardATKData;

	DataTableAssetData<FCard_Spawn_Data> CardSpawn;
	UPROPERTY()
	TArray<FCard_Spawn_Data> CardSpawnData;

	DataTableAssetData<FCard_Defence_Data> CardDefence;
	UPROPERTY()
	TArray<FCard_Defence_Data> CardDefenceData;

	DataTableAssetData<FCard_Function_Data> CardFunction;
	UPROPERTY()
	TArray<FCard_Function_Data> CardFunctionData;

	DataTableAssetData<FCard_RangeATK_Data> CardRangeATK;
	UPROPERTY()
	TArray<FCard_RangeATK_Data> CardRangeATKData;
};

UCLASS()
class FVM_API UCardDataStruct : public UObject
{
	GENERATED_BODY()

};
