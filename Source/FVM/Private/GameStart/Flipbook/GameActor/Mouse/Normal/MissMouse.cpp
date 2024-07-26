// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MissMouse.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "GameSystem/Tools/GameSystemFunction.h"

AMissMouse::AMissMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AMissMouse::MouseInit()
{
	Super::MouseInit();

	this->bAtkLine = true;
	this->bMiss = false;

	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMissMouse::AnimPlayEnd);
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMissMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bAtkLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->CardCollisionType), DeltaTime);
	}
}

void AMissMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void AMissMouse::AttackedBegin()
{
	Super::AttackedBegin();

	this->CurPlayAnim = UGameSystemFunction::LoadRes(MouseAnim.ATK);
	this->SetPlayAnimation(this->CurPlayAnim);
}

void AMissMouse::AttackedEnd()
{
	Super::AttackedEnd();

	this->CurPlayAnim = UGameSystemFunction::LoadRes(MouseAnim.Def);
	this->SetPlayAnimation(this->CurPlayAnim);
}

bool AMissMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return false;
	}

	if (Cast<AMouseActor>(CurHitMouseObj))
	{
		return Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);
	}

	//那些条件可以攻击到老鼠[爆炸，遇到控制类型的buff,处于攻击状态]
	if (
		AttackType == EFlyItemAttackType::Bomb
		||
		this->M_Buff->GetConstBuff()
		||
		this->GetbIsAttack()
		)
	{
		return Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);
	}
	else {
		if (!this->bMiss)
		{
			this->bAtkLine = false;
			this->bMiss = true;

			if (this->MouseAnim.MissAction.Num())
			{
				//随机播放动画
				int32 CurIndex = UGameSystemFunction::GetRandomRange(0, this->MouseAnim.MissAction.Num() - 1);
				this->CurPlayAnim = UGameSystemFunction::LoadRes(MouseAnim.MissAction[CurIndex]);
				this->SetPlayAnimation(this->CurPlayAnim);
			}

			this->MoveStop();
		}

		return false;
	}

	return true;
}

void AMissMouse::MouseDeathed()
{
	this->bAtkLine = false;
	this->bMiss = false;

	this->ClosedBoxComponent(this->MesheComp);
	this->ClosedBoxComponent(this->CollisionComp);

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(MouseAnim.Death), true);
	}
}

void AMissMouse::AnimPlayEnd()
{
	if (this->GetCurrentHP() > 0.f)
	{
		if (this->bMiss)
		{
			this->bMiss = false;
			this->bAtkLine = true;
			this->MoveStart();
		}
		else {
			this->AddAttackCardUpdate();
		}

		if (this->GetbIsAttack())
		{
			this->CurPlayAnim = UGameSystemFunction::LoadRes(MouseAnim.ATK);
		}
		else {
			this->CurPlayAnim = UGameSystemFunction::LoadRes(MouseAnim.Def);
		}

		this->SetPlayAnimation(this->CurPlayAnim);
	}
}
