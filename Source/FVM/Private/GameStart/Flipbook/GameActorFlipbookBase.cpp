// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>

// Sets default values
AGameActorFlipbookBase::AGameActorFlipbookBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->M_Actor = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("GameActor"));
	this->M_Position = CreateDefaultSubobject<USceneComponent>(TEXT("GameActorAttachPosition"));
	this->M_Location = CreateDefaultSubobject<USceneComponent>(TEXT("GameActorLocation"));

	this->SetRootComponent(this->M_Position);
	this->M_Location->SetupAttachment(this->M_Position);
	this->M_Actor->SetupAttachment(this->M_Location);

	//关闭碰撞
	this->M_Actor->SetGenerateOverlapEvents(false);
	this->M_Actor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//在蓝图层，动画单次播放结束时
	this->M_Actor->OnDynamicAnimationEndDelegate.AddDynamic(this, &AGameActorFlipbookBase::FuncDelegatePlayEnd);
}

// Called when the game starts or when spawned
void AGameActorFlipbookBase::BeginPlay()
{
	Super::BeginPlay();

	this->AddActorLocalOffset(this->M_OffsetPosition);



	//动画完全播放结束【如果动画没有绑定，才能绑定默认函数】
	if (!this->M_Actor->OnAnimationComplete.IsBound())
	{
		this->M_Actor->OnAnimationComplete.BindUObject(this,
			&AGameActorFlipbookBase::FuncDelegatePlayComplete);
	}
}


// Called every frame
void AGameActorFlipbookBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UFlipbookBaseComponent* AGameActorFlipbookBase::GetRenderComponent() const
{
	return this->M_Actor;
}

void AGameActorFlipbookBase::SetAnimationPlayEndDestroy()
{
	this->M_Actor->SetAnimationPlayEndDestroy();
}

void AGameActorFlipbookBase::SetPlayAnimationOnce(UPaperFlipbook* _PlayAnim, UPaperFlipbook* _PlayEndAnim)
{
	this->M_Actor->SetPlayAnimationOnce(_PlayAnim, _PlayEndAnim);
}

void AGameActorFlipbookBase::SetPlayAnimation(UPaperFlipbook* _PlayAnim, bool _bInfluence_AnimOnce)
{
	this->M_Actor->SetPlayAnimation(_PlayAnim, _bInfluence_AnimOnce);
}

UFlipbookBaseComponent* AGameActorFlipbookBase::GetMyActor()
{
	return this->M_Actor;
}

USceneComponent* AGameActorFlipbookBase::GetMyPosition()
{
	return this->M_Position;
}

USceneComponent* AGameActorFlipbookBase::GetGameLocation()
{
	return this->M_Location;
}

void AGameActorFlipbookBase::SetAnimationPlayRate(float _Value)
{
	this->GetRenderComponent()->SetPlayRate(_Value);
}

void AGameActorFlipbookBase::SetFlipbookColor(FVector _Color, float _Alpha)
{
	this->GetRenderComponent()->SetSpriteColor(FLinearColor(_Color.X, _Color.Y, _Color.Z, _Alpha));
}

void AGameActorFlipbookBase::SetFlipbookRotation(FRotator _rotation)
{
	this->M_Actor->SetRelativeRotation(_rotation);

	//两个坐标相同则无须交换
	if (this->M_Actor->GetRelativeLocation().X == this->M_Actor->GetRelativeLocation().Y)
	{
		return;
	}

	const float& _CurrentX = this->M_Actor->GetRelativeLocation().X;
	const float& _CurrentY = this->M_Actor->GetRelativeLocation().Y;
	const float& _CurrentZ = this->M_Actor->GetRelativeLocation().Z;

	this->M_Actor->SetRelativeLocation(FVector(_CurrentY, _CurrentX, _CurrentZ));
}

void AGameActorFlipbookBase::SetFlipbookPitchRotation(float _angle)
{
	this->M_Actor->SetRelativeRotation(FRotator(_angle, this->M_Actor->GetRelativeRotation().Yaw, this->M_Actor->GetRelativeRotation().Roll));
}

void AGameActorFlipbookBase::SetGameActorFlipbookRotation(FRotator _rotation)
{
	this->SetActorRelativeRotation(_rotation);
}

void AGameActorFlipbookBase::SetRenderLayer(int32 _Layer)
{
	this->CurActorTransLayer = _Layer;
	this->GetRenderComponent()->SetTranslucentSortPriority(_Layer);
}

int32 AGameActorFlipbookBase::GetTranslucentSortPriority() const
{
	return this->CurActorTransLayer;
}

float AGameActorFlipbookBase::GetFlipbookPitchRotation()
{
	return this->M_Actor->GetRelativeRotation().Pitch;
}

//初始化旋转(直接校准)注意：初始化后X与Y的坐标会进行交换
void AGameActorFlipbookBase::InitRotation()
{
	this->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
}

void AGameActorFlipbookBase::FuncDelegatePlayComplete()
{
	this->Destroy();
}

void AGameActorFlipbookBase::FuncDelegatePlayEnd()
{
	this->OnAnimationPlayEndBlueprint();
}

bool AGameActorFlipbookBase::IsPendingKill(AActor* _Actor)
{
	if (!_Actor)
		return true;

	return _Actor->IsPendingKillPending();
}
