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

	this->bEnbaleAttackLine = true;

	this->PlayMove();
}

void ATSubmersibleAlien::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bEnbaleAttackLine)
	{
		this->AddAttackLineFunc(
			UGameSystemFunction::GetCardCollisionBoxType(this->CheckCardType),
			DeltaTime,
			true);
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

	if (this->GetMouseLineType() == CurLineType)
	{
		return;
	}

	this->State = -1;

	//水上
	if (CurLineType == ELineType::OnWater)
	{
		this->GetMouseManager()->ChangeMouseLineType(
			this, this->GetMouseLine().Row, ELineType::OnWater, this->MesheComp,
			this->BodyComp
		);
	}
	else {
		//陆地
		this->GetMouseManager()->ChangeMouseLineType(
			this, this->GetMouseLine().Row, ELineType::OnGround, this->MesheComp,
			this->BodyComp
		);
	}

	if (this->GetbIsAttack())
	{
		this->PlayAttack();
	}
	else {
		this->PlayMove();
	}
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

		TSubclassOf<class UAssetCategoryName> WalkName = this->M_DefAnim_Anim.WalkAnimRes;
		if (this->GetMouseLineType() == ELineType::OnWater)
		{
			WalkName = this->M_DefAnim_Anim.WalkAnimDamageRes;

			this->GetMouseManager()->ChangeMouseLineType(
				this, this->GetMouseLine().Row, ELineType::OnWater, this->MesheComp,
				this->BodyComp
			);
		}

		//播放走路动画
		UTrackEntry* Track = this->SetAnimation(0,
			WalkName.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		this->SetTrackEntry(Track);
	}
}

void ATSubmersibleAlien::PlayAttack()
{
	if (this->State != 1)
	{
		this->State = 1;

		TSubclassOf<class UAssetCategoryName> AttackName = this->M_DefAnim_Anim.AttackAnimRes;
		if (this->GetMouseLineType() == ELineType::OnWater)
		{
			AttackName = this->M_DefAnim_Anim.AttackAnimDamageRes;

			this->GetMouseManager()->ChangeMouseLineType(
				this, this->GetMouseLine().Row, ELineType::OnGround, this->MesheComp,
				this->BodyComp
			);

		}

		UTrackEntry* Track = this->SetAnimation(0,
			AttackName.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &ATSubmersibleAlien::AnimationPlayEnd);
		this->SetTrackEntry(Track);
	}
}
