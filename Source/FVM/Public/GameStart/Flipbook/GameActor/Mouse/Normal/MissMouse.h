// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameStart/VS/MapBaseType.h"
#include "MissMouse.generated.h"

/**
 * 非主流鼠
 */

class UBoxComponent;
class UPaperFlipbook;
class USphereComponent;
class UPaperFlipbook;

//非主流鼠
USTRUCT(BlueprintType)
struct FMissMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//默认
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def;
	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> ATK;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
	//闪避动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSoftObjectPtr<UPaperFlipbook>> MissAction;
};

UCLASS()
class FVM_API AMissMouse : public ANormalBase
{
	GENERATED_BODY()

public:
	AMissMouse();
	//老鼠初始化
	virtual void MouseInit() override;
	//更新
	virtual void MouseTick(const float& DeltaTime) override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin() override;
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd() override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//动画结束
	void AnimPlayEnd();
public:
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComp = nullptr;
	//老鼠动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMissMouseResourceStruct MouseAnim;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType CardCollisionType = ECardCollisionType::E_CardActor2;
private:
	UPROPERTY()
		UPaperFlipbook* CurPlayAnim = nullptr;

	bool bAtkLine = true;
	bool bMiss = false;
};
