// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Giant.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>

void AGiantAlien::BeginPlay()
{
	Super::BeginPlay();

	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void AGiantAlien::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(this->AlienName, TEXT("577"));

	this->ChangeState(NewObject<UGiantAlien_Walk>());
}

void AGiantAlien::MouseDeathed()
{
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Super::MouseDeathed();

	UTrackEntry* Trac = this->SetAnimation(0,
		this->M_DefAnim_Anim.DeadAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
	);
	BINDANIMATION(Trac, this, &AMouseActor::AlienDeadAnimationCompelet);
	this->SetTrackEntry(Trac);
}

void UGiantAlien_Walk::Init()
{
	this->Boss = Cast<AGiantAlien>(this->Get());

	this->Boss->MoveStart();

	this->Boss->SetAnimation(
		0, this->Boss->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
		true
	);
	this->Boss->SetTrackEntry(nullptr);
}

void UGiantAlien_Walk::MouseTick(const float& DeltaTime)
{
	this->Boss->AddAttackLineFunc(
		UGameSystemFunction::GetCardCollisionBoxType(this->Boss->MECardCollisionType),
		DeltaTime);
}

void UGiantAlien_Walk::AttackedBegin()
{
	this->Boss->MoveStop();

	this->Boss->ChangeState(NewObject<UGiantAlien_Attack>());
}

void UGiantAlien_Walk::MoveingUpdate(const float& DeltaTime)
{
	this->Get()->UpdateMove(DeltaTime);
}

void UGiantAlien_Attack::Init()
{
	this->Boss = Cast<AGiantAlien>(this->Get());

	UTrackEntry* Track = this->Boss->SetAnimation(
		0, this->Boss->M_DefAnim_Anim.AttackAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
		true
	);
	BINDANIMATION(Track, this->Boss, &ABossBase::OnBossAnimPlayEnd);
	this->Boss->SetTrackEntry(Track);
}

void UGiantAlien_Attack::AttackedEnd()
{
	//攻击结束【当前卡片死亡】 切换状态
	this->Boss->ChangeState(NewObject<UGiantAlien_Walk>());
}

void UGiantAlien_Attack::AnimPlayEnd()
{
	//攻击卡片【如果卡片死亡则会return】
	this->Boss->AddAttackCardUpdate();

	//创建范围攻击
	TArray<FHitResult> Hits;
	if (UGameSystemFunction::AddSphereTraceMulti(
		this->Boss,
		Hits,
		FVector::ZeroVector,
		FVector::ZeroVector,
		this->Boss->AttackRadius,
		UGameSystemFunction::GetCardCollisionTraceType(this->Boss->MECardCollisionType)
	))
	{
		for (const FHitResult& Hit : Hits)
		{
			//卡片
			ACardActor* Card = Cast<ACardActor>(Hit.GetActor());
			if (IsValid(Card))
			{
				Card->UpdateCardState(900000000);
				Card->KillCard();
				UResourceManagerComponent::ResourceAddBadCard();
			}
		}
	}
}
