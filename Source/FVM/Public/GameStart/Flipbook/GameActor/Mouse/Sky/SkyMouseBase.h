// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "SkyMouseBase.generated.h"

/**
 * 普通飞行老鼠，一般不会有任何能力
 */
UCLASS()
class FVM_API ASkyMouseBase : public AMouseActor
{
	GENERATED_BODY()

public:
	//构造
	ASkyMouseBase();
	//初始化
	virtual void BeginPlay() override;
	//更新
	virtual void Tick(float DeltaTime) override;
	//动画播放完毕
	void OnAnimationPlayEnd();
public:
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
public:
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue,EFlyItemAttackType AttackType) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
public:
	//状态
	void State();
};
