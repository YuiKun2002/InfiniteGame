// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "MouseCustomBase.generated.h"


UCLASS()
class FVM_API AMouseCustomBase : public AMouseActor
{
	GENERATED_BODY()
public:
	AMouseCustomBase();
public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
public:
	//基本老鼠类型
	virtual EMouseBaseType GetMouseBaseType() override;
public:
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue,EFlyItemAttackType AttackType) override;
	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;

	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
};
