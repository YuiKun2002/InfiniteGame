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

UCLASS()
class FVM_API UCardDataStruct : public UObject
{
	GENERATED_BODY()

};
