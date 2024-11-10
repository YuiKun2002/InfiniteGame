// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/GunAlienWidget.h"
#include "MechaGiantAlien.generated.h"

/**
 * 电锯金刚
 */
UCLASS()
class FVM_API AMechaGiantAlien : public AGunAlienWidget
{
	GENERATED_BODY()
public:
	//近战攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 MeleeCombatATK = 9999;
public:
	UFUNCTION()
	void OnAnimationPlayEnd2(class UTrackEntry* Track);
	void PlayAttack2Anim();
	virtual	void CheckCard() override;
private:
	UPROPERTY()
	bool bEnableAttackLine = true;
};
