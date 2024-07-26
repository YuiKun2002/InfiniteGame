// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Item/CruveMoveComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UCruveMoveComponent::UCruveMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCruveMoveComponent::Cruve_Init(FVector CurLocation, FVector TargetLocation, float CruveHight)
{
	this->M_CurrentLocation = CurLocation;
	this->Target = TargetLocation;
	this->MaxHight = CruveHight;
}

void UCruveMoveComponent::Curve_UpdateLocation(AActor* TargetActor, float CruveLocationTime, float CruveHightTime)
{
	if (!IsValid(TargetActor))
	{
		return;
	}

	FVector A = this->M_CurrentLocation;
	A.Z = CruveHightTime * this->MaxHight + this->M_CurrentLocation.Z;

	FVector B = this->Target;
	B.Z = CruveHightTime * this->MaxHight + this->Target.Z;

	TargetActor->SetActorLocation(UKismetMathLibrary::VLerp(A, B, CruveLocationTime));
}
