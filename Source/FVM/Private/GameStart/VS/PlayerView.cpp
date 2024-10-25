// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/PlayerView.h"

#include <Components/BoxComponent.h>
#include <Components/WidgetInteractionComponent.h>

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

// Sets default values
APlayerView::APlayerView()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->M_UBoxComponent = this->CreateDefaultSubobject<UBoxComponent>(TEXT("M_UBoxComponent"));
	this->M_UCameraComponent = this->CreateDefaultSubobject<UCameraComponent>(TEXT("M_UCameraComponent"));
	this->M_USpringArmComponent = this->CreateDefaultSubobject<USpringArmComponent>(TEXT("M_USpringArmComponent"));

	this->SetRootComponent(this->M_UBoxComponent);
	this->M_USpringArmComponent->SetupAttachment(this->M_UBoxComponent);
	this->M_UCameraComponent->SetupAttachment(this->M_USpringArmComponent);
}

// Called when the game starts or when spawned
void APlayerView::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerView::MoveUp(float _Value)
{
	if (_Value != 0.f)
	{
		this->AddActorLocalOffset(FVector(0.f, 0.f, _Value * this->M_PlayerViewMoveSpeed));
	}
}

void APlayerView::MoveRight(float _Value)
{
	if (_Value != 0.f)
	{
		this->AddActorLocalOffset(FVector(0.f, _Value * this->M_PlayerViewMoveSpeed, 0.f));
	}
}

void APlayerView::UpView(float _Value)
{
	if (_Value != 0.f)
	{
		this->M_UCameraComponent->SetFieldOfView(this->M_UCameraComponent->FieldOfView + _Value);
	}
}

// Called to bind functionality to input
void APlayerView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("PlayerViewMoveUp", this, &APlayerView::MoveUp);
	PlayerInputComponent->BindAxis("PlayerViewMoveRight", this, &APlayerView::MoveRight);

	PlayerInputComponent->BindAxis("PlayerViewUp", this, &APlayerView::UpView);
}