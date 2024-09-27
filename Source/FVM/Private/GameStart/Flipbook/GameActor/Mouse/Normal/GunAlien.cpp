// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/GunAlien.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/Alien/AlienAttackLauncherComponent.h"

AGunAlien::AGunAlien()
{
	this->AttackLauncherComp = CreateDefaultSubobject<UAlienAttackLauncherComponent>(TEXT("AlienAttackLauncherComponent"));
}

void AGunAlien::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞网格位置
	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void AGunAlien::MouseInit()
{
	Super::MouseInit();

	//检查线路
	UMesheControllComponent* LocalMesheComp = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;
	TArray<FGunAlienCheckLineTraceBaseConfig> NewLine;
	for (const FGunAlienCheckLineTraceBaseConfig& Line : this->CheckLineTraceConfig)
	{
		if (
			this->GetMouseLine().Row + Line.OffsetLine >= 0
			||
			this->GetMouseLine().Row + Line.OffsetLine < LocalMesheComp->GetMapMeshRowAndCol().Row
			)
		{
			NewLine.Emplace(Line);
		}
	}

	this->CheckLineTraceConfig.Empty();
	this->CheckLineTraceConfig = NewLine;

	this->PlayMoveAnim();
}

void AGunAlien::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);
}

void AGunAlien::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);

	if (Time < 0.2f)
	{
		Time += DeltaTime;
	}
	else {
		Time = 0.f;
		this->CheckCard();
	}
}

bool AGunAlien::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	//更新血量
	return Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);
}

void AGunAlien::MouseDeathed()
{
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

void AGunAlien::InMapMeshe(ELineType CurLineType)
{
	if (this->GetMouseLineType() != ELineType::OnGround)
	{
		this->M_Proper_Condition.M_CurrentInType = CurLineType;
	}
	else {
		Super::InMapMeshe(CurLineType);
	}
}

void AGunAlien::OnAnimationPlayEnd(class UTrackEntry* Track)
{
	//启动
	this->AttackLauncherComp->OnAnimationComplete(Track);

	//检测卡片
	this->CheckCard();
}

FVector AGunAlien::GetBulletLauncherLocation()
{
	return FVector(
		this->GetActorLocation().X,
		this->GetActorLocation().Y + this->GetPointComponent()->GetRelativeLocation().X + this->LauncherBulletLocationComp->GetRelativeLocation().X,
		this->GetActorLocation().Z + this->GetPointComponent()->GetRelativeLocation().Z + this->LauncherBulletLocationComp->GetRelativeLocation().Z
	);
}

void AGunAlien::CheckCard()
{
	if (IsValid(this->GetCurrentAttackCard()) && this->GetCurrentAttackCard()->GetCurrentHP() > 0.f)
	{
		if (this->GetCurrentAttackCard()->GetLine().Row == this->GetMouseLine().Row)
		{
			this->AttackLauncherComp->SetAttackModEnabled(true);
			return;
		}
	}

	UMesheControllComponent* LocalMesheComp = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;

	this->SetCurrentAttackCard(nullptr);
	for (const FGunAlienCheckLineTraceBaseConfig& LineConfig : this->CheckLineTraceConfig)
	{
		//获取当前线路
		int32 CurLineZ = LocalMesheComp->GetMapMesh(
			this->GetMouseLine().Row + LineConfig.OffsetLine, 0
		)->GetActorLocation().Z;
		//设置基本位置
		FVector Location = this->GetActorLocation();
		Location.Z = CurLineZ;
		//添加检测
		if (UGameSystemFunction::AddLineTrance(
			this->GetWorld(), Location, FVector(
				0.f, LineConfig.CheckGridLength * -60.f, 0.f
			), FVector(0.f),
			UGameSystemFunction::GetCardCollisionBoxType(LineConfig.CheckCardType), this,
			[](UObject* GunAlienActor, AActor* CardActor) {
				Cast<AGunAlien>(GunAlienActor)->SetCurrentAttackCard(Cast<ACardActor>(CardActor));
			}
		))
		{
			this->MoveStop();
			break;
		}
	}

	if (IsValid(this->GetCurrentAttackCard()))
	{
		this->AttackLauncherComp->SetAttackModEnabled(true);
		this->PlayIdleAnim();
	}
	else {
		this->AttackLauncherComp->SetAttackModEnabled(false);
		this->PlayMoveAnim();
		this->MoveStart();
	}

}

void AGunAlien::PlayMoveAnim()
{
	if (this->State != 0)
	{
		//走路状态
		this->State = 0;
		//播放走路动画
		UTrackEntry* Track = this->SetAnimation(0,
			this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		this->SetTrackEntry(Track);
	}
}

void AGunAlien::PlayAttackAnim()
{
	if (this->State != 1)
	{
		//攻击状态
		this->State = 1;

		//播放攻击动画
		UTrackEntry* Track = this->SetAnimation(0,
			this->M_DefAnim_Anim.AttackAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		BINDANIMATION(Track, this, &AGunAlien::OnAnimationPlayEnd);
		this->SetTrackEntry(Track);
	}
}

void AGunAlien::PlayIdleAnim()
{
	if (this->State != 2)
	{
		//攻击状态
		this->State = 2;

		//播放攻击动画
		UTrackEntry* Track = this->SetAnimation(0,
			this->M_DefAnim_Anim.IdleAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		BINDANIMATION(Track, this, &AGunAlien::OnAnimationPlayEnd);
		this->SetTrackEntry(Track);
	}
}
