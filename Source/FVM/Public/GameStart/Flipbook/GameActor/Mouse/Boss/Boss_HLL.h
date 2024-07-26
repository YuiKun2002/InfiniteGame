// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "Boss_HLL.generated.h"

/**
 * 轰隆隆
 */
UCLASS()
class FVM_API ABoss_HLL : public ABossSphereBase
{
	GENERATED_BODY()
public:
	//创建
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Create;
	//发呆
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Idle;
	//发呆残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_IdleLow;
	//飞行
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Fly;
	//飞行残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_FlyLow;
	//飞行准备
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_FlyReady;
	//飞行准备残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_FlyReadyLow;
	//降落
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Fall;
	//降落残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_FallLow;
	//向前飞行
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_ForwardFly;
	//向前飞行残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_ForwardFlyLow;
	//向后飞行
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_BackwardFly;
	//向后飞行残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_BackwardFlyLow;
	//踩踏攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_FallATK;
	//踩踏攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_FallATKLow;
	//导弹攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_MissileATK;
	//导弹攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_MissileATKLow;
	//激光攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_LaserATK;
	//激光攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_LaserATKLow;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 轰隆隆")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Death;
	//移动曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 移动曲线")
		TSoftObjectPtr<UCurveFloat> BossMoveCurve;
public:
	UPROPERTY()
		int32 CurStateNum = 0;
public:
	//轰隆隆构造
	ABoss_HLL();
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//进入网格
	virtual void InMapMeshe(ELineType CurLineType) override;
public:
	//生成导弹
	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnMissile(FVector Location);
	//生成极光
	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnLaser(FVector Location);
	//生成范围伤害
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnRange();
};

//轰隆隆通用状态基本类型
UCLASS()
class FVM_API UHLL_BaseState : public UBossUniformState
{
	GENERATED_BODY()
public:
	//获取轰隆隆
	ABoss_HLL* GetHLL();
	//受到攻击
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
};

//创建
UCLASS()
class FVM_API UHLL_CreateState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};

//导弹攻击
UCLASS()
class FVM_API UHLL_MissileState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
};

//导弹攻击
UCLASS()
class FVM_API UHLL_MissileSecondState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
private:
	UPROPERTY()
		int32 Count = 8;
};

//极光攻击
UCLASS()
class FVM_API UHLL_LaserState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
private:
	UPROPERTY()
		int32 IdleCount = 3;
};

//飞行[降落打击]
UCLASS()
class FVM_API UHLL_FlyState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};

//飞行发呆【从最后一列飞行】
UCLASS()
class FVM_API UHLL_FlyItemLastMoveState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
		int32 IdleCount = 3;
	UFUNCTION()
		void SelectPoint();
};

//向前飞行
UCLASS()
class FVM_API UHLL_FlyForwardState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
	void PlayMove(FLine NewLine);
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
public:
	UPROPERTY()
		FVector Location;
	UPROPERTY()
		FVector TargetLocation;
	UPROPERTY()
		FLine CurLine;
private:
	UPROPERTY()
		class UTimeLineClass* CurTimeLine = nullptr;
};

//降落攻击
UCLASS()
class FVM_API UHLL_FallATKtate : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};

//飞行
UCLASS()
class FVM_API UHLL_FlySecondState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
};

//当前位置飞行发呆
UCLASS()
class FVM_API UHLL_FlyICurMoveState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
private:
	UPROPERTY()
		int32 IdleCount = 3;
	UFUNCTION()
		void SelectPoint();
};

//向后飞行
UCLASS()
class FVM_API UHLL_FlyBackwardState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
	void PlayMove(FLine NewLine);
public:
	UPROPERTY()
		FVector Location;
	UPROPERTY()
		FVector TargetLocation;
	UPROPERTY()
		FLine CurLine;
private:
	UPROPERTY()
		class UTimeLineClass* CurTimeLine = nullptr;
};

//飞行降落
UCLASS()
class FVM_API UHLL_FlyFalltate : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};

//发呆【技能选择】
UCLASS()
class FVM_API UHLL_IdleState : public UHLL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;

public:
	UPROPERTY()
		int32 IdleCount = 1;
};
