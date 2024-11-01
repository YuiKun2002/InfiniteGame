// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "GameStart/VS/MapBaseType.h"
#include "Data/GameBuff.h"
#include "ElementSphere.generated.h"

/**
 * 元素球
 */
UCLASS()
class FVM_API AElementSphere : public ANormalSphereBase
{
	GENERATED_BODY()
public:
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
	//元素类型[冰]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGameBuffTag ElementType = EGameBuffTag::SlowDown;
	//遇到buff直接死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EGameBuffTag> BuffKill;
	//创建
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Create;
	//滚动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Rolling;
	//滚动残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RollingLow;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
public:
	//构造
	AElementSphere();
	//老鼠初始化
	virtual void MouseInit() override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin() override;
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd() override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//老鼠更新
	virtual void MouseTick(const float& DeltaTime) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
};
