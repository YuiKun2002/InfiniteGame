// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Item/AutoTrackShootComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UAutoTrackShootComponent::UAutoTrackShootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAutoTrackShootComponent::BeginPlay()
{
	Super::BeginPlay();
	//初始化飞行物
	this->M_Owner = Cast<AFlyItemActor>(this->GetOwner());
	if (this->M_Owner)
	{
		// 初始化老鼠管理器
		this->M_MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
	}
	else {
		this->SetComponentTickEnabled(false);
		UE_LOG(LogTemp, Error,
			TEXT("[AutoTrackShootComponent.cpp UAutoTrackShootComponent::BeginPlay]飞行物Actor获取失败!"));
	}
}


// Called every frame
void UAutoTrackShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bStop)
	{
		return;
	}

	if (this->_TickTime < 0.01f)
	{
		this->_TickTime += DeltaTime;
	}
	else {
		this->_TickTime = 0.f;

		AMouseActor* CurTop = nullptr;
		for (const ELineType& LineType : this->M_Owner->GetAttackType())
		{
			AMouseActor* TempTop = this->M_MouseManager->GetTopMouseByType(LineType);
			if (IsValid(TempTop) && TempTop->GetCurrentHP() > 0.f)
			{
				this->bTarget = true;

				if (!IsValid(CurTop))
				{
					CurTop = TempTop;
				}
				else {
					if (TempTop->GetActorLocation().Y < CurTop->GetActorLocation().Y)
					{
						CurTop = TempTop;
					}
				}
			}
		}

		if (IsValid(CurTop) && CurTop != this->M_CurrentMouse)
		{
			this->_AutoTrackTickTime = 0.f;
			this->M_CurrentMouse = CurTop;
			this->M_OwnerLocation = this->M_Owner->GetActorLocation();
		}

		if (
			(!IsValid(this->M_CurrentMouse)) || (IsValid(this->M_CurrentMouse)
				&&
				this->M_CurrentMouse->GetCurrentHP() <= 0.f)
			)
		{
			this->_AutoTrackTickTime = 0.f;
			this->bTarget = false;
			this->M_Owner->AddActorLocalOffset(FVector(0.f, DeltaTime * 150.f, 0.f), true);
			this->M_OwnerLocation = this->M_Owner->GetActorLocation();
			return;
		}


		if (this->M_CurrentMouse->GetCurrentHP() > 0.f)
		{
			this->bTarget = true;

			//时间
			this->_AutoTrackTickTime += DeltaTime * 2.f;

			//追踪
			const FVector& _V = UKismetMathLibrary::VLerp(
				this->M_OwnerLocation,
				this->M_CurrentMouse->GetActorLocation() + FVector(0.f, 0.f, 15.f),
				this->_AutoTrackTickTime
			);

			//设置坐标
			this->M_Owner->SetActorLocation(_V, true);

			if (this->_AutoTrackTickTime >= 1.f)
			{
				this->_AutoTrackTickTime = 1.f;

				FHitResult CurHit;
				if (UGameSystemFunction::AddSphereTrace(this->M_Owner, CurHit, FVector::ZeroVector,
					FVector::ZeroVector, 15.f,
					UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType::MouseActor)))
				{
					this->M_Owner->HitMouse_OverlapBegin(CurHit.GetActor());
				}
				else {
					this->M_CurrentMouse = nullptr;
					this->M_Owner->ReturnPool();
				}
			}
		}

		//this->M_OwnerLocation = this->M_Owner->GetActorLocation();
	}
}

void UAutoTrackShootComponent::Stop()
{
	this->bStop = true;
}

void UAutoTrackShootComponent::Start()
{
	this->bStop = false;
}

void UAutoTrackShootComponent::InitLocation()
{
	// 初始化老鼠管理器
	this->M_MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
	//获取坐标
	this->M_OwnerLocation = this->M_Owner->GetActorLocation();
	this->_AutoTrackTickTime = 0.f;
}

bool UAutoTrackShootComponent::GetTarget()
{
	return this->bTarget;
}

