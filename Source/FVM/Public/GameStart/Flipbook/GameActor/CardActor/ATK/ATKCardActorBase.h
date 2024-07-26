// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "ATKCardActorBase.generated.h"

/**
 *  带有攻击力的卡片
 */

 //攻击类数据
USTRUCT(BlueprintType)
struct FATKCardActorLineTrace {
	GENERATED_USTRUCT_BODY()
public:
	//射线开始偏移(起点)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TraceBeginOffset = FVector(-1.f, 35.f, -30.f);
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLineTraceSetting> SourceLineTraceSettings;
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLineTraceSetting> LineTraceSettings;
};

UCLASS()
class FVM_API AATKCardActorBase : public ACardActor
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击力")
		float GetSourceATK();
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击力")
		float GetCurrentATK();
	UFUNCTION(BlueprintCallable, Category = "攻击|二段攻击力")
		float GetCurrentSecondATK(const float& TAKRate);
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击速度")
		float GetCurrentAttackSpeed();
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击次数")
		int32 GetCurrentAttackCount();
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击延迟1")
		float GetCurrentFristAttackDelay();
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击延迟2")
		float GetCurrentSecondAttackDelay();

	//获取线路检测结构
	UFUNCTION(BlueprintCallable, Category = "线路|获取检测")
		FATKCardActorLineTrace& GetLineTracePosition();
	//获取射线偏移
	UFUNCTION()
		const TArray<FLineTraceSetting>& GetLineTraceSetting();
	//获取原射线偏移【未计算】
	UFUNCTION()
		const TArray<FLineTraceSetting>& GetSourceLineTraceSetting() const;
public:
	UFUNCTION(BlueprintCallable, Category = "攻击|攻击力")
		void SetCurrentATK(const float& CurNewATK);
public:
	UFUNCTION(BlueprintCallable, Category = "线路|初始化检测")
		void InitLineTracePosition(const TArray<FLineTraceSetting>& InputLineTraceSetting);
	UFUNCTION(BlueprintCallable, Category = "线路|更新检测")
		void CalculateLineTracePosition(FATKCardActorLineTrace& InputATKCardActorLineTrace);
protected:
	//攻击力
	inline void SetATK(const float& ATKBase);
	inline float GetATK() const;

	inline void SetCurATK(const float& ATKBase);
	inline float GetCurATK() const;

	//攻击速度
	inline void SetAttackSpeed(const float& Speed);
	inline float GetAttackSpeed() const;

	inline void SetCurAttackSpeed(const float& Speed);
	inline float GetCurAttackSpeed() const;

	//攻击次数
	inline void SetAttackCount(const int32& Count);
	inline int32 GetAttackCount() const;

	//投射延迟
	inline void SetAttackDelay(const float& First, const float& Second);
	inline float GetFristAttackDelay() const;
	inline float GetSecondAttackDelay() const;
private:
	//卡片攻击力
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "攻击卡片数据")
		float ATK = 0.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "攻击卡片数据")
		float CurATK = 0.f;
	//卡片攻击速度
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "攻击卡片数据")
		float AttackSpeed = 0.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "攻击卡片数据")
		float CurAttackSpeed = 0.f;
	//投射次数
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "攻击卡片数据")
		int32 AttackCount = 0;
	//首次投射时间
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "攻击卡片数据")
		float FirstAttackTime = 0.f;
	//每次投射间隔
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "攻击卡片数据")
		float AttackEveryTime = 0.f;
	//线路检测
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "攻击卡片线路检测数据")
		FATKCardActorLineTrace ATKCardActorLineTrace;
};
