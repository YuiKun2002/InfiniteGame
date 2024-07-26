// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "Boss_DaLiShen.generated.h"

class UPaperFlipbook;

//大力神的状态
UENUM()
enum class EDaLiShenState : uint8 {
	Def,
	Attack_1,
	Attack_2,
	Attack_3,
	Death,
};

class ADaLiShenMissile;

UCLASS()
class FVM_API ABoss_DaLiShen : public ABossBase
{
	GENERATED_BODY()
private:
	//大力神的状态
	UPROPERTY()
		EDaLiShenState M_EDaLiShenState = EDaLiShenState::Def;
private:
	//攻击时间(间隔)->一开始是6秒，后面统一15秒
	float AttakTimeCurrent = 6.f;
	//首次投射延迟
	float Attack_1_FristDelay = 0.5f;
	//首次投射是否完成
	bool bAttack_1_Frist = false;
	//投射间隔
	float Attack_1_Delay = 0.1f;
	//导弹数量
	int32 Attack_2_Count = 9;
	//当前创建个数
	int32 Attack_2_CurrentCount = 0;
public:
	//网格的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BoxMesh = nullptr;
	//身体的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BoxBody = nullptr;
private:
	UPROPERTY()
		UPaperFlipbook* M_Mouse_Def = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_Mouse_DefLow = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_Mouse_Attack_1 = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_Mouse_Attack_1Low = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_Mouse_Attack_2 = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_Mouse_Attack_2Low = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_Mouse_Attack_3 = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_Mouse_Attack_3Low = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_Mouse_Death = nullptr;
public:
	ABoss_DaLiShen();
public:
	//默认
	void Def_Tick(float DeltaTime);
	//攻击1
	void Attack1_Tick(float DeltaTime);
	//攻击3
	void Attack3_Tick(float DeltaTime);
public:
	//更新状态
	void UpdateState();
	//更新Attack1的状态
	void Update_Attack_1(float DeltaTime);
public:
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//动画播放完毕
	void OnAnimationPlayEnd();
public:
	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin() override;
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd() override;
	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
};
