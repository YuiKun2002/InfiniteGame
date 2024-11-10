// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalAlienByWidget.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>

void ANormalAlienByWidget::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞网格位置
	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void ANormalAlienByWidget::MouseInit()
{
	Super::MouseInit();

	this->bAttackLine = true;

	this->PlayAnim();
}

void ANormalAlienByWidget::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bAttackLine)
	{
		this->AddAttackLineFunc(
			UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime
		);
	}
}

void ANormalAlienByWidget::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void ANormalAlienByWidget::AttackedBegin()
{
	Super::AttackedBegin();

	this->PlayAnim();
}

void ANormalAlienByWidget::MoveingBegin()
{
	Super::MoveingBegin();

	this->PlayAnim();
}

void ANormalAlienByWidget::MouseDeathed()
{
	this->bAttackLine = false;
	//关闭碰撞
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

void ANormalAlienByWidget::OnAnimationPlayEnd(UTrackEntry* Track)
{
	//如果老鼠处于攻击状态
	if (this->GetbIsAttack())
	{
		//攻击防御卡
		if (!this->AttackCard())
		{
			//取消攻击
			this->SetbIsAttack(false);
		}
	}

	this->PlayAnim();
}

void ANormalAlienByWidget::PlayAnim()
{
	if (this->GetbIsAttack())
	{
		if (this->State != 0)
		{
			this->State = 0;

			UTrackEntry* Trac = this->SetAnimation(0,
				this->M_DefAnim_Anim.AttackAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
			);
			BINDANIMATION(Trac, this, &ANormalAlienByWidget::OnAnimationPlayEnd);
			this->SetTrackEntry(Trac);
		}
	}
	else {
		if (this->State != 1)
		{
			this->State = 1;

			UTrackEntry* Trac = this->SetAnimation(0,
				this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
			);
			this->SetTrackEntry(Trac);
		}
	}
}
