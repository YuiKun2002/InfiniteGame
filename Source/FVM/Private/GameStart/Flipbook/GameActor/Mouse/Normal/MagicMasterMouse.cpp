// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicMasterMouse.h"
#include "GameStart/Flipbook/Components/FlipbookBaseComponent.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>


void AMagicMasterHpAddtionBuff::Init(AMouseActor* MouseActor)
{
	if (IsValid(MouseActor))
	{
		this->CurMouse = MouseActor;

		this->SetLifeSpan(1.5f);
		this->time = this->CTime;
		this->InitSpineShow();
		this->SetRenderLayer(this->CurMouse->GetRenderLayer() + 5);
		this->SetActorLocation(this->CurMouse->GetActorLocation() + FVector(0.f, 0.f, 20.f));
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
			this->SetRenderLayer(this->CurMouse->GetRenderLayer() + 5);
			this->SetActorLocation(this->CurMouse->GetActorLocation() + FVector(0.f, 0.f, 20.f));
		}
	}
}


void AMagicMasterMouse::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞网格位置
	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);

	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMagicMasterMouse::OnAnimationPlayEnd);
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

	
	//设置动画
	UTrackEntry* Track = this->SetAnimation(0,
		this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
		true);
	BINDANIMATION(Track,this,&AMagicMasterMouse::AnimationPlayEnd);
	this->SetTrackEntry(Track);
}


void AMagicMasterMouse::MouseTick(const float& DeltaTime)
{
	if (this->bEnableAttakLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->MECardCollisionType), DeltaTime);
	}
}

void AMagicMasterMouse::AnimationPlayEnd(UTrackEntry* Track)
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	//使用魔笛
	if (this->bUse)
	{
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
					Cur->BeHit(Cur, CurReplyHp * -1.f, EFlyItemAttackType::Def);
					//生成加血动画
					AMagicMasterHpAddtionBuff* CurAnimObj = this->GetWorld()->SpawnActor<AMagicMasterHpAddtionBuff>();
					CurAnimObj->Init(Cur);
				}
			}
		}

	}

	this->AddAttackCardUpdate();
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
		return;
	}

	this->UpdateMove(DeltaTime);
}

void AMagicMasterMouse::MouseDeathed()
{
	this->bEnableAttakLine = false;
	this->bUse = false;

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
