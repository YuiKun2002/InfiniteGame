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
class FVM_API AFlyCatchMouse : public ANormalCapsuleBase
{
	GENERATED_BODY()
public:
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//动画播放完毕
	UFUNCTION()
	void AnimationPlayEnd(class UTrackEntry* Track);
public:
	//卡片动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ASpineActor* CurCardAnim = nullptr;
private:
	//当前抓去的卡片
	UPROPERTY()
		class ACardActor* CurCatchCard = nullptr;
	//对应的抓去位置
	UPROPERTY()
		class UUI_MapMeshe* CurUI = nullptr;
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
