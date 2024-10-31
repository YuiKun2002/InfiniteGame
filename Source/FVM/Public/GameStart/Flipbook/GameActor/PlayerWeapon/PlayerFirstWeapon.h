// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "Data/EquipmentDataSturct.h"
#include "GameSystem/Item/ItemStruct.h"
#include "PlayerFirstWeapon.generated.h"

class UFirstWeaponProjectionComponent;
class UPlayerStructManager;
class UPaperFlipbook;
class UUI_MapMeshe;
class AMapMeshe;
class AMouseActor;
class AGamePlayer;

//---------------------------------------------------------------------------------
//子弹的属性数值重写
USTRUCT(BlueprintType)
struct FFlyItemProOverride
{
	GENERATED_USTRUCT_BODY()
public:
	//是否重写
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverride = false;
	//覆盖武器基础子弹
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class AFlyItemActor> WeaponBulletClassObjOverride;
	//子弹攻击的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFlyItemAttackType FlyItemAttackType = EFlyItemAttackType::Def;
	//穿透层数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PanetrateLayers = 999;
	//静态物品创建一次【遇到穿透可能会多次创建】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCreateStaticItemOnceOverride = false;
	//攻击类型重写
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ELineType> AttackTypeOverride;
	//Buff重写
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItem_Buff ItemBuffOverride;
	//基础攻击力附加重写【基于原来的攻击力增长】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackUpRate = 1.f;
	//二次攻击力覆盖重写【覆盖原有的二次攻击力】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondAttackOverwrite = 0.f;
};

//数据计算
UCLASS()
class FVM_API UFlyItemProOverrideFunc : public UObject
{
	GENERATED_BODY()
public:
	//重写子弹
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetWeaponBulletClassObjOverride(
		const FFlyItemProOverride& FlyItemProOverride,
		const TSoftClassPtr<class AFlyItemActor>& Fly
	);
	//重写攻击类型
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetFlyItemAttackType(
		const FFlyItemProOverride& FlyItemProOverride,
		EFlyItemAttackType Type
	);
	//重写穿透层数
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetPanetrateLayers(
		const FFlyItemProOverride& FlyItemProOverride,
		int32 PanetrateLayers
	);
	//重写是否静态物品创建一次
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetbCreateStaticItemOnceOverride(
		const FFlyItemProOverride& FlyItemProOverride,
		bool bCreateStaticItemOnceOverride
	);
	//重写攻击目标
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetAttackTypeOverride(
		const FFlyItemProOverride& FlyItemProOverride,
		const TArray<ELineType>& AttackTypeOverride
	);

	//重写Buff
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetItemBuffOverride(
		const FFlyItemProOverride& FlyItemProOverride,
		const FItem_Buff& ItemBuffOverride
	);

	//重写攻击力加成
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetBaseAttackUpRate(
		const FFlyItemProOverride& FlyItemProOverride,
		float BaseAttackUpRate = 1.f
	);

	//重写二次攻击力加成【* 基础攻击力】（攻击力加成倍率）
	UFUNCTION(BlueprintCallable, Category = "武器飞行物属性重写")
	static FFlyItemProOverride SetSecondAttackOverwrite(
		const FFlyItemProOverride& FlyItemProOverride,
		float SecondAttackOverwrite = 0.f
	);
};

//---------------------------------------------------------------------------------

//技能修改的返回结果
USTRUCT(BlueprintType)
struct FWeaponSkillObjectReturn
{
	GENERATED_USTRUCT_BODY()
public:
	//武器数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMainWeaponData Data;
	//子弹属性重写
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFlyItemProOverride ProOverride;
};

//武器技能对象
UCLASS(Blueprintable)
class FVM_API UWeaponSkillObject : public UObject {
	GENERATED_BODY()
public:
	//技能运行
	UFUNCTION(BlueprintImplementableEvent)
	FWeaponSkillObjectReturn SkillRun(
		const FMainWeaponData& Data,
		const FFlyItemProOverride& ProOverride
	);
	//技能运行
	virtual void Run(FMainWeaponData& Data, FFlyItemProOverride& ProOverride);
};

UCLASS()
class FVM_API APlayerFirstWeapon : public ASpineActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFirstWeaponProjectionComponent* M_UFirstWeaponProjectionComponent = nullptr;
	//子弹的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* BulletLocationComp = nullptr;
	//浮动骨骼
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpineBoneFollowerComponent* BoneFollowerComp = nullptr;
public:
	//武器基础子弹
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "武器属性")
	TSoftClassPtr<class AFlyItemActor> WeaponBulletClassObj;
	//技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "武器属性")
	TMap<int32, TSubclassOf<UWeaponSkillObject>> WeaponSkils;
	//飞行物属性重写
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "武器属性")
	FFlyItemProOverride FlyItemProOverride;
public:

	APlayerFirstWeapon();

	//初始化技能
	virtual	void InitSkill(FMainWeaponData& WeaponData);

	//初始化武器数据
	void InitWeapon(
		AGamePlayer* Player,
		const FMainWeaponData& WeaponData,
		UUI_MapMeshe* _UI_MapMeshe,
		AMapMeshe* _MapMeshe
	);

	//获取主武器数据
	const FMainWeaponData& GetPlayerFirstWeaponData();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//板块移动
	void MeshMoveUpdate(float DeltaTime, UUI_MapMeshe* _UI_MapMeshe, AMapMeshe* _MapMeshe);
public:
	//设置主角
	void SetPlayeActor(AGamePlayer* _Player);
public:
	//获取子弹的位置
	USceneComponent* GetBulletLocationComp();
	//获取主角
	AGamePlayer* GetPlayerActor();
	//获取射线位置
	const TArray<FLineTraceSetting>& GetLineTraceSetting();
private:
	//主武器数据
	UPROPERTY()
	FMainWeaponData M_FFPlayerWeaponFirstData;
	//主武器射线位置(已经计算过后)
	UPROPERTY()
	TArray<FLineTraceSetting> M_FirstWeaponLineTraceSettings;
	//角色
	UPROPERTY()
	AGamePlayer* M_AGamePlayer = nullptr;
	//对应的网格实例
	UPROPERTY()
	AMapMeshe* M_AMapMeshe = nullptr;
};
