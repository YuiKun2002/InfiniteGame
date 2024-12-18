﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_MechaSpider.h"
#include "Boss_Tank.generated.h"


//行走
UCLASS()
class FVM_API UBoss_Tank_Walk : public UBoss_MechaSpider_Walk
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
private:
	//发射子弹的时间
	UPROPERTY()
	float LaunchTime = 0.f;
};


//发射导弹
UCLASS()
class FVM_API UBoss_Tank_AttackMissile : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	void PlayAttackAnim();
public:
	UFUNCTION()
	void AAnimPlayEnd(class UTrackEntry* Track);
	UFUNCTION()
	void Launch(UTrackEntry* entry, FSpineEvent evt);
private:
	UPROPERTY()
	class ABoss_Tank* Boss = nullptr;
};

//发射激光
UCLASS()
class FVM_API UBoss_Tank_AttackLaser : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	void PlayAttackAnim();
public:
	UFUNCTION()
	void AAnimPlayEnd(class UTrackEntry* Track);
	UFUNCTION()
	void Launch(UTrackEntry* entry, FSpineEvent evt);
private:
	UPROPERTY()
	class ABoss_Tank* Boss = nullptr;
};

/**
 * 坦克boss
 */
UCLASS()
class FVM_API ABoss_Tank : public ABoss_MechaSpider
{
	GENERATED_BODY()
public:
	//攻击延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "坦克Boss外星人")
	float LaunchTime = 15.f;
	//激光攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "坦克Boss外星人")
	TSubclassOf<class UAssetCategoryName> LaserAttackAnimRes;
	//激光受到伤害的攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "坦克Boss外星人")
	TSubclassOf<class UAssetCategoryName> LaserAttackAnimDamageRes;
public:
	//初始化
	virtual void MouseInit() override;
public:
	//生成导弹
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnMissile();
	//生成激光
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnLaser();
};
