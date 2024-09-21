// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "FirstWeaponProjectionComponent.generated.h"

/**
 * 主武器投射组件
 */

 //动态多播代理->当进行物体投射时->触发代理
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemSpawnDelegate);

class APlayerFirstWeapon;
class AFlyItemActor;

UCLASS(ShowCategories = (Mobility, ComponentReplication), ClassGroup = Paper2D, meta = (BlueprintSpawnableComponent))
class FVM_API UFirstWeaponProjectionComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
public:
	//可蓝图调用
	UPROPERTY(BlueprintAssignable)
	FItemSpawnDelegate OnSpawn;
public:
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	//初始化攻击条件时间等
	virtual void InitCondition() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//资源加载
	virtual void LoadResource() override;
public:
	//生成子弹
	UFUNCTION(BlueprintCallable)
	AFlyItemActor* SpawnFlyItem(TSoftClassPtr<AFlyItemActor> _Path_C, FVector _Offset);
	//更新攻击->有老鼠就自动攻击
	UFUNCTION(BlueprintCallable)
	void UpdateAutoAttack(float _DeltaTime);
private:
	/*碰撞结果*/
	FHitResult M_Trance_Result;
	FCollisionQueryParams M_Trance_Params;
	FCollisionResponseParams M_Trance_ResParams;
	ECollisionChannel M_Trance_Target;
	/*时间*/
	float M_time = 0;
	/*子弹生成位置*/
	FVector SpawnBulletLocation = FVector::ZeroVector;
private:
	//主武器
	UPROPERTY()
	APlayerFirstWeapon* M_Owner = nullptr;
	UPROPERTY()
	UObjectPoolManager* Pool = nullptr;
};
