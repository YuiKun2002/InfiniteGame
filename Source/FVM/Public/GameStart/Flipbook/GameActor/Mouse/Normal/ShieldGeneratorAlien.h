// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "ShieldGeneratorAlien.generated.h"

/**
 * 护盾生成
 */
UCLASS()
class FVM_API AShieldGeneratorAlien : public ANormalCapsuleBase
{
	GENERATED_BODY()
public:
	AShieldGeneratorAlien();
public:
	//护盾动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "治疗外星人")
	TSoftClassPtr<ASpineActor> ShieldAnim;
	//护盾持续时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "护盾外星人")
	float fUseShieldTime = 10.f;
	//使用护盾的冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "护盾外星人")
	float fUseWeaponTime = 15.f;
	//抵挡伤害量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "护盾外星人")
	float fReplySelfHp = 100.f;
	//可以攻击卡片的什么类型【攻击力0】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "护盾外星人")
	ECardCollisionType MECardCollisionType = ECardCollisionType::E_CardActor2;
	//护盾碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "护盾外星人")
	class UCapsuleComponent* ShieldComp = nullptr;
public:
	virtual void BeginPlay() override;
	virtual void MouseInit() override;
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void MoveingUpdate(float DeltaTime) override;
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimationPlayEnd();
private:
	//是否启动攻击线
	bool bEnableAttakLine = true;
	//是否处于创建动画状态
	bool bUse = false;

	//使用护盾的延迟
	float fUseDelay = 0.f;
	float fReplySelfHp_ = 100.f;
	float fUseShieldTime_ = 10.f;
	//护盾
	UPROPERTY()
	ASpineActor* CurShieldAnimObj;
};
