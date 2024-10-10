// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/ShieldGeneratorAlien.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>

AShieldGeneratorAlien::AShieldGeneratorAlien()
{
	this->ShieldComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldComp"));
	this->ShieldComp->SetupAttachment(this->GetPointComponent());
}

void AShieldGeneratorAlien::BeginPlay()
{
	Super::BeginPlay();

	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void AShieldGeneratorAlien::MouseInit()
{
	Super::MouseInit();

	//float curTemp = this->fUseWeaponTime - this->GetTotalHP() * 0.005;
	this->fUseShieldTime_ = this->fUseShieldTime;
	this->fUseDelay = 2.5f; //curTemp > 3 ? curTemp : 3.f;
	this->fReplySelfHp_ = this->fReplySelfHp + this->GetTotalHP() * 0.005;

	this->bEnableAttakLine = true;
	this->bUse = false;

	//设置动画
	UTrackEntry* Track = this->SetAnimation(0,
		this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
		true);
	BINDANIMATION(Track, this, &AShieldGeneratorAlien::AnimationPlayEnd);
	this->SetTrackEntry(Track);

	this->ShieldComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShieldGeneratorAlien::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bEnableAttakLine)
	{
		this->AddAttackLineFunc(
			UGameSystemFunction::GetCardCollisionBoxType(this->MECardCollisionType),
			DeltaTime);
	}

	if (this->bUse)
	{
		if (IsValid(this->CurShieldAnimObj))
		{
			if (this->fUseShieldTime_ > 0.f)
			{
				this->fUseShieldTime_ -= DeltaTime;
			}
			else {
				this->fUseShieldTime_ = this->fUseShieldTime;
				this->CurShieldAnimObj->Destroy();
				this->CurShieldAnimObj = nullptr;
				this->MoveStart();
				this->bUse = false;
				this->bEnableAttakLine = true;
				this->ShieldComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				this->BodyComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				UTrackEntry* Track = this->SetAnimation(0,
					this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
					true);
				BINDANIMATION(Track, this, &AShieldGeneratorAlien::AnimationPlayEnd);
				this->SetTrackEntry(Track);
			}
		}
	}
}

void AShieldGeneratorAlien::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	//护盾延迟
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
		//播放Idle
		this->SetAnimation(0,
			this->M_DefAnim_Anim.IdleAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
		);
		this->SetTrackEntry(nullptr);
		return;
	}

	this->UpdateMove(DeltaTime);
}

bool AShieldGeneratorAlien::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->bUse)
	{
		if (IsValid(this->CurShieldAnimObj))
		{
			fReplySelfHp_ -= _HurtValue;
			if (fReplySelfHp_ <= 0)
			{
				this->MoveStart();
				this->bUse = false;
				this->bEnableAttakLine = true;
				this->ShieldComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				this->BodyComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				this->CurShieldAnimObj->Destroy();

				UTrackEntry* Track = this->SetAnimation(0,
					this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
					true);
				BINDANIMATION(Track, this, &AShieldGeneratorAlien::AnimationPlayEnd);
				this->SetTrackEntry(Track);
			}

			return true;
		}
	}

	return Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);
}

void AShieldGeneratorAlien::MouseDeathed()
{
	this->bEnableAttakLine = false;
	this->bUse = false;

	if (IsValid(this->CurShieldAnimObj))
	{
		this->CurShieldAnimObj->Destroy();
	}

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

void AShieldGeneratorAlien::AnimationPlayEnd(class UTrackEntry* Track)
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	//使用护盾
	if (this->bUse)
	{
		//this->MoveStart();
		//this->bUse = false;
		//this->bEnableAttakLine = true;

		this->bEnableAttakLine = false;

		//停止移动
		this->MoveStop();

		//开启护盾
		this->ShieldComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//计算护盾血量
		this->fReplySelfHp_ = this->fReplySelfHp + this->GetTotalHP() * 0.005;

		if (!IsValid(this->CurShieldAnimObj))
		{
			//生成护盾动画
			this->CurShieldAnimObj = this->GetWorld()->
				SpawnActor<ASpineActor>(
					this->ShieldAnim.LoadSynchronous()
				);
			this->CurShieldAnimObj->SetActorLocation(this->GetActorLocation());
			this->CurShieldAnimObj->SetRenderLayer(8000);
		}
	}

	this->AddAttackCardUpdate();
}
