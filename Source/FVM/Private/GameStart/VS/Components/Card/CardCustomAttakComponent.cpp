// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardCustomAttakComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/Flipbook/GameActor/CardActor/AttackCardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

// Sets default values for this component's properties
UCardCustomAttakComponent::UCardCustomAttakComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


}


// Called when the game starts
void UCardCustomAttakComponent::BeginPlay()
{
	Super::BeginPlay();

	//替换卡片类

	this->M_CardActor = Cast<AAttackCardActor>(this->GetSpineActor());
	this->MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
	if (!this->M_CardActor)
	{
		this->SetComponentTickEnabled(false);
	}
}

void UCardCustomAttakComponent::LoadResource()
{
	Super::LoadResource();

	this->M_Idle = UGameSystemFunction::LoadRes(this->M_CardActor->CardActor_DefAnim);
	this->M_Attack = UGameSystemFunction::LoadRes(this->M_CardActor->CardActor_AttackAnim);

	this->Pool = UObjectPoolManager::MakePoolManager(this->GetWorld(),
		this->M_CardActor->CardActor_BulletClassObj, 1
	);

	this->Pool->SetObjectPoolMaxCount(10);

	if (!Pool->InitComplete())
	{
		UE_LOG(LogTemp, Error,
			TEXT("[CardCustomAttakComponent.cpp UCardCustomAttakComponent::LoadResource] 飞行物加载失败，请检查路径 或者 后缀_C"));
	}

	//设置攻击投射属性
	this->InitLaunchProperty(
		this->M_CardActor->GetCurrentAttackCount(),
		this->M_CardActor->GetCurrentAttackSpeed(),
		this->M_CardActor->GetCurrentFristAttackDelay(),
		this->M_CardActor->GetCurrentSecondAttackDelay()
	);
}

void UCardCustomAttakComponent::Spawn()
{
	Super::Spawn();

	//获取对象池的对象
	//生成子弹

	if (!IsValid(this->Pool))
	{
		return;
	}

	AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(this->Pool->GetObjectActor());

	if (!IsValid(_TargetActor))
	{
		return;
	}

	//获取对象的变换位置
	const FTransform& _trans = this->M_CardActor->GetActorTransform();
	FTransform NewTrans;
	NewTrans.SetLocation(_trans.GetLocation());

	//AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(
	//	UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(), this->M_AFlyItemActor->GetClass(), NewTrans));
	////新生成的对象设置自定义拥有者(CardActor)
	//_TargetActor->M_CustomActorOwner = this->M_CardActor;
	//_TargetActor->SetMouseActorLocation(this->M_CardActor->GetCurrentMouse());
	//_TargetActor->SetATK(this->M_CardActor->GetCurrentATK());
	//_TargetActor->SetSecondATK(this->M_CardActor->GetCurrentSecondATK(this->M_CardActor->GetATKCardData().M_SputteringATKRate));
	//_TargetActor->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	//_TargetActor->SetLine(this->M_CardActor->GetLine().Row);
	//UGameplayStatics::FinishSpawningActor(_TargetActor, NewTrans);

	//新生成的对象设置自定义拥有者(CardActor)
	_TargetActor->SetActorTransform(NewTrans);
	_TargetActor->AddActorLocalOffset(_TargetActor->M_OffsetPosition);
	_TargetActor->M_CustomActorOwner = this->M_CardActor;
	_TargetActor->SetMouseActorLocation(this->M_CardActor->GetCurrentMouse());
	_TargetActor->SetATK(this->M_CardActor->GetCurrentATK());
	_TargetActor->SetSecondATK(
		this->M_CardActor->GetCurrentSecondATK(
			this->M_CardActor->GetATKCardData().M_SputteringATKRate)
	);
	_TargetActor->SetLine(this->M_CardActor->GetLine().Row);
	_TargetActor->Init();
	_TargetActor->OnInit();

}

void UCardCustomAttakComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->M_CardActor->SetAnimation(0, SpineCardAnimationState_Attack, true);
}

void UCardCustomAttakComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->M_CardActor->SetAnimation(0, SpineCardAnimationState_Idle, true);
}


void UCardCustomAttakComponent::BeginDestroy()
{
	Super::BeginDestroy();

	if (IsValid(this->Pool))
	{
		this->Pool->ClearAllObjectActor();
		this->Pool = nullptr;
	}
}

// Called every frame
void UCardCustomAttakComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//判断游戏是否暂停（游戏暂停则不能攻击）
	if (this->M_CardActor->GamePause())
	{
		return;
	}

	//每0.2m判断一次
	_tick_count += DeltaTime;
	if (_tick_count > 0.2f)
	{
		_tick_count = 0.f;

		this->SetAttackModEnabled(false);
		bool bResult = false;
		for (const FLineTraceSetting& CurLine : this->M_CardActor->GetSourceLineTraceSetting())
		{
			switch (CurLine.M_ELineTraceType)
			{
			case ELineTraceType::E_MouseGround: {
				if (IsValid(this->MouseManager->GetTopMouseByType(ELineType::OnGround)))
				{
					bResult = true;
				}
			}break;
			case ELineTraceType::E_MouseSky: {
				if (IsValid(this->MouseManager->GetTopMouseByType(ELineType::Sky)))
				{
					bResult = true;
				}
			}break;
			case ELineTraceType::E_MouseUnder: {
				if (IsValid(this->MouseManager->GetTopMouseByType(ELineType::Underground)))
				{
					bResult = true;
				}
			}break;
			case ELineTraceType::E_MouseGroundAndSky: {
				if (
					IsValid(this->MouseManager->GetTopMouseByType(ELineType::OnGround))
					||
					IsValid(this->MouseManager->GetTopMouseByType(ELineType::Sky))
					)
				{
					bResult = true;
				}
			}break;
			default:
			{
				if (IsValid(this->MouseManager->GetTopFirstMouse()))
				{
					bResult = true;
				}
			}break;
			}

			if (bResult)
			{
				break;
			}
		}

		if (bResult)
		{
			this->SetAttackModEnabled(true);
		}
	}
}

