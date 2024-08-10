// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Item/ShootLineComponent.h"
#include "SpineActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

// Sets default values for this component's properties
UShootLineComponent::UShootLineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UShootLineComponent::Over()
{
	this->SetComponentTickEnabled(false);
}

void UShootLineComponent::Move(const float& _Speed, const float& _MoveHight, float& _CurrentMoveHight)
{
	//提升高度
	if (_MoveHight > 0.f && _CurrentMoveHight < _MoveHight)
	{
		_CurrentMoveHight += _Speed;
		this->M_Owner->AddActorLocalOffset(FVector(0.f, 0.f, _Speed));

		if (_CurrentMoveHight < _MoveHight)
		{
			Cast<AFlyItemActor>(this->M_Owner)->SetCollisionEnabled(false);
		}
		else
		{
			Cast<AFlyItemActor>(this->M_Owner)->SetCollisionEnabled(true);
		}

	}
	else if (_MoveHight < 0.f && _CurrentMoveHight > _MoveHight)
	{
		_CurrentMoveHight -= _Speed;
		this->M_Owner->AddActorLocalOffset(FVector(0.f, 0.f, -_Speed), true);

		if (_CurrentMoveHight > _MoveHight)
		{
			Cast<AFlyItemActor>(this->M_Owner)->SetCollisionEnabled(false);
		}
		else
		{
			Cast<AFlyItemActor>(this->M_Owner)->SetCollisionEnabled(true);
		}
	}
}

void UShootLineComponent::SetMoveEnabled(bool _bMove)
{
	this->M_bMove = _bMove;
}



void UShootLineComponent::SetTargetNode(FTargetNode _FTargetNode)
{
	this->M_MoveNode = _FTargetNode;
}

const EShootDirection& UShootLineComponent::GetMoveDirection()
{
	return this->M_MoveNode.M_EShootDirection;
}

void UShootLineComponent::SetMoveDirection(const EShootDirection& _Direction)
{
	this->M_MoveNode.M_EShootDirection = _Direction;
}

// Called when the game starts
void UShootLineComponent::BeginPlay()
{
	Super::BeginPlay();

	this->M_Owner = Cast<ASpineActor>(this->GetOwner());
	if (!this->M_Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("[UShootLineComponent::BeginPlay]: 拥有者不是一个Actor"));
		this->SetComponentTickEnabled(false);
		return;
	}

}


// Called every frame
void UShootLineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->M_bMove)
	{
		switch (this->M_MoveNode.M_EShootDirection)
		{
		case EShootDirection::EUp:
			this->M_Owner->AddActorLocalOffset(
				FVector(0.f, 0.f, this->M_MoveNode.M_Speed * DeltaTime * this->M_Speed));
			break;

		case EShootDirection::EDown:
			this->M_Owner->AddActorLocalOffset(
				FVector(0.f, 0.f, -this->M_MoveNode.M_Speed * DeltaTime * this->M_Speed));
			break;

		case EShootDirection::ELeft:
		{
			this->M_Owner->AddActorLocalOffset(
				FVector(0.f, -this->M_MoveNode.M_Speed * DeltaTime * this->M_Speed, 0.f));
			//高度增加目前只支持上下不支持左右
			this->Move(
				this->M_MoveNode.M_Speed * DeltaTime * this->M_Speed,
				this->M_MoveNode.M_Hight,
				this->M_MoveNode.M_CurrentHight);
		}
		break;

		case EShootDirection::ERight:
		{
			this->M_Owner->AddActorLocalOffset(
				FVector(0.f, this->M_MoveNode.M_Speed * DeltaTime * this->M_Speed, 0.f));
			//高度增加目前只支持上下不支持左右
			this->Move(
				this->M_MoveNode.M_Speed * DeltaTime * this->M_Speed,
				this->M_MoveNode.M_Hight,
				this->M_MoveNode.M_CurrentHight
			);
		}
		break;

		//斜线
		case EShootDirection::ERotation:
			this->M_Owner->AddActorLocalOffset(
				FVector(0.f, this->M_MoveNode.M_Speed * DeltaTime * this->M_Speed, 0.f));
			break;
		}
	}

}

