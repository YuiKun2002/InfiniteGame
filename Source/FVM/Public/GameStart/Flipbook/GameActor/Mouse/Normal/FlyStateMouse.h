// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "FlyStateMouse.generated.h"

/**
 *	飞行老鼠
 */

class UPaperFlipbook;
class UBoxComponent;
class USphereComponent;
class AObstacleActor;

//所有资产路径
USTRUCT(BlueprintType)
struct FFlyStateResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态飞行(手持炸弹)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> FlyTake;
	//正常形态飞行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Fly;
	//正常形态掉落
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Falling;

	//正常形态走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Walk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> WalkLow;

	//攻击形态走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> AttackLow;

	//死亡形态(非手持炸弹死亡)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleDeath;
};



UCLASS()
class FVM_API AFlyStateMouse : public ANormalBase
{
	GENERATED_BODY()

public:
	AFlyStateMouse();
	//初始化
	virtual void BeginPlay() override;
	//初始化
	virtual void MouseInit() override;
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue, EFlyItemAttackType AttackType) override;
	//移动逻辑
	virtual void MoveingUpdate(float DeltaTime) override;
	//老鼠死亡
	virtual void MouseDeathed() override;
	//进入网格时
	virtual void InMapMeshe(ELineType CurLineType) override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag,float& CurBuffTime);

	//动画播放完毕
	UFUNCTION()
		void OnAnimationPlayEnd();
	//启动
	void EnableCo(bool COnGround);
	//关闭
	void DisEnableCo();
public:
	//飞行状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FFlyStateResourceStruct FlyStateResourceStruct;
	//启用投掷物品
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEnableProjection = false;
	//障碍物类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AObstacleActor> MouseBoxClass;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComp = nullptr;
protected:
	UFUNCTION()
		void OnResourceLoadFinished();
	UFUNCTION()
		void UpdateState();
private:
	//处于飞行状态
	UPROPERTY()
		bool bFly = true;
	//是否抛出炸弹
	UPROPERTY()
		bool bShoot = false;
	//启动掉落
	UPROPERTY()
		bool bFalling = false;
	//掉落开始
	UPROPERTY()
		bool bFallingStart = false;
	//开始启动攻击
	UPROPERTY()
		bool bEnableAttack = false;
	//投掷延迟
	UPROPERTY()
		float bShootDelay = 5.f;
};