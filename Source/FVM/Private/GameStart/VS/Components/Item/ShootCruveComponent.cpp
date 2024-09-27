// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Item/ShootCruveComponent.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UShootCruveComponent::UShootCruveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShootCruveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->M_Owner = Cast<AFlyItemActor>(this->GetOwner());
	if (!Cast<AFlyItemActor>(this->M_Owner))
	{
		UE_LOG(LogTemp, Error, TEXT("[ShootCurveComponent.cpp UShootCruveComponent::BeginPlay] 当前FlyActor无效"));
		this->SetComponentTickEnabled(false);
	}

}

void UShootCruveComponent::InitTarget()
{
	//因为Actor组件BeginPlay() 先于 Actor 本身
	this->M_CurrentLocation = this->M_Owner->GetActorLocation();
	this->Target = Cast<AFlyItemActor>(this->M_Owner)->GetObjectActorLocation();
}

FVector UShootCruveComponent::GetCurveLine(float FlyTime, float FlyTime_MaxHight)
{
	FVector A = this->M_CurrentLocation;
	A.Z = FlyTime_MaxHight * this->MaxHight + this->M_CurrentLocation.Z;

	FVector B = this->Target;
	B.Z = FlyTime_MaxHight * this->MaxHight + this->Target.Z;

	return UKismetMathLibrary::VLerp(A, B, FlyTime);
}

