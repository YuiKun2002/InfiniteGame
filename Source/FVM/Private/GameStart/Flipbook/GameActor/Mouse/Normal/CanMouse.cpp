// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/CanMouse.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>

ACanMouse::ACanMouse()
{
	this->M_MesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalMouseMesheComponent"));
	this->M_BodyComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalMouseBodyComponent"));

	//设置依附
	this->M_MesheComponent->SetupAttachment(this->GetRootComponent());
	this->M_BodyComponent->SetupAttachment(this->M_MesheComponent);
}

void ACanMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACanMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bAttack)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime);
	}
}

void ACanMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->M_MesheComponent, this->M_BodyComponent);
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ACanMouse::OnAnimationPlayEnd);
}

void ACanMouse::MouseInit()
{
	Super::MouseInit();

	this->bEnJump = false;
	this->bEnJumpEnd = true;
	this->bAttack = true;
}

void ACanMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

	this->UpdateMove(DeltaTime);
}

void ACanMouse::AttackedBegin()
{
	Super::AttackedBegin();

	this->UpdateState();
}

void ACanMouse::AttackedEnd()
{
	Super::AttackedEnd();


	this->UpdateState();
}

void ACanMouse::BeAttakedBegin()
{
	Super::BeAttakedBegin();

	this->UpdateState();
}

bool ACanMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetCurrentHP() > 0.f)
	{
		if (AttackType == EFlyItemAttackType::Bomb && !this->AnimRes.bJump)
		{
			//启用跳跃
			this->AnimRes.bJump = true;
			this->bEnJump = true;
			this->bAttack = false;
			this->SetbIsAttack(false);
			this->MoveStart();
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Jump), true);
			//加速
			this->SetCurrentMoveSpeed(this->GetMoveSpeed() * 15.f);
		}
		else {

			if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
			{
				return false;
			}

			this->UpdateState();
		}
	}


	return true;
}

void ACanMouse::MouseDeathed()
{

	this->ClosedBoxComponent(this->M_MesheComponent);
	this->ClosedBoxComponent(this->M_BodyComponent);

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		if (this->AnimRes.bJump)
		{
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.NoDeath), true);
		}
		else {
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Death), true);
		}
	}
}

void ACanMouse::OnAnimationPlayEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	//跳跃动画结束
	if (this->bEnJump)
	{
		this->bEnJump = false;
		this->bEnJumpEnd = false;
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.JumpEnd), true);
		this->SetSpeedReset();
		this->MoveStop();
		return;
	}

	if (!this->bEnJumpEnd)
	{
		this->SetSpeedReset();
		this->SetbIsAttack(false);
		this->bEnJumpEnd = true;
		this->bAttack = true;
		this->MoveStart();
	}

	this->UpdateState();

	this->AddAttackCardUpdate();

}

void ACanMouse::UpdateState()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->bEnJump || !this->bEnJumpEnd)
	{
		return;
	}

	//重置速度
	this->SetSpeedReset();

	bool bAtk = this->GetbIsAttack();
	bool bJump = this->AnimRes.bJump;

	if (bJump)
	{
		if (bAtk)
		{
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.NoDefATK));
		}
		else {
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.NoDef));
		}
	}
	else {
		if (bAtk)
		{
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.DefATK));
		}
		else {
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Def));
		}
	}
}
