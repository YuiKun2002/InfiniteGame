// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/ElementSphere.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>

AElementSphere::AElementSphere() :ANormalSphereBase() {}

void AElementSphere::MouseInit()
{
	Super::MouseInit();

	this->SetPlayAnimationOnce(
		UGameSystemFunction::LoadRes(this->Create), UGameSystemFunction::LoadRes(this->Rolling)
	);
}

void AElementSphere::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞网格位置
	UGameSystemFunction::InitMouseMeshe(this->MMeshe, this->MBody, FVector2D(20.f),FVector2D(0.f));
}

void AElementSphere::AttackedBegin() {}
void AElementSphere::AttackedEnd() {
	this->MoveStart();
}

bool AElementSphere::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	float CurValue = _HurtValue;

	//元素伤害
	AFlyItemActor* CurActor = Cast<AFlyItemActor>(CurHitMouseObj);
	if (IsValid(CurActor))
	{
		if (CurActor->GetBuff().M_bCondition)
		{
			for (const auto& CurBuff : CurActor->GetBuff().M_Buffers)
			{
				switch (CurBuff.Key)
				{
				case Buff_Infor::Burn: {
					if (this->ElementType == Buff_Infor::E_Freeze)
					{
						CurValue = CurValue * 2.5;
					}
				}; break;
				case Buff_Infor::E_SlowDown: {
					if (this->ElementType == Buff_Infor::Burn)
					{
						CurValue = CurValue * 4;
					}
				}; break;
				default:break;
				}
			}
		}
	}

	bool Re = Super::BeHit(CurHitMouseObj, CurValue, AttackType);

	if (Re)
	{
		if (this->GetCurrentHP() <= this->GetTotalHP() * 0.5)
		{
			this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->RollingLow)
			);
		}
		else {
			this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(this->Rolling)
			);
		}
	}

	return Re;
}

void AElementSphere::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	CurBuffTime = 0;

	for (const auto& Cur : this->BuffKill)
	{
		if (Cur == BuffTag)
		{
			this->SetMouseDeath(true);
			break;
		}
	}
}

void AElementSphere::MoveingUpdate(float DeltaTime)
{
	this->AddActorLocalOffset(
		FVector(0.f, (DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed), 0.f));
}

void AElementSphere::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	this->AddAttackLineFunc(
		UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime
	);

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
}

void AElementSphere::MouseDeathed()
{
	Super::MouseDeathed();

	this->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->Death), true
	);

	this->ClosedBoxComponent(this->MMeshe);
	this->ClosedBoxComponent(this->MBody);
}
