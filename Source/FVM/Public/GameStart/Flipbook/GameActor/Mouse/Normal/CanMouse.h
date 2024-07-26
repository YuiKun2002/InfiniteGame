// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "CanMouse.generated.h"

/**
 * 罐头鼠
 */

 //普通老鼠资源结构(陆地普通无任何装饰)->一般作为老鼠class的一个属性使用
USTRUCT(BlueprintType)
struct FCanMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def;
	//残血形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> DefATK;
	//正常形态(无罐头)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> NoDef;
	//残血形态(无罐头)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> NoDefATK;
	//正常形态(跳跃)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Jump;
	//(跳跃结束)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> JumpEnd;
	//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
		//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> NoDeath;
	//是否跳跃
	UPROPERTY()
		bool bJump = false;
};

UCLASS()
class FVM_API ACanMouse : public ANormalBase
{
	GENERATED_BODY()

public:
	ACanMouse();
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
	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//动画播放完毕
	void OnAnimationPlayEnd();
	//更新状态
	void UpdateState();
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_MesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BodyComponent = nullptr;
	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCanMouseResourceStruct AnimRes;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
private:
	bool bEnJump = false;
	bool bEnJumpEnd = true;
	bool bAttack = true;
};
