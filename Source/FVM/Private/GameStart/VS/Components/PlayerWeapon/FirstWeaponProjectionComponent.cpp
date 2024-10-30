// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/PlayerWeapon/FirstWeaponProjectionComponent.h"
#include "GameStart/Flipbook/GameActor/PlayerWeapon/PlayerFirstWeapon.h"
#include "SpineSkeletonAnimationComponent.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"

void UFirstWeaponProjectionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器投射组件，初始化"));
	}

	this->M_Owner = Cast<APlayerFirstWeapon>(this->GetOwner());
	if (IsValid(this->M_Owner))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器对象获取成功"));
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器对象获取失败"));
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器投射组件，构造完毕"));
	}
}

void UFirstWeaponProjectionComponent::SpawnBullet(AFlyItemActor* NewBullet)
{
	//初始化生成位置
	this->SpawnBulletLocation =
		this->M_Owner->GetActorLocation() +
		this->M_Owner->GetPointComponent()->GetRelativeLocation() +
		this->M_Owner->GetBulletLocationComp()->GetRelativeLocation();

	FTransform Trans;
	Trans.SetLocation(this->SpawnBulletLocation);
	NewBullet->SetFlyParentActor(this->M_Owner->GetPlayerActor());
	NewBullet->SetActorTransform(Trans);
	NewBullet->SetObjectActorLocation(this->M_Owner->GetPlayerActor()->GetCurrentMouse());
	//设置线路
	NewBullet->SetLine(this->M_Owner->GetPlayerActor()->GetLine().Row);
	//设置模式
	NewBullet->SetFloatModeEnable(this->M_Owner->GetPlayerActor()->GetMapMeshe()->GetMove());

	if (this->M_Owner->FlyItemProOverride.bOverride)
	{
		//设置攻击力
		NewBullet->SetATK(this->TargetData.ATK * this->M_Owner->FlyItemProOverride.BaseAttackUpRate);
		//设置二次攻击力
		NewBullet->SetSecondATK(this->M_Owner->FlyItemProOverride.SecondAttackOverwrite);
		//重写buff
		NewBullet->M_FItem_Buff = this->M_Owner->FlyItemProOverride.ItemBuffOverride;
		//重写攻击类型
		NewBullet->M_AttackType = this->M_Owner->FlyItemProOverride.AttackTypeOverride;
		//条件
		NewBullet->M_FlyCondition.M_FlyItemAttackType = this->M_Owner->FlyItemProOverride.FlyItemAttackType;
		NewBullet->M_FlyCondition.PanetrateLayers = this->M_Owner->FlyItemProOverride.PanetrateLayers;
	}
	else {
		//设置攻击力
		NewBullet->SetATK(this->TargetData.ATK);
		//设置二次攻击力
		NewBullet->SetSecondATK(this->TargetData.SecondATKRate);
	}


	NewBullet->Init();
}

void UFirstWeaponProjectionComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->PlayAttackAnim();
}


void UFirstWeaponProjectionComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->PlayIdleAnim();
}

void UFirstWeaponProjectionComponent::InitCondition()
{
	Super::InitCondition();

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("初始化条件"));
	}
}

void UFirstWeaponProjectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->bAuto)
	{
		return;
	}

	if (this->M_time < 0.3f)
	{
		this->M_time += DeltaTime;
	}
	else {
		this->M_time = 0.f;

		if (!IsValid(this->M_Owner->GetPlayerActor()->GetCurrentMouse()))
		{
			this->CheckAlien();
		}
	}
}

void UFirstWeaponProjectionComponent::LoadResource()
{
	Super::LoadResource();

	if (!IsValid(this->M_Owner))
	{
		this->SetTickableWhenPaused(true);
		return;
	}

	if (!IsValid(this->M_Owner->GetPlayerActor()))
	{
		this->SetTickableWhenPaused(true);
		return;
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("加载资源"));
	}

	const FMainWeaponData& LData = this->M_Owner->GetPlayerFirstWeaponData();

	this->TargetData = UMainWeaponDataFunc::Calculate(LData);

	//初始化发射条件
	this->InitLaunchProperty(
		this->TargetData.AttackCount,
		this->TargetData.AttackCoolingTime,
		0,
		this->TargetData.AttackBackTime
	);

	this->SetFirstAttackDelay(900.f);

	this->bPlayComp = true;

	//初始化子弹资源
	this->InitLaunchBulletByDef(this->M_Owner->WeaponBulletClassObj);

	this->PlayIdleAnim();
}

void UFirstWeaponProjectionComponent::UpdateAutoAttack(float _DeltaTime)
{
	this->bAuto = true;

	if (this->GetAttackModEnabled())
	{
		return;
	}

	if (AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->IsMouseExist())
	{
		this->SetAttackModEnabled(true);
	}
	else {
		this->SetAttackModEnabled(false);
	}
}

void UFirstWeaponProjectionComponent::OnAnimationComplete(class UTrackEntry* Track)
{
	this->SetFirstAttackDelay(900.f);
	this->OnAnimationPlayEnd();
	this->SetTrackEntry(nullptr);
	if (this->bAuto)
	{
		this->SetAttackModEnabled(false);
	}
	else {
		this->CheckAlien();
	}
}

void UFirstWeaponProjectionComponent::OnAnimationEvent(UTrackEntry* entry, FSpineEvent evt)
{
	if (evt.Name.Equals(TEXT("Attack")))
	{
		this->SetFirstAttackDelay(0.f);
	}
}

void UFirstWeaponProjectionComponent::CheckAlien()
{
	if (this->bAuto)
	{
		return;
	}

	//当前检测到的老鼠线路
	ELineTraceType CurCheckLineType;
	//当前老鼠
	AActor* CurMouse = UGameSystemFunction::CreateCheckMouseLineTrace(
		this->GetWorld(),
		this->M_Owner->GetLineTraceSetting(), CurCheckLineType
	);

	//老鼠有效则设置攻击模式
	if (IsValid(CurMouse) && Cast<AMouseActor>(CurMouse))
	{
		this->M_Owner->GetPlayerActor()->SetCurrentMouse(Cast<AMouseActor>(CurMouse));
		this->SetAttackModEnabled(true);
	}
	else
	{
		this->M_Owner->GetPlayerActor()->SetCurrentMouse(nullptr);
		this->SetAttackModEnabled(false);
	}
}

void UFirstWeaponProjectionComponent::PlayIdleAnim()
{
	//播放角色的默认动画
	this->M_Owner->GetPlayerActor()->PlayerDef_Anim();
	//播放武器的默认动画
	UTrackEntry* Track = this->M_Owner->SetAnimation(0, this->GetIdleAnimName(), true);
	Track->SetTimeScale(1 + TargetData.AttackSpeedUpRate);
	//绑定动画事件
	BINDANIMATION(Track, this, &UFirstWeaponProjectionComponent::OnAnimationComplete);
	this->SetTrackEntry(Track);
}

void UFirstWeaponProjectionComponent::PlayAttackAnim()
{
	//播放角色的攻击动画
	this->M_Owner->GetPlayerActor()->PlayerAttack_Anim(1 + TargetData.AttackSpeedUpRate);
	//播放武器的攻击动画
	UTrackEntry* Track = this->M_Owner->SetAnimation(0, this->GetAttackAnimName(), true);
	Track->SetTimeScale(1 + TargetData.AttackSpeedUpRate);
	BINDANIMATION(Track, this, &UFirstWeaponProjectionComponent::OnAnimationComplete);
	Track->AnimationEvent.AddDynamic(this, &UFirstWeaponProjectionComponent::OnAnimationEvent);
	this->SetTrackEntry(Track);
}
