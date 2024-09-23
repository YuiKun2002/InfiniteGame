// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/FVMGameInstance.h"
#include "Math/UnrealMathUtility.h"


// Sets default values for this component's properties
UCardLauncherComponent::UCardLauncherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

ASpineActor* const UCardLauncherComponent::GetSpineActor()
{
	return this->M_OwnerActor;
}

// Called when the game starts
void UCardLauncherComponent::BeginPlay()
{
	Super::BeginPlay();

	this->M_OwnerActor = Cast<ASpineActor>(this->GetOwner());

	if (!this->M_OwnerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]获取ASpineActor失败!"), *FString(__FUNCTION__));
		this->SetComponentTickEnabled(false);
		return;
	}

	/*this->AnimTrackEntry = this->M_OwnerActor->GetCurrentAnimationTrackEntry(0);
	this->AnimTrackEntry->AnimationComplete.AddDynamic(
		this, &UCardLauncherComponent::OnAnimationPlayEnd);*/
}

// Called every frame
void UCardLauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FVM_VS_GamePause())
	{
		return;
	}

	if (this->M_BeginAttackMod)
	{
		//攻击
		this->Attack(DeltaTime);
	}

	//生成投射物
	this->SpawnCardItem(DeltaTime);
}

void UCardLauncherComponent::TickCondition(float DeltaTime)
{
	if (this->M_BeginAttackMod)
	{
		//攻击
		this->Attack(DeltaTime);
	}

	//生成投射物
	this->SpawnCardItem(DeltaTime);
}

void UCardLauncherComponent::SetAttackModEnabled(bool _value)
{
	this->M_BeginAttackMod = _value;
}

void UCardLauncherComponent::OnAnimationPlayEnd()
{
	//攻击模式状态
	if (this->M_Condition.InAttack)
	{
		//时间记录
		this->M_Condition.LTime = 0.f;
		//攻击模式
		this->M_Condition.InAttack = false;
		//播放默认模式
		this->PlayIdleAnimation();
	}
}

void UCardLauncherComponent::SetTrackEntry(class UTrackEntry* Track)
{
	if (IsValid(this->AnimTrackEntry))
	{
		this->AnimTrackEntry->AnimationComplete.RemoveAll(this);
	}
	else {
		return;
	}

	this->AnimTrackEntry = nullptr;
	this->AnimTrackEntry = Track;
}

class UTrackEntry* UCardLauncherComponent::GetTrackEntry()
{
	return this->AnimTrackEntry;
}

//发射个数，攻击速度，首次攻击延迟，每次攻击延迟
void UCardLauncherComponent::InitLaunchProperty(int32 _LaunchCount, float _AttackDelay, float _FirstProjectionAttackDelay, float _ProjectionAttackDelay)
{
	this->M_LauncherCondition.M_ItemCount = _LaunchCount;
	this->M_LauncherCondition.M_AttackDelay = _AttackDelay;
	this->M_LauncherCondition.M_FirstProjectionDelay = _FirstProjectionAttackDelay;
	this->M_LauncherCondition.ProjectionDelay = _ProjectionAttackDelay;

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error,
			TEXT("[CardLauncherComponent.cpp UCardLauncherComponent::InitLaunchProperty]:发射器初始化：[个数%d] [攻击延迟:%.2f] [首次延迟:%.2f] [延迟:%.2f]"),
			_LaunchCount,
			_AttackDelay,
			_FirstProjectionAttackDelay,
			_ProjectionAttackDelay
		);
	}
}


void UCardLauncherComponent::LoadResource() {}

void UCardLauncherComponent::Attack(const float& DeltaTime, const FVector& _Location)
{
	//如果处于攻击模式则不再更新
	if (this->M_Condition.InAttack)
	{
		return;
	}

	//时间累计
	if (!this->M_Condition.InAttack)
	{
		this->M_Condition.LTime += DeltaTime;
	}

	//攻击模式冷却
	if (this->M_Condition.LTime >= this->M_LauncherCondition.M_AttackDelay)
	{
		//初始化
		this->M_Condition.Init();
		//初始化投射物生成个数
		this->M_Condition.SpawnItem = this->M_LauncherCondition.M_ItemCount;
		//开启攻击模式
		this->M_Condition.InAttack = true;
		//播放攻击动画
		this->PlayAttackAnimation();
	}
}

void UCardLauncherComponent::SpawnCardItem(const float& DeltaTime)
{
	//投射物模式启动
	if (this->M_Condition.SpawnItem > 0)
	{
		//发射投射物
		this->M_Condition.LSpawnTime += DeltaTime;

		// 第一次投射物时间
		if (
			!this->M_Condition.FirstProjectionEnd
			&&
			this->M_Condition.LSpawnTime > this->M_LauncherCondition.M_FirstProjectionDelay
			)
		{
			this->M_Condition.FirstProjectionEnd = true;
			this->SpawnOnce(this->M_Condition.SpawnItem);
			this->SpawnProjection();
		}
		else {
			//剩余投射物
			if (
				this->M_Condition.FirstProjectionEnd
				&&
				this->M_Condition.LSpawnTime >= this->M_LauncherCondition.ProjectionDelay
				)
			{
				this->SpawnProjection();
			}
		}

		//如果所有投射物生成完毕则关闭投射模式
		if (this->M_Condition.SpawnItem == 0)
		{
			//投射物生成个数
			this->M_Condition.SpawnItem = 0;
			//生成投射物的延迟
			this->M_Condition.LSpawnTime = 0.f;
			//首次投射物是否结束
			this->M_Condition.FirstProjectionEnd = false;
		}

	}
}

void UCardLauncherComponent::SpawnProjection()
{
	//生成投射物
	this->Spawn();
	//时间归零
	this->M_Condition.LSpawnTime = 0;
	//总个数减一
	this->M_Condition.SpawnItem--;
}

void UCardLauncherComponent::PlayAttackAnimation() {}
void UCardLauncherComponent::PlayIdleAnimation() {}
void UCardLauncherComponent::InitCondition() { this->M_Condition.Init(); }
void UCardLauncherComponent::Spawn() {}
void UCardLauncherComponent::SpawnOnce(const int32& SpawnCount) {}

