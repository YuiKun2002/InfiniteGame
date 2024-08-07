// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MatrixChildMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/MapMeshe.h"

AMatrixChildMouse::AMatrixChildMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AMatrixChildMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bAttack)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime);
	}
}

void AMatrixChildMouse::BeginPlay()
{
	Super::BeginPlay();
	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);
	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMatrixChildMouse::AnimPlayEnd);
}

void AMatrixChildMouse::MouseInit()
{
	Super::MouseInit();

	this->bCreateMouse = false;
	this->bAttack = false;
	this->AttackDelay = 0.1f;

	/*this->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->Anim.OnCreate)
	);*/

	this->SetAnimation(0, TEXT("SpineTag"), true);

	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMatrixChildMouse::MoveFunc(const float& DeltaTime)
{
	if (this->bCreateMouse)
	{
		//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

		this->UpdateMove(DeltaTime);
	}
}

void AMatrixChildMouse::AttackedBegin()
{
	Super::AttackedBegin();

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->SetAtk(true);
		this->GetMatrixGroup()->MoveStop();
		this->AtkBegin();
		this->GetMatrixGroup()->UpdateState(this);
	}
	else {
		this->UpdateAnim();
	}
}

void AMatrixChildMouse::AttackedEnd()
{
	Super::AttackedEnd();

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->GetMove();
		this->GetMatrixGroup()->SetAtk(this->GetMatrixGroup()->GetAtk());
		this->GetMatrixGroup()->UpdateState(this);
	}

	this->UpdateAnim();

	//UE_LOG(LogTemp, Error, TEXT("攻击结束"));

}

bool AMatrixChildMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	Super::BeHit(CurHitMouseObj,_HurtValue,AttackType);

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->GetMove();
		this->GetMatrixGroup()->SetAtk(this->GetMatrixGroup()->GetAtk());
		this->GetMatrixGroup()->UpdateState(this);
	}

	this->UpdateAnim();

	return true;
}

void AMatrixChildMouse::MouseDeathed()
{
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->bAttack = false;

	Super::MouseDeathed();

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->DeathMouse(this);
	}

	if (this->GetMouseDeathByBomb())
	{
		this->PlayBombEffAnim();
	}
	else {
		/*this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.IdleDeath), true
		);*/

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
}

void AMatrixChildMouse::AnimPlayEnd()
{
	if (!this->bCreateMouse)
	{
		this->bCreateMouse = true;
		this->bAttack = true;

		if (this->GetMatrixGroup())
		{
			if (!this->GetMatrixGroup()->GetbMove())
			{
				this->AtkBegin();
				return;
			}
		}
	}

	if (this->GetbIsAttack())
	{
		//攻击防御卡
		if (!this->AttackCard() && this->bAttack)
		{
			//取消攻击
			this->SetbIsAttack(false);
		}
	}

	this->UpdateAnim();
}

void AMatrixChildMouse::UpdateAnim()
{
	if (!this->bCreateMouse || this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->GetMatrixGroup())
	{
		if (this->GetMatrixGroup()->GetbMove())
		{
			this->MoveBegin();
		}
		else {
			this->AtkBegin();
		}

		return;
	}

	float Cur = this->GetCurrentHP();
	float Tot = this->GetTotalHP();
	bool bAtk = this->GetbIsAttack();

	if (Cur > Tot * 0.5f)
	{
		if (bAtk)
		{
			/*this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->Anim.Attack), true
			);*/

			this->SetAnimation(0, TEXT("SpineTag"), true);
		}
		else {
			/*this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->Anim.Walk)
			);*/

			this->SetAnimation(0, TEXT("SpineTag"), true);
		}
	}
	else {
		if (bAtk)
		{
			/*this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->Anim.AttackLow), true
			);*/

			this->SetAnimation(0, TEXT("SpineTag"), true);
		}
		else {
			/*this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->Anim.WalkLow)
			);*/

			this->SetAnimation(0, TEXT("SpineTag"), true);
		}
	}
}

void AMatrixChildMouse::AtkBegin()
{
	float Cur = this->GetCurrentHP();
	float Tot = this->GetTotalHP();

	if (Cur > Tot * 0.5f)
	{
		/*this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.Attack), true
		);*/

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else {
		/*this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.AttackLow), true
		);*/

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
}

void AMatrixChildMouse::MoveBegin()
{
	float Cur = this->GetCurrentHP();
	float Tot = this->GetTotalHP();

	if (Cur > Tot * 0.5f)
	{
		/*this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.Walk)
		);*/

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else {
		/*this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.WalkLow)
		);*/

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
}
