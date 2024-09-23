// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "Components/ActorComponent.h"
#include "CardLauncherComponent.generated.h"

//发射器属性
USTRUCT(BlueprintType)
struct FLauncherCondition {
	GENERATED_USTRUCT_BODY()
public:
	//一共发射多少次
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_ItemCount = 1;
	//攻击延迟(从卡片默认状态到攻击状态直到开启首次投射模式的时间)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_AttackDelay = 3.5f;
	//首次投射延迟时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_FirstProjectionDelay = 1.f;
	//投射时间间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectionDelay = 0.3f;
};

//发射器条件属性
USTRUCT(BlueprintType)
struct FLauncherComponentCondition {
	GENERATED_USTRUCT_BODY()
public:
	//时间记录
	float LTime = 0.f;
	//生成投射物的延迟
	float LSpawnTime = 0.f;
	//攻击模式
	bool InAttack = false;
	//首次投射物是否结束
	bool FirstProjectionEnd = false;
	//投射物生成个数
	int32 SpawnItem = 0;
public:
	void Init() {
		//时间记录
		this->LTime = 0.f;
		//生成投射物的延迟
		this->LSpawnTime = 0.f;
		//攻击模式
		this->InAttack = false;
		//投射物生成个数
		this->SpawnItem = 0;
		//首次投射物是否结束
		this->FirstProjectionEnd = false;
	}
};


////发射器组件Actor
//UCLASS()
//class FVM_API ALauncherComponentActor : public AActor {
//	GENERATED_BODY()
//private:
//	//条件
//	UPROPERTY()
//	FLauncherComponentCondition  LauncherComponentCondition;
//	//发射器条件
//	UPROPERTY()
//	FLauncherCondition LauncherCondition;
//};


//发射器组件
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardLauncherComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//条件
	UPROPERTY()
	FLauncherComponentCondition M_Condition;
	//发射器条件
	UPROPERTY()
	FLauncherCondition M_LauncherCondition;
public:
	// Sets default values for this component's properties
	UCardLauncherComponent();
	//获取Actor Base
	ASpineActor* const GetSpineActor();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickCondition(float DeltaTime);

	//资源加载
	UFUNCTION(BlueprintCallable)
	virtual void LoadResource();

	//设置攻击模式的开启和关闭
	UFUNCTION(BlueprintCallable)
	void SetAttackModEnabled(bool _value);
	//设置动画轨道
	UFUNCTION()
	void SetTrackEntry(class UTrackEntry* Track);
	//获取动画轨道
	UFUNCTION()
	class UTrackEntry* GetTrackEntry();
	//设置发射的属性
	UFUNCTION(BlueprintCallable)
	void InitLaunchProperty(
		int32 _LaunchCount,
		float _AttackDelay,
		float _FirstProjectionAttackDelay,
		float _ProjectionAttackDelay
	);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void Spawn();
	//并发投射生成
	virtual void SpawnOnce(const int32& SpawnCount);
	//播放攻击动画
	virtual void PlayAttackAnimation();
	//播放默认动画
	virtual void PlayIdleAnimation();
	//初始化攻击条件时间等
	virtual void InitCondition();
	//当动画结束时
	virtual	void OnAnimationPlayEnd();
private:
	//攻击
	void Attack(const float& DeltaTime, const FVector& _Location = FVector::ZeroVector);
	//生成投射物
	void SpawnCardItem(const float& DeltaTime);
	//生成
	void SpawnProjection();
private:
	UPROPERTY()
	ASpineActor* M_OwnerActor = nullptr;
	UPROPERTY()
	class UTrackEntry* AnimTrackEntry = nullptr;
private:
	//攻击模式
	bool M_BeginAttackMod = false;
};
