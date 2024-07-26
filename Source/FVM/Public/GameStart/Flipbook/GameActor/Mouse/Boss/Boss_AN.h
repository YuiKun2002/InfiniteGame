// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "Boss_AN.generated.h"

/**
 * 阿诺
 */

class UBoxComponent;
class USphereComponent;

UCLASS()
class FVM_API UAN_Out : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init();
	virtual void MouseTick(const float& DeltaTime);
	virtual void MoveingUpdate(const float& DeltaTime);
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType);
	virtual void AnimPlayEnd();
private:
	UPROPERTY()
		int32 RanCount = 5;
	UPROPERTY()
		UTimeLineClass* OutTimeLine = nullptr;
private:
	UFUNCTION()
		void RandomChangePosition();
};

UCLASS()
class FVM_API UAN_SkillSelect : public UBossStateBase
{
	GENERATED_BODY()
public:
	void Select(int32 MouseLevel);
};

UCLASS()
class FVM_API UAN_Move : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init();
	virtual void MouseTick(const float& DeltaTime);
	virtual void AnimPlayEnd();
public:
	UPROPERTY()
		int32 Level = 1;
	UPROPERTY()
		int32 PlayCount = 3;
private:
	UPROPERTY()
		bool bInitAnimEnd = false;
	UPROPERTY()
		UTimeLineClass* TimeLine = nullptr;
public:
	UPROPERTY()
		FVector CurLocation = FVector::ZeroVector;
	UPROPERTY()
		FVector TargetLocation = FVector::ZeroVector;
};

UCLASS()
class FVM_API UAN_Shoot : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init();
	virtual void MouseTick(const float& DeltaTime);
	virtual void AnimPlayEnd();
public:
	UPROPERTY()
		bool bRandom = false;
	UPROPERTY()
		int32 ShootCount = 1;
	UPROPERTY()
		UTimeLineClass* OutTimeLine = nullptr;
	UPROPERTY()
		bool bOut = false;
private:
	UFUNCTION()
		void Ran();
	UFUNCTION()
		void RanSelect(int32 GetCount, const TArray<AMapMeshe*>& InputMeshe, TArray<FVector>& OutLocation);
	UFUNCTION()
		void Def();
	UFUNCTION()
		void Out();
};

UCLASS()
class FVM_API UAN_Hidd : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init();
	virtual void MouseTick(const float& DeltaTime);
	virtual void AnimPlayEnd();
private:
	UPROPERTY()
		bool bHid = false;
	UPROPERTY()
		UTimeLineClass* TimeLine = nullptr;
	UPROPERTY()
		float HidTime = 10.f;
};

UCLASS()
class FVM_API ABoss_AN : public ABossBase
{
	GENERATED_BODY()
public:
	//网格的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMesh = nullptr;
	//身体的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBody = nullptr;
public:
	//出现动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> OutAnim;
	//射击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> ShootAnim;
	//射击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> ShootLowAnim;
	//移动准备
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> MoveReadyAnim;
	//移动动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> MoveAnim;
	//移动准备
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> MoveReadyLowAnim;
	//移动动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> LowAnim;
	//死亡动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画")
		TSoftObjectPtr<UPaperFlipbook> DeathAnim;
	//移动时的时间线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "移动曲线")
		TSoftObjectPtr<UCurveFloat> MouseMoveCurve;
	//出现时的缩放间线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "缩放曲线")
		TSoftObjectPtr<UCurveFloat> MouseOutScaleCurve;
	//出现时显示透明度放间线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "透明度曲线")
		TSoftObjectPtr<UCurveFloat> MouseOpacityCurve;
	//生成的老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "能力")
		TArray<FString> SpawnMouseNames;
public:
	ABoss_AN();
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
	//老鼠死亡
	virtual void MouseDeathed() override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//结束buff
	virtual void ExitBuff(EGameBuffTag BuffTag);
	//设置碰撞
	void SetCollision(bool bEnable);
public:
	//生成导弹
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnBullet(const FVector& CurLocation, const FVector& TargetLocation);
};
