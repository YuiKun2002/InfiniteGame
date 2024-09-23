// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Data/EquipmentDataSturct.h"
#include "GameStart/VS/Components/Card/AttackLauncherComponent.h"
#include "FirstWeaponProjectionComponent.generated.h"

/**
 * 主武器投射组件
 */

 //动态多播代理->当进行物体投射时->触发代理
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemSpawnDelegate);

class APlayerFirstWeapon;
class AFlyItemActor;

UCLASS(ShowCategories = (Mobility, ComponentReplication), ClassGroup = Paper2D, meta = (BlueprintSpawnableComponent))
class FVM_API UFirstWeaponProjectionComponent : public UAttackLauncherComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void SpawnBullet(AFlyItemActor* NewBullet);
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
	//当动画播放完毕
	UFUNCTION()
	void OnAnimationComplete(class UTrackEntry* Track);
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
	FMainWeaponData TargetData;
};
