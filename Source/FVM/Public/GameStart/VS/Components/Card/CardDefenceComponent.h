// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardDefenceComponent.generated.h"

class ACardActor;
class ADefenceCardActor;
class UPaperFlipbook;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardDefenceComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UCardDefenceComponent();

	//更新状态
	UFUNCTION(BlueprintCallable)
		void UpdateState();

	//加载资源
	UFUNCTION(BlueprintCallable)
		void LoadResource();
public:
	//受到伤害时
	UFUNCTION()
		void BeHurt(class AMouseActor* CurMouseActor);
	//受到伤害时
	UFUNCTION()
		void HpChange(float ATK_Value, float UpHP_Value);
	//死亡时
	UFUNCTION()
		void Death();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//防御卡
	UPROPERTY()
		ADefenceCardActor* DefenceCardActor = nullptr;
};
