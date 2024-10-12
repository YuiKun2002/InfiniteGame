// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MachineMouse.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>


void AMachineMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bAttackLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->CheckCardType), DeltaTime, true);
	}
}

void AMachineMouse::BeginPlay()
{
	Super::BeginPlay();

	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void AMachineMouse::MouseInit()
{
	Super::MouseInit();

	this->bAttackLine = true;

	this->PlayWalk();
}

void AMachineMouse::AttackedBegin()
{
	Super::AttackedBegin();

	this->PlayAttack();
}

void AMachineMouse::AttackedEnd()
{
	Super::AttackedEnd();

	this->PlayWalk();
}

void AMachineMouse::MoveingBegin()
{
	Super::MoveingBegin();

	this->PlayWalk();
}

void AMachineMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void AMachineMouse::MouseDeathed()
{
	this->bAttackLine = false;

	//创建爆炸
	TArray<FHitResult> Hits;
	if (UGameSystemFunction::AddSphereTraceMulti(
		this, Hits, FVector::ZeroVector, FVector::ZeroVector, this->BombRadius,
		UGameSystemFunction::GetCardCollisionTraceType(CheckCardType)
	))
	{
		for (const FHitResult& Hit : Hits)
		{
			ACardActor* Card = Cast<ACardActor>(Hit.GetActor());
			if (IsValid(Card))
			{
				Card->UpdateCardState(9999999.f);
				Card->KillCard();
				UResourceManagerComponent::ResourceAddBadCard();
			}
		}
	}

	this->OnSpawnBomb();

	Super::MouseDeathed();

	this->ClosedBoxComponent(this->MesheComp);
	this->ClosedBoxComponent(this->BodyComp);

	if (!this->GetPlayPlayBombEffAnim())
	{
		UTrackEntry* Track = this->SetAnimation(0,
			this->M_DefAnim_Anim.DeadAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AMouseActor::AlienDeadAnimationCompelet);
	}
}

void AMachineMouse::AnimationPlayEnd(class UTrackEntry* Track)
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	this->AddAttackCardUpdate();
}

void AMachineMouse::PlayWalk()
{
	if (!this->GetbIsAttack())
	{
		if (this->State != 0)
		{
			this->State = 0;

			UTrackEntry* Track = this->SetAnimation(0,
				this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true);
			this->SetTrackEntry(Track);
		}
	}
}

void AMachineMouse::PlayAttack()
{
	if (this->GetbIsAttack())
	{
		if (this->State != 1)
		{
			this->State = 1;

			UTrackEntry* Track = this->SetAnimation(0,
				this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true);
			BINDANIMATION(Track, this, &AMachineMouse::AnimationPlayEnd);
			this->SetTrackEntry(Track);
		}
	}
}
