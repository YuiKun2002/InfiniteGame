// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "JumpStateMouse.generated.h"

/**
 * 跳跃老鼠
 */

class UBoxComponent;
class USphereComponent;

//所有资产路径
USTRUCT(BlueprintType)
struct FJumpStateResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态(滑板)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleLow;

	//正常形态(跳跃)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> JumpLow;

	//正常形态(走)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Walk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> WalkLow;

	//正常形态(攻击)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> AttackLow;

	//死亡形态(死亡)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
};


UCLASS()
class FVM_API AJumpStateMouse : public ANormalBase
{
	GENERATED_BODY()

public:
	AJumpStateMouse();
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//开始攻击
	virtual void AttackedBegin() override;
	//老鼠移动
	virtual void MoveingUpdate(float DeltaTime) override;
	//受到攻击
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue, EFlyItemAttackType AttackType);
	//老鼠死亡
	virtual void MouseDeathed() override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
protected:
	//动画播放完毕
	UFUNCTION()
		void AnimPlayEnd();
	//检测卡片
	UFUNCTION()
		bool CheckCard(const float& DeltaTime);
public:
	//老鼠的动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJumpStateResourceStruct JumpAnim;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComp = nullptr;
private:
	void UpdateState();
private:

	//是否可以跳跃
	UPROPERTY()
		bool bJumpStart = false;
	//跳跃结束
	UPROPERTY()
		bool bJumpEnd = false;
	//可以越过
	UPROPERTY()
		bool bJumpPass = true;
	//检测延迟
	UPROPERTY()
		float CheckDelay = 0.2f;
	//是否可以攻击
	UPROPERTY()
		bool bAttack = false;
};
