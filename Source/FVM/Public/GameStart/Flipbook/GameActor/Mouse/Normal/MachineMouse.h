// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "MachineMouse.generated.h"

/**
 * 机器鼠
 */

USTRUCT(BlueprintType)
struct FMachineMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def;
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> DefLow;
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Idle;
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleLow;
	//死亡(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
};

//机器鼠爆炸动画
UCLASS()
class FVM_API AMachineBombAnim : public AGameActorFlipbookBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnInit();
};

UCLASS()
class FVM_API AMachineMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	AMachineMouse();
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//进入网格时
	virtual void InMapMeshe(ELineType CurLineType) override;
	//动画播放完毕
	void OnAnimationPlayEnd();
	//更新状态
	void UpdateState();
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* MMesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* MBodyComponent = nullptr;
	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMachineMouseResourceStruct AnimRes;
	//爆炸类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AMachineBombAnim> BombClass;
private:
	UPROPERTY()
		bool bBomb = false;
	UPROPERTY()
		float bTime = 0.2f;
	UPROPERTY()
		FVector Offset = FVector::ZeroVector;
};
