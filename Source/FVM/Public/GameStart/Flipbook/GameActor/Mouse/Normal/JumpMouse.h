// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Data/CardData/MouseDataStruct.h"
#include "JumpMouse.generated.h"

/**
 * 跳跳鼠
 */

class UBoxComponent;
class USphereComponent;

//跳跳鼠
USTRUCT(BlueprintType)
struct FJumpMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//跳跃
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Jump;
	//跳跃残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> JumpLow;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
};

UCLASS()
class FVM_API AJumpMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	AJumpMouse();
	//Tick 更新
	virtual void MouseTick(const float& DeltaTime) override;
	//老鼠初始化
	virtual void MouseInit() override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimationPlayEnd();
	//更新状态
	void UpdateState();
	//检测卡片
	bool CheckCard(const float& DeltaTime);
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBodyComponent = nullptr;
	//老鼠动画资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJumpMouseResourceStruct MouseAnimalRes;
private:
	bool bHi = false;
	bool bJump = false;
	bool bJumpStart = false;
	float CheckDelay = 0.f;
	float JumpSpeedRate = 1.f;
};
