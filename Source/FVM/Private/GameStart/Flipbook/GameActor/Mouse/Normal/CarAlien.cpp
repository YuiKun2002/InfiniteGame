﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/CarAlien.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

ACarAlien::ACarAlien()
{
	this->CheckLeftLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("CheckLeftLocationComp"));
	this->CheckRightLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("CheckRightLocationComp"));
}

void ACarAlien::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞网格位置
	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void ACarAlien::MouseInit()
{
	Super::MouseInit();

	this->PlayMoveAnim();
}

void ACarAlien::BeAttakedBegin()
{
	Super::BeAttakedBegin();

	this->PlayMoveAnim();
}

void ACarAlien::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);

	if (Time < 0.2f)
	{
		Time += DeltaTime;
	}
	else {
		Time = 0.f;
		this->CheckCard();
	}
}

void ACarAlien::MouseDeathed()
{
	this->ClosedBoxComponent(this->MesheComp);
	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		UTrackEntry* Trac = this->SetAnimation(0,
			this->M_DefAnim_Anim.DeadAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
		);
		BINDANIMATION(Trac, this, &AMouseActor::AlienDeadAnimationCompelet);
		this->SetTrackEntry(Trac);
	}
}

void ACarAlien::CheckCard()
{
	//获取网格
	UMesheControllComponent* LocalMesheComp = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;

	//获取当前线路
	int32 CurLineZ = LocalMesheComp->GetMapMesh(
		this->GetMouseLine().Row, 0
	)->GetActorLocation().Z;

	//设置基本位置
	FVector LocationBegin = this->GetActorLocation() + FVector(
		this->CheckLeftLocationComp->GetRelativeLocation().Y,
		this->CheckLeftLocationComp->GetRelativeLocation().X,
		this->CheckLeftLocationComp->GetRelativeLocation().Z
	);
	LocationBegin.Z = CurLineZ;

	FVector LocationEnd = this->GetActorLocation() + FVector(
		this->CheckRightLocationComp->GetRelativeLocation().Y,
		this->CheckRightLocationComp->GetRelativeLocation().X,
		this->CheckRightLocationComp->GetRelativeLocation().Z
	);
	LocationEnd.Z = CurLineZ;

	//添加检测
	UGameSystemFunction::AddLineTrance(
		this->GetWorld(), LocationBegin, LocationEnd,
		UGameSystemFunction::GetCardCollisionBoxType(this->CheckCardType), this,
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

void ACarAlien::PlayMoveAnim()
{
	if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		if (this->State != 0)
		{
			//走路状态
			this->State = 0;
			//播放走路动画
			UTrackEntry* Track = this->SetAnimation(0,
				this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
				true);
			this->SetTrackEntry(Track);
		}
	}
	else {
		if (this->State != 1)
		{
			//走路状态
			this->State = 1;
			//播放走路动画
			UTrackEntry* Track = this->SetAnimation(0,
				this->M_DefAnim_Anim.WalkAnimDamageRes.GetDefaultObject()->GetCategoryName().ToString(),
				true);
			this->SetTrackEntry(Track);
		}
	}
}
