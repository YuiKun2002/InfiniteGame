// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicMasterMouse.h"
#include "GameStart/Flipbook/Components/FlipbookBaseComponent.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>


void AMagicMasterHpAddtionBuff::Init(AMouseActor* MouseActor, TSoftObjectPtr<UPaperFlipbook> Anim)
{
	if (IsValid(MouseActor))
	{
		this->CurMouse = MouseActor;

		this->SetLifeSpan(1.5f);
		this->time = this->CTime;
		this->InitRotation();
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim));
		this->SetTranslucentSortPriority(this->CurMouse->GetTranslucentSortPriority() + 5);
		this->SetActorLocation(this->CurMouse->GetActorLocation() + FVector(0.f, 0.f, 20.f));
		this->SetAnimationPlayEndDestroy();
	}
	else {
		this->Destroy();
	}
}


void AMagicMasterHpAddtionBuff::Tick(float DeltaTime)
{
	if (this->time > 0.f)
	{
		this->time -= DeltaTime;
	}
	else {
		this->time = this->CTime;

		if (IsValid(this->CurMouse))
		{
			this->SetTranslucentSortPriority(this->CurMouse->GetTranslucentSortPriority() + 5);
			this->SetActorLocation(this->CurMouse->GetActorLocation() + FVector(0.f, 0.f, 20.f));
		}
	}
}

AMagicMasterMouse::AMagicMasterMouse()
{

	this->MMesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComponent"));
	this->MBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BodyComponent"));
	this->WepaonAnimLocation = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("WepaonAnimLocationComponent"));

	//设置依附
	this->MMesheComponent->SetupAttachment(this->GetRootComponent());
	this->MBodyComponent->SetupAttachment(this->MMesheComponent);
	this->WepaonAnimLocation->SetupAttachment(this->GetRenderComponent());
}

void AMagicMasterMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MMesheComponent, this->MBodyComponent);

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMagicMasterMouse::OnAnimationPlayEnd);
}

void AMagicMasterMouse::MouseInit()
{
	Super::MouseInit();

	//float curTemp = this->fUseWeaponTime - this->GetTotalHP() * 0.005;
	this->fUseDelay = 2.5f; //curTemp > 3 ? curTemp : 3.f;
	this->fWeaponRangeRadius_ = this->fWeaponRangeRadius + this->GetTotalHP() * 0.005;
	this->fReplySelfHpRate_ = this->fReplySelfHpRate + this->GetTotalHP() * 0.001;

	this->bEnableAttakLine = true;
	this->bUse = false;

	this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Idle));
	this->WepaonAnimLocation->SetPlayAnimation(nullptr);
}

void AMagicMasterMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicMasterMouse::MouseTick(const float& DeltaTime)
{
	if (this->bEnableAttakLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->MECardCollisionType), DeltaTime);
	}
}

void AMagicMasterMouse::OnAnimationPlayEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	//使用魔笛
	if (this->bUse)
	{
		this->WepaonAnimLocation->SetPlayAnimation(nullptr);

		this->MoveStart();
		this->bUse = false;
		this->bEnableAttakLine = true;

		TArray<FHitResult> CurMouse;
		if (UGameSystemFunction::AddSphereTraceMulti(
			this, CurMouse, FVector(0.f, 0.f, 15.f), FVector(0.f, 0.f, 15.f), this->fWeaponRangeRadius_,
			UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType::MouseActor)))
		{

			for (const FHitResult& CurHit : CurMouse)
			{
				AMouseActor* Cur = Cast<AMouseActor>(CurHit.GetActor());
				if (IsValid(Cur) && Cur->GetCurrentHP() > 0.f && Cur->GetMouseBaseType() != EMouseBaseType::Boss)
				{
					float CurReplyHp = Cur->GetTotalHP() * this->fReplySelfHpRate;
					Cur->SetbIsHurt(true);
					Cur->BeHit(Cur,CurReplyHp * -1.f, EFlyItemAttackType::Def);
					//生成加血动画
					AMagicMasterHpAddtionBuff* CurAnimObj = this->GetWorld()->SpawnActor<AMagicMasterHpAddtionBuff>();
					CurAnimObj->Init(Cur, this->AnimRes.AddHPAnim);
				}
			}
		}

	}

	this->UpdateState();
	this->AddAttackCardUpdate();
}

void AMagicMasterMouse::UpdateState()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->bUse)
	{
		return;
	}

	bool bAtk = this->GetbIsAttack();

	if (bAtk)
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Attack));
	}
	else {
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Idle));
	}
}

void AMagicMasterMouse::MoveingBegin()
{
	Super::MoveingBegin();

	this->UpdateState();
}

void AMagicMasterMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	//魔笛延迟
	if (this->fUseDelay > 0.f)
	{
		this->fUseDelay -= DeltaTime;
	}
	else {
		float curTemp = this->fUseWeaponTime - this->GetTotalHP() * 0.005;
		this->fUseDelay = curTemp > 3 ? curTemp : 3.f;
		this->MoveStop();
		this->bEnableAttakLine = false;
		this->bUse = true;
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Use));
		this->WepaonAnimLocation->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.UseAnim));
		return;
	}

	//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));
	this->UpdateMove(DeltaTime);
}

bool AMagicMasterMouse::BeHit(UObject* CurHitMouseObj,float HurtValue, EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj,HurtValue, AttackType))
	{
		return false;
	}

	this->UpdateState();

	return true;
}

void AMagicMasterMouse::AttackedBegin()
{
	Super::AttackedBegin();

	this->UpdateState();
}

void AMagicMasterMouse::AttackedEnd()
{
	Super::AttackedEnd();

	this->UpdateState();
}

void AMagicMasterMouse::MouseDeathed()
{
	this->bEnableAttakLine = false;
	this->bUse = false;

	//关闭碰撞
	this->ClosedBoxComponent(this->MMesheComponent);
	this->ClosedBoxComponent(this->MBodyComponent);

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Death), true);
	}
}
