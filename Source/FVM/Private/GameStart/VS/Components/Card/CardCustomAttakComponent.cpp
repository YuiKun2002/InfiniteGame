﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardCustomAttakComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include "SpineSkeletonAnimationComponent.h"
#include "GameStart/Flipbook/GameActor/CardActor/AttackCardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

// Sets default values for this component's properties
UCardCustomAttakComponent::UCardCustomAttakComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCardCustomAttakComponent::BeginPlay()
{
	Super::BeginPlay();

	//替换卡片类

	this->M_CardActor = Cast<AAttackCardActor>(this->GetSpineActor());
	this->MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
	if (!this->M_CardActor)
	{
		this->SetComponentTickEnabled(false);
	}
}

void UCardCustomAttakComponent::LoadResource()
{
	Super::LoadResource();

	//设置攻击投射属性
	this->InitLaunchProperty(
		this->M_CardActor->GetCurrentAttackCount(),
		this->M_CardActor->GetCurrentAttackSpeed(),
		this->M_CardActor->GetCurrentFristAttackDelay(),
		this->M_CardActor->GetCurrentSecondAttackDelay()
	);

	//初始化默认子弹
	this->InitLaunchBulletByDef(this->M_CardActor->CardActor_BulletClassObj);

	//播放发呆动画
	this->M_CardActor->SetAnimation(0, this->GetIdleAnimName(), true);
	this->SetTrackEntry(nullptr);
}


void UCardCustomAttakComponent::OnAnimationComplete(class UTrackEntry* Track)
{
	this->OnAnimationPlayEnd();
	this->SetTrackEntry(nullptr);
}

void UCardCustomAttakComponent::SpawnBullet(AFlyItemActor* NewBullet)
{
	//获取对象的变换位置
	FTransform NewTrans;
	NewTrans.SetLocation(this->M_CardActor->GetBulletLauncherLocation());

	//新生成的对象设置自定义拥有者(CardActor)
	NewBullet->SetActorTransform(NewTrans);
	NewBullet->SetObjectActorLocation(this->M_CardActor->GetCurrentMouse());
	NewBullet->SetATK(this->M_CardActor->GetCurrentATK());
	NewBullet->SetSecondATK(
		this->M_CardActor->GetCurrentSecondATK(
			this->M_CardActor->GetATKCardData().M_SputteringATKRate)
	);
	NewBullet->SetLine(this->M_CardActor->GetLine().Row);
	NewBullet->Init();
	
}

void UCardCustomAttakComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	//播放动画
	UTrackEntry* Track = this->M_CardActor->SetAnimation(0, this->GetAttackAnimName(), true);
	//绑定动画事件
	Track->AnimationComplete.AddDynamic(
		this, &UCardCustomAttakComponent::OnAnimationComplete);
	this->SetTrackEntry(Track);
}

void UCardCustomAttakComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->M_CardActor->SetAnimation(0, this->GetIdleAnimName(), true);
}

// Called every frame
void UCardCustomAttakComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//判断游戏是否暂停（游戏暂停则不能攻击）
	if (this->M_CardActor->GamePause())
	{
		return;
	}

	//每0.2m判断一次
	_tick_count += DeltaTime;
	if (_tick_count > 0.2f)
	{
		_tick_count = 0.f;

		this->SetAttackModEnabled(false);
		bool bResult = false;
		for (const FLineTraceSetting& CurLine : this->M_CardActor->GetSourceLineTraceSetting())
		{
			switch (CurLine.M_ELineTraceType)
			{
			case ELineTraceType::E_MouseGround: {
				if (IsValid(this->MouseManager->GetTopMouseByType(ELineType::OnGround)))
				{
					bResult = true;
				}
			}break;
			case ELineTraceType::E_MouseSky: {
				if (IsValid(this->MouseManager->GetTopMouseByType(ELineType::Sky)))
				{
					bResult = true;
				}
			}break;
			case ELineTraceType::E_MouseUnder: {
				if (IsValid(this->MouseManager->GetTopMouseByType(ELineType::Underground)))
				{
					bResult = true;
				}
			}break;
			case ELineTraceType::E_MouseGroundAndSky: {
				if (
					IsValid(this->MouseManager->GetTopMouseByType(ELineType::OnGround))
					||
					IsValid(this->MouseManager->GetTopMouseByType(ELineType::Sky))
					)
				{
					bResult = true;
				}
			}break;
			default:
			{
				if (IsValid(this->MouseManager->GetTopFirstMouse()))
				{
					bResult = true;
				}
			}break;
			}

			if (bResult)
			{
				break;
			}
		}

		if (bResult)
		{
			this->SetAttackModEnabled(true);
		}
	}
}

