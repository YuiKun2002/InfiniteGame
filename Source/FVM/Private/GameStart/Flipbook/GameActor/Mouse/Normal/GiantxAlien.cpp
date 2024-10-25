// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/GiantxAlien.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"

void AGiantxAlien::BeginPlay()
{
	Super::BeginPlay();

	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void AGiantxAlien::MouseInit()
{
	Super::MouseInit();

	this->bAttackLineEnable = true;
	this->State = -1;

	this->PlayWalk();
}

void AGiantxAlien::MouseDeathed()
{
	this->bAttackLineEnable = false;

	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Super::MouseDeathed();

	UTrackEntry* Trac = this->SetAnimation(0,
		this->M_DefAnim_Anim.DeadAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
	);
	BINDANIMATION(Trac, this, &AMouseActor::AlienDeadAnimationCompelet);
	this->SetTrackEntry(Trac);
}

void AGiantxAlien::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bAttackLineEnable)
	{
		this->AddAttackLineFunc(
			UGameSystemFunction::GetCardCollisionBoxType(this->MECardCollisionType),
			DeltaTime);
	}
}

void AGiantxAlien::AttackedBegin()
{
	Super::AttackedBegin();

	this->PlayAttack();
}

void AGiantxAlien::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void AGiantxAlien::AttackedEnd()
{
	Super::AttackedEnd();

	this->PlayWalk();
}

void AGiantxAlien::AnimPlayEnd(class UTrackEntry* Track)
{
	//攻击卡片【如果卡片死亡则会return】
	this->AddAttackCardUpdate();

	//创建范围攻击
	TArray<FHitResult> Hits;
	if (UGameSystemFunction::AddSphereTraceMulti(
		this,
		Hits,
		FVector::ZeroVector,
		FVector::ZeroVector,
		this->AttackRadius,
		UGameSystemFunction::GetCardCollisionTraceType(this->MECardCollisionType)
	))
	{
		for (const FHitResult& Hit : Hits)
		{
			//卡片
			ACardActor* Card = Cast<ACardActor>(Hit.GetActor());
			if (IsValid(Card))
			{
				Card->UpdateCardState(900000000);
				Card->KillCard();
				UResourceManagerComponent::ResourceAddBadCard();
			}
		}
	}
}

void AGiantxAlien::PlayWalk()
{
	if (this->State != 0)
	{
		this->State = 0;
		this->SetAnimation(
			0, this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
			true
		);
		this->SetTrackEntry(nullptr);
	}
}

void AGiantxAlien::PlayAttack()
{
	if (this->State != 1)
	{
		this->State = 1;
		UTrackEntry* Track = this->SetAnimation(
			0, this->M_DefAnim_Anim.AttackAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
			true
		);
		BINDANIMATION(Track, this, &AGiantxAlien::AnimPlayEnd);
		this->SetTrackEntry(Track);
	}
}
