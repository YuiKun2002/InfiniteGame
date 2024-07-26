// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicChildMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "GameSystem/Tools/GameSystemFunction.h"

AMagicChildMouse::AMagicChildMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AMagicChildMouse::OnFlyEnd()
{
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->bFlyMove = true;
	this->bAtk = true;

	this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Walk));
}

void AMagicChildMouse::DisEnableATK()
{
	this->bAtk = false;
}

void AMagicChildMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (this->bAtk)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->CardCollisionType), DeltaTime);
	}
}

void AMagicChildMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMagicChildMouse::AnimPlayEnd);
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMagicChildMouse::MouseInit()
{
	Super::MouseInit();

	//播放走动动画
	this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Walk));
	this->bAtk = true;
}

void AMagicChildMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	if (this->bFlyMove)
	{
		//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

		this->UpdateMove(DeltaTime);
	}
}

void AMagicChildMouse::AttackedBegin()
{
	Super::AttackedBegin();

	this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Attack));
}

void AMagicChildMouse::AttackedEnd()
{
	Super::AttackedEnd();

	this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Walk));
}

void AMagicChildMouse::MouseDeathed()
{
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->bAtk = false;

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Death));
	}
}

void AMagicChildMouse::AnimPlayEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->GetbIsAttack())
	{
		if (!this->AttackCard())
		{
			this->SetbIsAttack(false);
		}
	}
}
