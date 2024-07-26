// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicMirrorMouse.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

AMagicMirrorMouse::AMagicMirrorMouse()
{
	this->MMesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MouseMesheComponent"));
	this->MBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MouseBodyComponent"));

	//设置依附
	this->MMesheComponent->SetupAttachment(this->GetRootComponent());
	this->MBodyComponent->SetupAttachment(this->MMesheComponent);
}

void AMagicMirrorMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MMesheComponent, this->MBodyComponent);
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMagicMirrorMouse::OnAnimationPlayEnd);
}

void AMagicMirrorMouse::MouseInit()
{
	Super::MouseInit();

	this->CurRes = AGameMapInstance::GetResourceManagerComponent_Static();

	this->bAttackLine = true;
	this->bTake = false;
	this->bControll = false;
	this->fTakeDelay = 15.f - (((this->GetTotalHP() * 0.05f) > 10.f) ? 10 : this->GetTotalHP() * 0.05f);
	this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Def));
}

void AMagicMirrorMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bAttackLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->MECardCollisionType), DeltaTime);
	}

	if (this->bTake && !this->bControll)
	{
		if (this->fTakeSpeed > 0.f)
		{
			this->fTakeSpeed -= DeltaTime;
		}
		else {
			this->fTakeSpeed = this->AnimRes.TakeSpeed;
			CurRes->SubtractFlameNum(this->AnimRes.TakeEnergyCount);
		}
	}
}

void AMagicMirrorMouse::OnAnimationPlayEnd()
{
	if (this->GetCurrentHP() <= 0)
	{
		return;
	}

	//如果启动了吸取，则播放完毕关闭吸取
	if (this->bTake)
	{
		this->bTake = false;
		this->bAttackLine = true;
		this->MoveStart();
	}

	this->UpdateState();

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
}

void AMagicMirrorMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

	this->UpdateMove(DeltaTime);

	if (this->bTake)
	{
		return;
	}

	if (this->fTakeDelay > 0.f)
	{
		this->fTakeDelay -= DeltaTime;
	}
	else {
		if (CurRes->GetCurrentFlameNum() > 0)
		{
			this->fTakeDelay = 15.f - (((this->GetTotalHP() * 0.05f) > 10.f) ? 10 : this->GetTotalHP() * 0.05f);
			this->bTake = true;
			this->bAttackLine = false;
			this->MoveStop();
			//播放吸取动画
			if (this->GetCurrentHP() > this->GetTotalHP() * 0.5)
			{
				this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Take));
			}
			else {
				this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.TakeLow));
			}
		}
	}
}

bool AMagicMirrorMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		return false;
	}

	this->UpdateState();

	return true;
}

void AMagicMirrorMouse::AttackedBegin()
{
	Super::AttackedBegin();

	this->UpdateState();
}

void AMagicMirrorMouse::AttackedEnd()
{
	Super::AttackedEnd();

	this->UpdateState();
}

void AMagicMirrorMouse::MoveingBegin()
{
	Super::MoveingBegin();

	this->UpdateState();
}

void AMagicMirrorMouse::MouseDeathed()
{
	this->bAttackLine = false;
	this->bTake = false;
	this->bControll = false;
	this->fTakeDelay = 15.f;

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		//死亡动画
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Death));
	}

	//关闭碰撞
	this->ClosedBoxComponent(this->MMesheComponent);
	this->ClosedBoxComponent(this->MBodyComponent);
}

void AMagicMirrorMouse::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	Super::ExecuteBuff(BuffTag, CurBuffTime);

	if (
		BuffTag == EGameBuffTag::Freeze
		||
		BuffTag == EGameBuffTag::Solidification
		)
	{
		this->bControll = true;
	}
}


void AMagicMirrorMouse::ExitBuff(EGameBuffTag BuffTag)
{
	Super::ExitBuff(BuffTag);

	if (
		BuffTag == EGameBuffTag::Freeze
		&&
		BuffTag == EGameBuffTag::Solidification
		)
	{
		this->bControll = false;
	}

}

void AMagicMirrorMouse::UpdateState()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->bTake)
	{
		return;
	}

	float HP = this->GetCurrentHP();
	float Total = this->GetTotalHP();
	bool bAtk = this->GetbIsAttack();

	if (HP > Total * 0.5f)
	{
		if (bAtk)
		{
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.ATK));
		}
		else {
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Def));
		}
	}
	else {
		if (bAtk)
		{
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.ATKLow));
		}
		else {
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.DefLow));
		}
	}
}
