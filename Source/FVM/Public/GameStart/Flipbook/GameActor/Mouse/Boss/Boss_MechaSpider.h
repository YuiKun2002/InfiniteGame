// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "Boss_MechaSpider.generated.h"

class USceneComponent;


//行走
UCLASS()
class FVM_API UBoss_MechaSpider_Walk : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MoveingUpdate(const float& DeltaTime) override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)override;
	void CheckCard();
	void PlayMoveAnim();
protected:
	UPROPERTY()
	class ABoss_MechaSpider* Boss = nullptr;
	UPROPERTY()
	float Time = 0.f;
	UPROPERTY()
	int32 State = -1;
};

/**
 * 机械蜘蛛Boss
 */
UCLASS()
class FVM_API ABoss_MechaSpider : public ABossCapsuleBase
{
	GENERATED_BODY()
public:
	//左检测点位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* CheckLeftLocationComp = nullptr;
	//右检测点位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* CheckRightLocationComp = nullptr;
	//检测卡片类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType CheckCardType = ECardCollisionType::E_CardActor2;
public:
	ABoss_MechaSpider();
	//初始化
	virtual void BeginPlay() override;
	//初始化
	virtual void MouseInit() override;
	//老鼠死亡
	virtual void MouseDeathed() override;
	//当Boss死亡
	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();
};
