// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GameStart/VS/Components/Card/AttackLauncherComponent.h"
#include "CardCustomAttakComponent.generated.h"

class AAttackCardActor;
class UPaperFlipbook;
class AFlyItemActor;
class UCardAttackComponent;
class UMouseManagerComponent;

//防御卡发射器-一旦出现老鼠立即检测
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardCustomAttakComponent : public UAttackLauncherComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UCardCustomAttakComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成子弹
	virtual void SpawnBullet(AFlyItemActor* NewBullet);
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//资源加载
	virtual void LoadResource() override;
	//当动画播放完毕时触发
	UFUNCTION()
	void OnAnimationComplete(class UTrackEntry* Track);
private:
	UPROPERTY()
	AAttackCardActor* M_CardActor = nullptr;
private:
	UPROPERTY()
	float _tick_count = 0.f;
	UPROPERTY()
	UMouseManagerComponent* MouseManager = nullptr;
};
