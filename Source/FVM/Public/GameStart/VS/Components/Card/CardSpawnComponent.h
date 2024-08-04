// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "CardSpawnComponent.generated.h"

class ACardActor;
class AFlameActor;
class ASpawnCardActor;
class UPaperFlipbook;

#define SpineAnimationState_SpawnCard_Idle TEXT("Idle")
#define SpineAnimationState_SpawnCard_Spawn TEXT("Spawn")
#define SpineAnimationState_SpawnCard_Grow TEXT("Grow")

UENUM(BlueprintType)
enum class FCardSpawn : uint8 {
	_EIdle UMETA(DisplayName = "Idle"),
	_ESpawn UMETA(DisplayName = "Spawn"),
};

//资源数据
USTRUCT(BlueprintType)
struct FUCardSpawnComponent_Resource {
	GENERATED_USTRUCT_BODY()
public:
	//默认动画
	UPROPERTY(EditInstanceOnly, Category = "ResInstance")
		UPaperFlipbook* M_UPaperFlipbook_Idle = nullptr;
	//生产动画
	UPROPERTY(EditInstanceOnly, Category = "ResInstance")
		UPaperFlipbook* M_UPaperFlipbook_Spawn = nullptr;
	//生长动画
	UPROPERTY(EditInstanceOnly, Category = "ResInstance")
		UPaperFlipbook* M_UPaperFlipbook_Spawn_Up = nullptr;
};

//条件数据
USTRUCT(BlueprintType)
struct FUCardSpawnComponent_Condition {
	GENERATED_USTRUCT_BODY()
public:
	//当前卡片是否可以生长
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardProperty")
		bool M_CurrentCardIsWeekUp = false;
	//成长延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardProperty")
		float M_AutoUpTime = 0.f;
	//当前记录的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardProperty")
		float M_AutoUpCurrentTime = 0.f;
	//提升火苗产量的倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardProperty")
		float M_SpawnFlameNumRate = 2.f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardSpawnComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UCardSpawnComponent();

	//资源加载
	UFUNCTION(BlueprintCallable)
		virtual void LoadResource() override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//动画播放完毕
	virtual	void OnAnimationPlayEnd() override;
private:
	//当前生产卡片对象
	UPROPERTY()
		ASpawnCardActor* CurSpawnCardActor = nullptr;
private:
	//卡片的状态
	UPROPERTY(EditInstanceOnly, Category = "卡片状态")
		FCardSpawn M_FCardSpawn;
	//条件
	UPROPERTY(EditInstanceOnly, Category = "卡片状态 | 条件")
		FUCardSpawnComponent_Condition M_SpawnCondition;
	//动画资源实例
	UPROPERTY(EditInstanceOnly, Category = "卡片状态 | 动画")
		FUCardSpawnComponent_Resource M_Resource;
	//生产的火苗值
	UPROPERTY()
		int32 M_SpawnFlameValue = 25;
	//生长模式
	UPROPERTY()
		bool InWeekUp = false;
};
