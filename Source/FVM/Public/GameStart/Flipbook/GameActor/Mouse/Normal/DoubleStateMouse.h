// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "DoubleStateMouse.generated.h"


class UPaperFlipbook;
class UBoxComponent;
class USphereComponent;

//所有资产路径
USTRUCT(BlueprintType)
struct FDoubleStateResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态(手持炸弹)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleTake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleTakeLow;

	//正常形态(无炸弹)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleLow;

	//死亡形态(手持炸弹死亡)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleTakeDeath;
	//死亡形态(非手持炸弹死亡)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleDeath;
};


/**
 * 参考鲶鱼老鼠
 */
UCLASS()
class FVM_API ADoubleStateMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	//构造
	ADoubleStateMouse();
	//初始化
	virtual void BeginPlay() override;
	//更新
	virtual void Tick(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue,EFlyItemAttackType AttackType) override;
	//移动逻辑
	virtual void MoveingUpdate(float DeltaTime) override;
	//老鼠死亡
	virtual void MouseDeathed() override;
	//进入网格时
	virtual void InMapMeshe(ELineType CurLineType) override;
	//动画播放完毕
	void OnAnimationPlayEnd();
public:
	//老鼠的资源集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDoubleStateResourceStruct DoubleStateResourceStruct;
	//炸弹类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AObstacleActor> MouseBombClass;
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
	//是否抛出炸弹
	UPROPERTY()
		bool bShoot = false;
	//投掷延迟
	UPROPERTY()
		float bShootDelay = 12.f;
};
