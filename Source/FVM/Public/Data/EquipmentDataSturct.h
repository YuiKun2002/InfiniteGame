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
/*                              角色类型                                 */
/************************************************************************/

 //角色
USTRUCT(BlueprintType)
struct FEquipment_Hero_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//角色名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftClassPtr<class AGamePlayer>> Heroes;
};

//角色
USTRUCT(BlueprintType)
struct FItemHeroBase : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	//角色等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HeroLevel = 1;
	//角色星级  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StarsLevel = 0;
	//角色稀有度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RarityLevel = 0;
	//角色攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATK = 10;
	//角色加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATKRate = 1.2f;
	//角色生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP = 100;
	//角色系数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPCofficient = 1.1;
	//角色生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPRate = 10.f;
	//角色生命值系数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPRateCofficient = 1.1f;
	//角色CD
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CD = 5.f;
	//角色CD系数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CDCoefficient = 1.1f;
};

//数据计算
UCLASS()
class FVM_API UItemHeroDataFunc : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	static FItemHeroBase Calculate(const FItemHeroBase& InputData);
	UFUNCTION(BlueprintPure)
	static FItemHeroBase CalculateNext(const FItemHeroBase& InputData);
};

/************************************************************************/
/*                              武器类型                                 */
/************************************************************************/

 //武器
USTRUCT(BlueprintType)
struct FEquipment_Weapon_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//武器名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftClassPtr<class APlayerFirstWeapon>> Weapons;
};

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
struct FMainWeaponData : public FItemWeaponBase {
	GENERATED_USTRUCT_BODY()
public:
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATK = 2.f;
	//攻击力加成
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATKRCoefficient = 1.002f;

	//二次攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondATKRate = 0.f;

	//攻击冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolingTime = 1.f;
	//攻击冷却成长(0.003)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolingTimeCoefficient = 1.002f;
	//提升比例
	UPROPERTY()
	float AttackSpeedUpRate = 0.f;

	//攻击后摇时间【攻击次数之间的间隔】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackBackTime = 0.1f;
	//攻击次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackCount = 1;

	//暴击倍数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalMultiple = 2.f;
	//暴击率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalStrikeRate = 0.02f;
	//暴击率成长系数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalStrikeRateCoefficient = 1.2f;

	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLineTraceSetting> LineTraceSettings;

	//是否可以进化
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEvolve = false;

	//技能开启
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, TSubclassOf<class UWeaponSkillObject>> Skils;
	//技能描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FText> SkilDes;
};

//武器详细数据
USTRUCT(BlueprintType)
struct FMainWeaponData_Data : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMainWeaponData M_FEquipment;
};

//数据计算
UCLASS()
class FVM_API UMainWeaponDataFunc : public UObject
{
	GENERATED_BODY()
public:
	//计算数据
	UFUNCTION(BlueprintPure)
	static FMainWeaponData Calculate(const FMainWeaponData& InputData);
	//计算下一集
	UFUNCTION(BlueprintPure)
	static FMainWeaponData CalculateNext(const FMainWeaponData& InputData);
public:
	//提升攻击速度【0.f(0%) - 1.f(100%)】
	UFUNCTION(BlueprintPure, Category = "主武器数据计算")
	static FMainWeaponData Calculate_AttackSpeed(const FMainWeaponData& InputData, float Rate);
	//提升【基础】攻击力【0.f(0%),0.2f(20%Rate)】
	UFUNCTION(BlueprintPure, Category = "主武器数据计算")
	static FMainWeaponData Calculate_Attack(const FMainWeaponData& InputData, float Rate);
	//设置攻击次数
	UFUNCTION(BlueprintPure, Category = "主武器数据计算")
	static FMainWeaponData Calculate_AttackCount(const FMainWeaponData& InputData, int32 AttackCount);
	//设置攻击线路
	UFUNCTION(BlueprintPure, Category = "主武器数据计算")
	static FMainWeaponData Calculate_LineTraceSettings(const FMainWeaponData& InputData, const TArray<FLineTraceSetting>& LineTraceSettings);

};

//副武器数据结构
USTRUCT(BlueprintType)
struct FSecondaryWeaponData : public FItemWeaponBase {
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
	FMainWeaponData MainWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMainEquip = false;

	//副武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMainWeaponData SecondaryWeapon;
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
	//获取角色
	TArray<FEquipment_Hero_Data>& GetHeroes();
	//获取武器
	TArray<FEquipment_Weapon_Data>& GetWeapons();
	//获取武器详细数据
	TArray<FMainWeaponData_Data>& GetWeaponDetailDatas();
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

	//角色实例对象
	DataTableAssetData<FEquipment_Hero_Data> Heroes;
	TArray<FEquipment_Hero_Data> HeroesData;

	//武器实例对象
	DataTableAssetData<FEquipment_Weapon_Data> Weapons;
	TArray<FEquipment_Weapon_Data> WeaponsData;

	//武器详细数据
	DataTableAssetData<FMainWeaponData_Data> WeaponDetails;
	TArray<FMainWeaponData_Data> WeaponDetailDatas;
};


UCLASS()
class FVM_API UEquipmentDataSturct : public UObject
{
	GENERATED_BODY()
};
