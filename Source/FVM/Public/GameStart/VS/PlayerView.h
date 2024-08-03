// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerView.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;
class UWidgetInteractionComponent;
class UAudioComponent;

UCLASS()
class FVM_API APlayerView : public APawn
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "PlayerView")
	USpringArmComponent* M_USpringArmComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "PlayerCollision")
	UBoxComponent* M_UBoxComponent = nullptr;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerView")
	UCameraComponent* M_UCameraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerViewMove")
	float M_PlayerViewMoveSpeed = 1.f;
public:
	// Sets default values for this pawn's properties
	APlayerView();

protected:

	UFUNCTION()
	void MoveUp(float _Value);

	UFUNCTION()
	void MoveRight(float _Value);

	UFUNCTION()
	void UpView(float _Value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
