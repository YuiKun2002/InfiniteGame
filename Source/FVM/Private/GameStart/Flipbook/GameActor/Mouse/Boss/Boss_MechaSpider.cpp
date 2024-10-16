﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_MechaSpider.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

ABoss_MechaSpider::ABoss_MechaSpider()
{
	this->CheckLeftLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("CheckLeftLocationComp"));
	this->CheckRightLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("CheckRightLocationComp"));
}

void ABoss_MechaSpider::BeginPlay()
{
	Super::BeginPlay();

	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void ABoss_MechaSpider::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(this->AlienName, TEXT("577"));

	this->ChangeState(NewObject<UBoss_MechaSpider_Walk>());
}

void ABoss_MechaSpider::MouseDeathed()
{
	this->OnDead();

	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Super::MouseDeathed();

	UTrackEntry* Trac = this->SetAnimation(0,
		this->M_DefAnim_Anim.DeadAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
	);
	BINDANIMATION(Trac, this, &AMouseActor::AlienDeadAnimationCompelet);
	this->SetTrackEntry(Trac);
}

void UBoss_MechaSpider_Walk::Init()
{
	this->Boss = Cast<ABoss_MechaSpider>(this->Get());

	//播放走路动画
	this->PlayMoveAnim();
}

void UBoss_MechaSpider_Walk::MoveingUpdate(const float& DeltaTime)
{
	this->Boss->UpdateMove(DeltaTime);

	if (Time < 0.2f)
	{
		Time += DeltaTime;
	}
	else {
		Time = 0.f;
		this->CheckCard();
	}
}

void UBoss_MechaSpider_Walk::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	this->PlayMoveAnim();
}

void UBoss_MechaSpider_Walk::CheckCard()
{
	//获取网格
	UMesheControllComponent* LocalMesheComp =
		AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;

	//获取当前线路
	int32 CurLineZ = LocalMesheComp->GetMapMesh(
		this->Boss->GetMouseLine().Row, 0
	)->GetActorLocation().Z;

	//设置基本位置
	FVector LocationBegin = this->Boss->GetActorLocation() + FVector(
		this->Boss->CheckLeftLocationComp->GetRelativeLocation().Y,
		this->Boss->CheckLeftLocationComp->GetRelativeLocation().X,
		this->Boss->CheckLeftLocationComp->GetRelativeLocation().Z
	);
	LocationBegin.Z = CurLineZ;

	FVector LocationEnd = this->Boss->GetActorLocation() + FVector(
		this->Boss->CheckRightLocationComp->GetRelativeLocation().Y,
		this->Boss->CheckRightLocationComp->GetRelativeLocation().X,
		this->Boss->CheckRightLocationComp->GetRelativeLocation().Z
	);
	LocationEnd.Z = CurLineZ;

	//UE_LOG(LogTemp, Error, TEXT("%.2f %.2f %.2f"), LocationBegin.X, LocationBegin.Y, LocationBegin.Z);
	//UE_LOG(LogTemp, Error, TEXT("%.2f %.2f %.2f"), LocationEnd.X, LocationEnd.Y, LocationEnd.Z);

	//添加检测
	UGameSystemFunction::AddLineTrance(
		this->Boss->GetWorld(), LocationBegin, LocationEnd,
		UGameSystemFunction::GetCardCollisionBoxType(this->Boss->CheckCardType), this,
		[](UObject* GunAlienActor, AActor* CardActor) {
			if (IsValid(Cast<ACardActor>(CardActor)))
			{
				Cast<ACardActor>(CardActor)->SetCardCurrentHP(0.f);
				Cast<ACardActor>(CardActor)->KillCard();
				UResourceManagerComponent::ResourceAddBadCard();
			}
		}
	);
}

void UBoss_MechaSpider_Walk::PlayMoveAnim()
{
	if (this->Boss->GetCurrentHP() > this->Boss->GetTotalHP() * 0.5f)
	{
		if (this->State != 0)
		{
			//走路状态
			this->State = 0;
			//播放走路动画
			UTrackEntry* Track = this->Boss->SetAnimation(0,
				this->Boss->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
				true);
			this->Boss->SetTrackEntry(Track);
		}
	}
	else {
		if (this->State != 1)
		{
			//走路状态
			this->State = 1;
			//播放走路动画
			UTrackEntry* Track = this->Boss->SetAnimation(0,
				this->Boss->M_DefAnim_Anim.WalkAnimDamageRes.GetDefaultObject()->GetCategoryName().ToString(),
				true);
			this->Boss->SetTrackEntry(Track);
		}
	}
}
