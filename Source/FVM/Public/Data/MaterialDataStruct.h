// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "Data/CardSkillBookStruct.h"
#include "MaterialDataStruct.generated.h"

 //材料-防御卡合成配方数据
USTRUCT(BlueprintType)
struct FMaterial_CardSynthesisBlueprint_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//配方结构
	UPROPERTY(EditDefaultsOnly)
	FCardBlueprint M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//材料-防御卡合成配方材料数据
USTRUCT(BlueprintType)
struct FMaterial_CardSynthesisMaterial_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//配方材料结构
	UPROPERTY(EditDefaultsOnly)
	FCardBlueprintMaterial M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//材料-兑换券材料数据
USTRUCT(BlueprintType)
struct FMaterial_TicketMaterial_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//兑换券结构
	UPROPERTY(EditDefaultsOnly)
	FTicket M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//强化水晶
USTRUCT(BlueprintType)
struct FMaterial_Crystal_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FCrystalMaterial M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//武器开槽-钻头
USTRUCT(BlueprintType)
struct FMaterial_Bit_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FBitMaterial M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//关卡钥匙
USTRUCT(BlueprintType)
struct FMaterial_LevelKey_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FLevelKey M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//材料-防御卡转职材料数据
USTRUCT(BlueprintType)
struct FMaterial_CardChangeJobMaterial_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//转职材料结构
	UPROPERTY(EditDefaultsOnly)
	FCardChangeJobMaterial M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//材料-防御卡金卡进化数据
USTRUCT(BlueprintType)
struct FMaterial_GoldCardUp_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//金卡进化对应的防御卡名称
	UPROPERTY(EditDefaultsOnly)
	FString M_AddCardName = TEXT("强袭煮蛋器投手双鱼座星宿");
	//对应的金卡进化凭证
	UPROPERTY(EditDefaultsOnly)
	FString M_GoldCardItemName = TEXT("雷神进化凭证");
	//对应的金卡名称
	UPROPERTY(EditDefaultsOnly)
	FString M_GoldCardName = TEXT("雷神");
};

//材料-香料数据
USTRUCT(BlueprintType)
struct FMaterial_SpicesMaterial_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//转职材料结构
	UPROPERTY(EditDefaultsOnly)
	FSpicesMaterial M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//材料-四叶草数据
USTRUCT(BlueprintType)
struct FMaterial_CloverMaterial_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//四叶草结构
	UPROPERTY(EditDefaultsOnly)
	FCloverMaterial M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//角色-技能书
USTRUCT(BlueprintType)
struct FCardSkillBooks_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FCardSkillBookMaterial M_FMaterial;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FMaterial;
		return true;
	}
};

//材料数据资产缓存
UCLASS()
class FVM_API UMaterialDataAssetCache : public UGameDataAssetCache
{
	GENERATED_BODY()
public:
	virtual void Unload_Implementation() override;
public:
	//配方
	TArray<FMaterial_CardSynthesisBlueprint_Data>& GetBlueprint();
	//配方材料
	TArray<FMaterial_CardSynthesisMaterial_Data>& GetBlueprintMater();
	//转职材料
	TArray<FMaterial_CardChangeJobMaterial_Data>& GetChange();
	//香料
	TArray<FMaterial_SpicesMaterial_Data>& GetSpices();
	//四叶草
	TArray<FMaterial_CloverMaterial_Data>& GetClover();
	//技能书
	TArray<FCardSkillBooks_Data>& GetSkillBook();
	//兑换券
	TArray<FMaterial_TicketMaterial_Data>& GetTicket();
	//强化水晶
	TArray<FMaterial_Crystal_Data>& GetCrystal();
	//钻头
	TArray<FMaterial_Bit_Data>& GetBit();
	//钥匙徽章
	TArray<FMaterial_LevelKey_Data>& GetLevelKey();
private:
	DataTableAssetData<FMaterial_CardSynthesisBlueprint_Data> Blueprint;
	UPROPERTY()
	TArray<FMaterial_CardSynthesisBlueprint_Data> BlueprintData;

	DataTableAssetData<FMaterial_CardSynthesisMaterial_Data> BlueprintMater;
	UPROPERTY()
	TArray<FMaterial_CardSynthesisMaterial_Data> BlueprintMaterData;

	DataTableAssetData<FMaterial_CardChangeJobMaterial_Data> Change;
	UPROPERTY()
	TArray<FMaterial_CardChangeJobMaterial_Data> ChangeData;

	DataTableAssetData<FMaterial_SpicesMaterial_Data> Spices;
	UPROPERTY()
	TArray<FMaterial_SpicesMaterial_Data> SpicesData;

	DataTableAssetData<FMaterial_CloverMaterial_Data> Clover;
	UPROPERTY()
	TArray<FMaterial_CloverMaterial_Data> CloverData;

	DataTableAssetData<FCardSkillBooks_Data> SkillBook;
	UPROPERTY()
	TArray<FCardSkillBooks_Data> SkillBookData;

	DataTableAssetData<FMaterial_TicketMaterial_Data> Ticket;
	UPROPERTY()
	TArray<FMaterial_TicketMaterial_Data> TicketData;

	DataTableAssetData<FMaterial_Crystal_Data> Crystal;
	UPROPERTY()
	TArray<FMaterial_Crystal_Data> CrystalData;

	DataTableAssetData<FMaterial_Bit_Data> Bit;
	UPROPERTY()
	TArray<FMaterial_Bit_Data> BitData;

	DataTableAssetData<FMaterial_LevelKey_Data> LevelKey;
	UPROPERTY()
	TArray<FMaterial_LevelKey_Data> LevelKeyData;
};



UCLASS()
class FVM_API UMaterialDataStruct : public UObject
{
	GENERATED_BODY()
};
