// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/JumpStateMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/MapMeshe.h"

AJumpStateMouse::AJumpStateMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AJumpStateMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bAttack)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime);
	}
}

void AJumpStateMouse::AttackedBegin()
{
	this->UpdateState();
}

void AJumpStateMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->CheckCard(DeltaTime);

	if (this->bJumpStart && !this->bJumpEnd)
	{
		//跳跃的过程中不能进行强行移动
		this->DisEnableForceMove();
		this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed * 2.5f, 0.f));
	}
	else {
		//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

		this->UpdateMove(DeltaTime);
	}
}

bool AJumpStateMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);

	this->UpdateState();

	return true;
}

void AJumpStateMouse::MouseDeathed()
{
	//取消攻击
	this->bAttack = false;
	//取消碰撞
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();

	if (this->GetMouseDeathByBomb())
	{
		this->PlayBombEffAnim();
	}
	else {
		this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->JumpAnim.Death), true
		);
	}


	Super::MouseDeathed();
}

void AJumpStateMouse::AnimPlayEnd()
{
	if (this->bJumpStart && !this->bJumpEnd)
	{
		this->bJumpEnd = true;

		this->bAttack = true;

		this->CheckDelay = 0.f;

		this->MoveStart();

		this->SetMoveSpeed(this->GetMoveSpeed() / 2.3f);
	}

	//如果老鼠处于攻击状态
	if (this->GetbAttackState())
	{
		//攻击防御卡
		if (!this->AttackCard())
		{
			//取消攻击
			this->SetbIsAttack(false);
		}
	}

	if (!this->GetMouseIsDeath())
	{
		this->UpdateState();
	}
}


bool AJumpStateMouse::CheckCard(const float& DeltaTime)
{
	if (this->bJumpStart)
	{
		return false;
	}

	if (this->CheckDelay > 0.f)
	{
		this->CheckDelay -= DeltaTime;
		return false;
	}
	else {
		this->CheckDelay = 0.2f;
	}

	FVector Offset = FVector::ZeroVector;
	if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
	{
		Offset = FVector(0.f, 0.f, 10.f);
	}
	FHitResult Ht;
	if (UGameSystemFunction::AddSphereTrace(
		this, Ht, Offset, Offset, 10.f,
		UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor2)
	))
	{
		ACardActor* CurCard = Cast<ACardActor>(Ht.GetActor());
		if (IsValid(CurCard))
		{
			this->bJumpStart = true;
			for (const auto& Card : CurCard->GetUIMapMesh()->GetCardDatas())
			{
				if (IsValid(Card.Value) && Card.Value->GetCardData().CardCategoryTag == ECardCategoryTag::HighBlock)
				{
					//遇到高墙
					this->bJumpPass = false;
					//停止移动
					this->MoveStop();
					break;
				}
			}

			//更具当前血量播放跳跃动画
			if (this->GetCurrentHP() > this->GetTotalHP() * 0.6)
			{
				this->SetPlayAnimation(
					UGameSystemFunction::LoadRes(this->JumpAnim.Jump)
				);
			}
			else {
				if (this->GetCurrentHP() > 0.f)
				{
					this->SetPlayAnimation(
						UGameSystemFunction::LoadRes(this->JumpAnim.JumpLow)
					);
				}
			}

			return true;
		}
	}

	return false;
}

void AJumpStateMouse::UpdateState()
{
	//如果在跳跃的过程中受到攻击
	if (this->bJumpStart && !this->bJumpEnd)
	{
		return;
	}

	float Cur = this->GetCurrentHP();
	if (Cur <= 0.f)
	{
		return;
	}

	float Tot = this->GetTotalHP();

	//更新动画
	if (Cur > Tot * 0.6f)
	{
		if (this->bAttack)
		{
			if (this->GetbIsAttack())
			{
				this->SetPlayAnimation(
					UGameSystemFunction::LoadRes(this->JumpAnim.Attack), true
				);
			}
			else
			{
				this->SetPlayAnimation(
					UGameSystemFunction::LoadRes(this->JumpAnim.Walk)
				);
			}
		}
		else {
			this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->JumpAnim.Idle)
			);
		}
	}
	else
	{
		if (this->bAttack)
		{
			if (this->GetbIsAttack())
			{
				this->SetPlayAnimation(
					UGameSystemFunction::LoadRes(this->JumpAnim.AttackLow), true
				);
			}
			else
			{
				this->SetPlayAnimation(
					UGameSystemFunction::LoadRes(this->JumpAnim.WalkLow)
				);
			}
		}
		else {
			this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->JumpAnim.IdleLow)
			);
		}
	}
}

void AJumpStateMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AJumpStateMouse::AnimPlayEnd);
}

void AJumpStateMouse::MouseInit()
{
	Super::MouseInit();

	this->bJumpStart = false;
	this->bJumpEnd = false;
	this->bJumpPass = true;
	this->bAttack = false;
	this->CheckDelay = 0.2f;
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	this->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->JumpAnim.Idle)
	);
}

