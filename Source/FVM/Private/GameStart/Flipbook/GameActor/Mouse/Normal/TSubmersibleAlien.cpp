// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/TSubmersibleAlien.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>

void ATSubmersibleAlien::BeginPlay()
{
	Super::BeginPlay();

	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void ATSubmersibleAlien::MouseInit()
{
	Super::MouseInit();

	this->bEnbaleAttackLine = false;

	this->PlayMove();
}

void ATSubmersibleAlien::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bEnbaleAttackLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->CheckCardType), DeltaTime, true);
	}
}

void ATSubmersibleAlien::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void ATSubmersibleAlien::AttackedBegin()
{
	Super::AttackedBegin();

	this->PlayAttack();
}

void ATSubmersibleAlien::MoveingBegin()
{
	Super::MoveingBegin();

	this->PlayMove();
}

void ATSubmersibleAlien::InMapMeshe(ELineType CurLineType)
{
	Super::InMapMeshe(CurLineType);
}

void ATSubmersibleAlien::MouseDeathed()
{
	this->bEnbaleAttackLine = false;
	this->ClosedBoxComponent(this->MesheComp);
	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//触发死亡
	Super::MouseDeathed();

	//播放死亡动画
	if (!this->GetPlayPlayBombEffAnim())
	{
		UTrackEntry* Trac = this->SetAnimation(0,
			this->M_DefAnim_Anim.DeadAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
		);
		BINDANIMATION(Trac, this, &AMouseActor::AlienDeadAnimationCompelet);
		this->SetTrackEntry(Trac);
	}
}

void ATSubmersibleAlien::AnimationPlayEnd(class UTrackEntry* Track)
{
	if (!this->GetMouseIsDeath())
	{
		this->AddAttackCardUpdate();
	}
}

void ATSubmersibleAlien::PlayMove()
{
	if (this->State != 0)
	{
		this->State = 0;

		this->SetAnimation(0,
			this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true);
		this->SetTrackEntry(nullptr);
	}
}

void ATSubmersibleAlien::PlayAttack()
{
	if (this->State != 0)
	{
		this->State = 0;

		UTrackEntry* Track = this->SetAnimation(0,
			this->M_DefAnim_Anim.AttackAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &ATSubmersibleAlien::AnimationPlayEnd);
		this->SetTrackEntry(nullptr);
	}
}
