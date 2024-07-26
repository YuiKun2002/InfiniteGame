// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/MatrixMainMouse.h"
#include "MatrixChildMouse.generated.h"

/**
 *忍者鼠
 */

class AMatrixMainMouse;

//所有资产路径
USTRUCT(BlueprintType)
struct FMatrixChildResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//召唤
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> OnCreate;

	//走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Walk;
	//走残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> WalkLow;

	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Attack;
	//攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> AttackLow;

	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleDeath;

};

UCLASS()
class FVM_API AMatrixChildMouse : public AMatrixGroupMouse
{
	GENERATED_BODY()

public:
	AMatrixChildMouse();
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//移动
	virtual void MoveFunc(const float& DeltaTime) override;
	//开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimPlayEnd();
	//更新动画
	virtual void UpdateAnim() override;
public:
	void AtkBegin();
	void MoveBegin();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMatrixChildResourceStruct Anim;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* CollisionComp = nullptr;

private:

	UPROPERTY()
		bool bAttack = false;
	UPROPERTY()
		float AttackDelay = 0.1f;
};
