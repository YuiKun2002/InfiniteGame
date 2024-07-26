// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Equipment/EquipmentBaseStruct.h"
#include "GameSystem/Item/Equipment/GemsStruct.h"
#include "PlayerWepaonStruct.generated.h"


//---------------------------------------------------------------------------------------------------------
// 
//    武器结构开始
//---------------------------------------------------------------------------------------------------------

//道具->武器宝石->槽
USTRUCT(BlueprintType)
struct FPlayerWeaponGemSlot {
	GENERATED_USTRUCT_BODY()
public:

	//宝石不需要ID存储到武器->使用了宝石，宝石道具会消失->如果卸载宝石，宝石将返回背包

	//当前槽位是否解锁
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CurrentWeaponGemEnable = false;
	//宝石的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_WepaonGemName = TEXT("");
	//宝石的等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_WeaponGemGrade = 0;
	//宝石纹理[不可手动修改，从数据表修改]
	UPROPERTY()
		FString M_WepaonGemHeadPath = TEXT("");
};
//道具->玩家武器
USTRUCT(BlueprintType)
struct FPlayerWeaponBase : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	FPlayerWeaponBase() {}
public:
	//主武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerWeaponType M_EPlayerWeaponType = EPlayerWeaponType::E_First;
	//武器Def动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_WeaponDef_Path = FSoftObjectPath(TEXT(""));
	//武器Attack动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_WeaponAttack_Path = FSoftObjectPath(TEXT(""));
	//对象路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftClassPath M_WeaponClassPath = FSoftClassPath(TEXT(""));
	//宝石槽位限制(最多能开启4个槽位)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_WeaponGemCountMax = 4;
	//宝石数据（槽）->默认一个槽未开启->开启一个槽->增加一次，最多4次
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerWeaponGemSlot> M_WeaponGem;
};

//道具->玩家武器->主武器
USTRUCT(BlueprintType)
struct FPlayerWeaponFirst : public FPlayerWeaponBase
{
	GENERATED_USTRUCT_BODY()
public:
	//卡片基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATKBase = 0.f;
	//卡片攻击力加成比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATKRate = 0.f;
	//是否有溅射伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_SputteringHurt = false;
	//溅射伤害比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SputteringATKRate = 0.f;
	//攻击速度(攻击延迟)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootSpeed = 2.f;
	//首次投射时间 {延迟}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FirstShootSpeed = 0.5f;
	//投射延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootDelay = 0.1f;
	//投射多少发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ProjectionItemCount = 1;
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLineTraceSetting> M_LineTraceSettings;
};

//道具->玩家武器->副武器
USTRUCT(BlueprintType)
struct FPlayerWeaponSecond : public FPlayerWeaponBase
{
	GENERATED_USTRUCT_BODY()
public:
	//直接给玩家进行的体力加成
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_HP_Value = 10.f;
};

//道具->玩家武器->超级武器
USTRUCT(BlueprintType)
struct FPlayerWeaponSuper : public FPlayerWeaponBase
{
	GENERATED_USTRUCT_BODY()
public:
	//当前武器的基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK_Value = 30.f;
};

//---------------------------------------------------------------------------------------------------------
// 
//    武器结构开结束
//---------------------------------------------------------------------------------------------------------

//装备-玩家主武器数据
USTRUCT(BlueprintType)
struct FEquipment_WeaponFirst_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FPlayerWeaponFirst M_FEquipment;
};

//装备-玩家副武器数据
USTRUCT(BlueprintType)
struct FEquipment_WeaponSecond_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FPlayerWeaponSecond M_FEquipment;
};

//装备-玩家超级武器数据
USTRUCT(BlueprintType)
struct FEquipment_WeaponSuper_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FPlayerWeaponSuper M_FEquipment;
};

UCLASS()
class FVM_API UPlayerWepaonStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString GetDataTablePath(const EEquipment& _Equipment);
};
