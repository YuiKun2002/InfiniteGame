// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GiantxAlien.generated.h"

/**
 * 普通巨人
 */
UCLASS()
class FVM_API AGiantxAlien : public ANormalCapsuleByWidgetBase
{
	GENERATED_BODY()
public:
	//攻击范围半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "巨人")
	float AttackRadius = 150.f;
	//可以攻击卡片的什么类型【攻击力0】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "巨人")
	ECardCollisionType MECardCollisionType = ECardCollisionType::E_CardActor2;
public:
	//初始化
	virtual void BeginPlay() override;
	//初始化
	virtual void MouseInit() override;
	//老鼠死亡
	virtual void MouseDeathed() override;
public:
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void MoveingUpdate(float DeltaTime) override;
	virtual void AttackedBegin() override;
	virtual void AttackedEnd() override;

	UFUNCTION()
	void AnimPlayEnd(class UTrackEntry* Track);

public:
	void PlayWalk();
	void PlayAttack();
private:
	UPROPERTY()
	int32 State = -1;
	UPROPERTY()
	bool bAttackLineEnable = true;
};
