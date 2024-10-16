// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "Giant.generated.h"

//行走
UCLASS()
class FVM_API UGiantAlien_Walk : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void AttackedBegin() override;
	virtual void MoveingUpdate(const float& DeltaTime) override;
private:
	UPROPERTY()
	class AGiantAlien* Boss = nullptr;
};

//攻击
UCLASS()
class FVM_API UGiantAlien_Attack : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AttackedEnd() override;
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
	class AGiantAlien* Boss = nullptr;
};

/**
 * 巨人Boss
 */
UCLASS()
class FVM_API AGiantAlien : public ABossCapsuleBase
{
	GENERATED_BODY()
public:
	//攻击范围半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "巨人Boss")
	float AttackRadius = 150.f;
	//可以攻击卡片的什么类型【攻击力0】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "巨人Boss")
	ECardCollisionType MECardCollisionType = ECardCollisionType::E_CardActor2;
public:
	//初始化
	virtual void BeginPlay() override;
	//初始化
	virtual void MouseInit() override;
	//老鼠死亡
	virtual void MouseDeathed() override;
};
