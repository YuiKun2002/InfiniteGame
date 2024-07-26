// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Data/CardData/MouseDataStruct.h"
#include "UndergroundMouse.generated.h"

/**
 * 鼹鼠
 */

class UBoxComponent;
class USphereComponent;

USTRUCT(BlueprintType)
struct FUndergroundMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//地下
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> InUnder;
	//钻出
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Out;
	//发呆
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Idle;
	//走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Walk;
	//走残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> WalkLow;
	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> ATK;
	//攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> ATKLow;
	//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
	//是否钻出
	UPROPERTY()
		bool bOut = false;
};

UCLASS()
class FVM_API AUndergroundMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBodyComponent = nullptr;
	//老鼠动画资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FUndergroundMouseResourceStruct MouseAnimalRes;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType MECardCollisionType = ECardCollisionType::E_CardActor2;
public:
	AUndergroundMouse();
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin() override;
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd() override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimationPlayEnd();
	//更新状态
	void UpdateState();
private:
	//是否开启攻击
	bool bEnableAttakcLine = false;
	//开启钻出动画
	bool bEnOut = false;
	//是否完成动作
	bool bFinish = false;
};
