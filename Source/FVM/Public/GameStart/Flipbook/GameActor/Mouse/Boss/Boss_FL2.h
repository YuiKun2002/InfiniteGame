// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once
#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_FL.h"
#include "Boss_FL2.generated.h"
/**
 * 法老，法老变身
 */


 //出场
UCLASS()
class FVM_API UFLB_Create : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
		bool bInit = false;
};

//发呆
UCLASS()
class FVM_API UFLB_Idle : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType);
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
		int32 Count = 4;
};

//移动
UCLASS()
class FVM_API UFLB_Move : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void InMapMeshe(ELineType CurLineType) override;
public:
	UPROPERTY()
		UTimeLineClass* MoveTime = nullptr;
	UPROPERTY()
		FVector CurLocation;
	UPROPERTY()
		FVector TargetLocation;
};

//技能选择
UCLASS()
class FVM_API UFLB_SelectSKill : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
};

//回复血量
UCLASS()
class FVM_API UFLB_ReverseHP : public UFL_ReverseHP
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};

//召唤甲虫
UCLASS()
class FVM_API UFLB_CallBug : public UFL_CallBug
{
	GENERATED_BODY()
public:
	virtual void AnimPlayEnd() override;
};

//卡片摧毁
UCLASS()
class FVM_API UFLB_KillCard : public UFL_KillCard
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};

//召唤老鼠
UCLASS()
class FVM_API UFLB_CallMouse : public UFL_CallMouse
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};