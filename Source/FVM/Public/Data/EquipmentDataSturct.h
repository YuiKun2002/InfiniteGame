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

/************************************************************************/
/*                              武器类型                                 */
/************************************************************************/

UENUM(BlueprintType)
enum class EWeaponType : uint8//[武器类型]
{
	//主武器
	MainWeapon UMETA(DisplayName = "MainWeapon"),
	//副武器
	SecondaryWeapon UMETA(DisplayName = "SecondaryWeapon")
};

//武器
USTRUCT(BlueprintType)
struct FItemWeaponBase : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	//武器等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevel = 1;
	//星星等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StarsLevel = 0;
	//武器类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::MainWeapon;
};

//主武器数据结构
USTRUCT(BlueprintType)
struct FMainWeaponData : public FItemWeaponBase{
	GENERATED_USTRUCT_BODY()
public:
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATK = 2.f;
	//攻击力加成[%]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATKRate = 1.f;
	//攻击冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolingTime = 1.f;
	//攻击前摇时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackFristTime = 0.8;
	//攻击后摇时间【攻击次数之间的间隔】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackBackTime = 0.2f;
	//攻击次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackCount = 1;
	//是否可以装备副武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEquipSecondary = true;
};

//副武器数据结构
USTRUCT(BlueprintType)
struct FSecondaryWeaponData : public FItemWeaponBase{
	GENERATED_USTRUCT_BODY()
public:
	//基础生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LifeBase = 100;
	//生命值回复
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LifeReply = 10;
	//生命值回复时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeReplyTime = 1.f;
	//攻击闪避率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackMissRate = 0.f;
};

//角色装备的武器
USTRUCT(BlueprintType)
struct FPlayerEquipWeapon : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	//主武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemWeaponBase MainWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMainWeaponData MainWeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMainEquip = false;

	//副武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemWeaponBase SecondaryWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMainWeaponData SecondaryWeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSecondaryEquip = false;
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
