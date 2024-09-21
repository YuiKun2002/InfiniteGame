// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "CardCustomAttakComponent.generated.h"

class AAttackCardActor;
class UPaperFlipbook;
class AFlyItemActor;
class UMouseManagerComponent;

//防御卡发射器-一旦出现老鼠立即检测
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardCustomAttakComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	AAttackCardActor* M_CardActor = nullptr;
	UPROPERTY()
	AFlyItemActor* M_AFlyItemActor = nullptr;
	UPROPERTY()
	UPaperFlipbook* M_Idle = nullptr;
	UPROPERTY()
	UPaperFlipbook* M_Attack = nullptr;
	UPROPERTY()
	UObjectPoolManager* Pool = nullptr;
private:
	UPROPERTY()
	float _tick_count = 0.f;
	UPROPERTY()
	UMouseManagerComponent* MouseManager = nullptr;
public:
	// Sets default values for this component's properties
	UCardCustomAttakComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	//销毁
	virtual void BeginDestroy() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	//资源加载
	virtual void LoadResource() override;
};
