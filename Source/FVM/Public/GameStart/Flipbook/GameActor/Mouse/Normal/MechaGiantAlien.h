// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/GunAlien.h"
#include "MechaGiantAlien.generated.h"

/**
 * 电锯金刚
 */
UCLASS()
class FVM_API AMechaGiantAlien : public AGunAlien
{
	GENERATED_BODY()
public:
	virtual void MouseTick(const float& DeltaTime) override;
	void OnAnimationPlayEnd2(class UTrackEntry* Track);
public:
	void PlayAttack2Anim();
	virtual	void CheckCard() override;
private:
	UPROPERTY()
	bool bEnableAttackLine = true;
};
