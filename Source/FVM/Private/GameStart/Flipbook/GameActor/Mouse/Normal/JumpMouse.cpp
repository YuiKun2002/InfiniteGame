// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/JumpMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "GameStart/Flipbook/GameActor/CardActor.h"

AJumpMouse::AJumpMouse()
{
	this->MMesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("JumpMouseMesheComponent"));
	this->MBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("JumpMouseBodyComponent"));

	//设置依附
	this->MMesheComponent->SetupAttachment(this->GetRootComponent());
	this->MBodyComponent->SetupAttachment(this->MMesheComponent);
}

void AJumpMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->GetCurrentHP() > 0.f)
	{
		this->CheckCard(DeltaTime);
	}
}

void AJumpMouse::MouseInit()
{
	Super::MouseInit();

	this->EnableBoxComponent(this->MMesheComponent);
	this->EnableBoxComponent(this->MBodyComponent);

	this->JumpSpeedRate = 1.f;
	this->CheckDelay = 0.2f;
	this->bJumpStart = false;
	this->bJump = false;
	this->bHi = false;


	UGameSystemFunction::InitMouseMeshe(this->MMesheComponent, this->MBodyComponent);
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AJumpMouse::AnimationPlayEnd);
}

void AJumpMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime * this->JumpSpeedRate);
}

bool AJumpMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		return false;
	}

	this->UpdateState();

	return true;
}

void AJumpMouse::MouseDeathed()
{
	this->ClosedBoxComponent(this->MMesheComponent);
	this->ClosedBoxComponent(this->MBodyComponent);

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.Death), true);
	}

	this->MoveStop();
}

void AJumpMouse::AnimationPlayEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	//遇到高强
	if (this->bHi)
	{
		this->bJumpStart = false;
		this->UpdateState();
		return;
	}

	//在跳跃
	if (this->bJump)
	{
		this->bJump = false;
		this->JumpSpeedRate = 1.f;
	}

	//跳跃开始
	if (this->bJumpStart)
	{
		this->bJumpStart = false;
		this->bJump = true;
		this->MoveStart();
	}

	this->UpdateState();
}

void AJumpMouse::UpdateState()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->bJump)
	{
		return;
	}

	if (this->bJumpStart)
	{
		return;
	}

	if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.Jump));
	}
	else {
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.JumpLow));
	}
}

bool AJumpMouse::CheckCard(const float& DeltaTime)
{
	//在跳跃
	if (this->bJump)
	{
		return false;
	}

	//跳跃开始
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


	FVector Offset = FVector(0.f, -25.f, 0.f);
	if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
	{
		Offset = FVector(0.f, -25.f, 25.f);
	}

	FHitResult Ht;
	if (UGameSystemFunction::AddSphereTrace(
		this, Ht, Offset, Offset, 3.f,
		UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor2)
	))
	{
		//检测到卡片
		this->MoveStop();
		ACardActor* CurCard = Cast<ACardActor>(Ht.GetActor());
		if (IsValid(CurCard))
		{
			//开始跳跃
			this->bJumpStart = true;
			this->bHi = false;
			for (const auto& Card : CurCard->GetUIMapMesh()->GetCardDatas())
			{
				if (IsValid(Card.Value) && Card.Value->GetCurrentHP() > 0.f)
				{
					if (Card.Value->GetCardData().CardCategoryTag == ECardCategoryTag::HighBlock)
					{
						//停止移动【遇到高墙】
						this->bJumpStart = false;
						this->bHi = true;
						return true;
					}
				}
			}

			//开始跳跃
			if (this->bJumpStart)
			{
				this->bHi = false;
				this->JumpSpeedRate = 3.2f;
				//更具当前血量播放跳跃动画
				if (this->GetCurrentHP() > this->GetTotalHP() * 0.6)
				{
					this->SetPlayAnimation(
						UGameSystemFunction::LoadRes(this->MouseAnimalRes.Jump), true
					);
				}
				else {
					this->SetPlayAnimation(
						UGameSystemFunction::LoadRes(this->MouseAnimalRes.JumpLow), true
					);
				}
			}

			return true;
		}
	}

	if (!this->GetbIsMove())
	{
		this->MoveStart();
		this->JumpSpeedRate = 1.f;
	}

	return false;
}

