// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/ItemStruct.h"
#include "EquipmentDataSturct.generated.h"


 //装备-邮件
USTRUCT(BlueprintType)
struct FEquipment_FMail_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FMail M_FEquipment;
};


//装备-玩家补偿数据
USTRUCT(BlueprintType)
struct FPlayerVersionCompensate_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FPlayerVersionCompensate M_FPlayerVersionCompensate;
};

//装备数据资产缓存
UCLASS()
class FVM_API UEquipmentDataAssetCache : public UGameDataAssetCache
{
	GENERATED_BODY()
public:
	virtual void Unload_Implementation() override;

	//背包
	TArray<FEquipment_Bag_Data>& GetBag();
	//服装装备
	TArray<FEquipment_PlayerEquipment_Data>& GetPlayerEquipment();
	//礼盒
	TArray<FEquipment_GiftBox_Data>& GetGiftBox();
	//主武器
	TArray<FEquipment_WeaponFirst_Data>& GetWeaponFirst();
	//副武器
	TArray<FEquipment_WeaponSecond_Data>& GetWeaponSecond();
	//超级武器
	TArray<FEquipment_WeaponSuper_Data>& GetWeaponSuper();
	//激光宝石类
	TArray<FEquipment_GemLaser_Data>& GetGemLaser();
	//攻击宝石类
	TArray<FEquipment_GemAttack_Data>& GetGemAttack();
	//获取邮件
	TArray<FEquipment_FMail_Data>& GetMail();

public:
	DataTableAssetData<FEquipment_Bag_Data> Bag;
	TArray<FEquipment_Bag_Data> BagData;

	TArray<FEquipment_PlayerEquipment_Data> PlayerEquipmentData;

	DataTableAssetData<FEquipment_GiftBox_Data> GiftBox;
	TArray<FEquipment_GiftBox_Data> GiftBoxData;

	DataTableAssetData<FEquipment_WeaponFirst_Data> WeaponFirst;
	TArray<FEquipment_WeaponFirst_Data> WeaponFirstData;

	DataTableAssetData<FEquipment_WeaponSecond_Data> WeaponSecond;
	TArray<FEquipment_WeaponSecond_Data> WeaponSecondData;

	DataTableAssetData<FEquipment_WeaponSuper_Data> WeaponSuper;
	TArray<FEquipment_WeaponSuper_Data> WeaponSuperData;

	DataTableAssetData<FEquipment_GemLaser_Data> GemLaser;
	TArray<FEquipment_GemLaser_Data> GemLaserData;

	DataTableAssetData<FEquipment_GemAttack_Data> GemAttack;
	TArray<FEquipment_GemAttack_Data> GemAttackData;

	DataTableAssetData<FEquipment_FMail_Data> Mail;
	TArray<FEquipment_FMail_Data> MailData;
};


UCLASS()
class FVM_API UEquipmentDataSturct : public UObject
{
	GENERATED_BODY()

};
