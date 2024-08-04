// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "FlyCatchMouse.generated.h"

/**
 * 蝙蝠鼠
 */

class UBoxComponent;
class USphereComponent;

//标记
UCLASS()
class FVM_API AFlyCatchMouseFlag : public AGameActorFlipbookBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FFlyCatchMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def;
	//抓去形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Catch;
	//离场形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Exit;
	//标志
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AFlyCatchMouseFlag> FlagClass;
};

UCLASS()
class FVM_API AFlyCatchMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	AFlyCatchMouse();
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
	//动画播放完毕
	void OnAnimationPlayEnd();
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBodyComponent = nullptr;
	//卡片动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ASpineActor* CurCardAnim = nullptr;
	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FFlyCatchMouseResourceStruct AnimRes;
private:
	//当前抓去的卡片
	UPROPERTY()
		class ACardActor* CurCatchCard = nullptr;
	//对应的抓去位置
	UPROPERTY()
		class UUI_MapMeshe* CurUI = nullptr;
	//标记
	UPROPERTY()
		AFlyCatchMouseFlag* CurFlag = nullptr;
	//延迟出现的时间
	UPROPERTY()
		float DealyTime = 3.f;
	//落地时间
	UPROPERTY()
		float fToGroundTime = 1.f;
	//是否可以出现
	UPROPERTY()
		bool bEnter = false;
	//退场
	UPROPERTY()
		bool bExit = false;
	//当前老鼠的位置
	UPROPERTY()
		FVector CurLocation = FVector::ZeroVector;
	//当前退场老鼠的位置
	UPROPERTY()
		FVector CurExitLocation = FVector::ZeroVector;
};
