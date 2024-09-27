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
	NewBullet->SetActorTransform(Trans);
	NewBullet->SetObjectActorLocation(this->M_Owner->GetPlayerActor()->GetCurrentMouse());
	NewBullet->SetATK(this->TargetData.ATK);
	NewBullet->SetSecondATK(0.f);
	NewBullet->SetLine(this->M_Owner->GetPlayerActor()->GetLine().Row);
	NewBullet->SetFloatModeEnable(this->M_Owner->GetPlayerActor()->GetMapMeshe()->GetMove());
	NewBullet->Init();
	NewBullet->OnInit();
}

void UFirstWeaponProjectionComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	//播放角色的攻击动画
	this->M_Owner->GetPlayerActor()->PlayerAttack_Anim(1 + TargetData.AttackSpeedUpRate);

	//播放武器的攻击动画
	UTrackEntry* Track = this->M_Owner->SetAnimation(0, this->GetAttackAnimName(), true);
	Track->SetTimeScale(1 + TargetData.AttackSpeedUpRate);
	//绑定动画事件
	Track->AnimationComplete.AddDynamic(
		this, &UFirstWeaponProjectionComponent::OnAnimationComplete);
	this->SetTrackEntry(Track);
}

void UFirstWeaponProjectionComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	//播放武器的默认动画
	this->M_Owner->SetAnimation(0, this->GetIdleAnimName(), true);
	//播放角色的默认动画
	this->M_Owner->GetPlayerActor()->PlayerDef_Anim();
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

	if (this->M_time < 0.3f)
	{
		this->M_time += DeltaTime;
	}
	else {
		this->M_time = 0.f;

		//bool LResult = false;

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
		this->TargetData.AttackFristTime,
		this->TargetData.AttackBackTime
	);

	//初始化子弹资源
	this->InitLaunchBulletByDef(this->M_Owner->WeaponBulletClassObj);

	//播放武器的默认动画
	this->M_Owner->SetAnimation(0, this->GetIdleAnimName(), true);
	//播放角色的默认动画
	this->M_Owner->GetPlayerActor()->PlayerDef_Anim();
}

AFlyItemActor* UFirstWeaponProjectionComponent::SpawnFlyItem(TSoftClassPtr<AFlyItemActor> _Path_C, FVector _Offset)
{
	/*
	if (!IsValid(this->Pool))
	{
		this->Pool = UObjectPoolManager::MakePoolManager(this->GetWorld(), _Path_C, 1);
	}

	FTransform Trans;
	Trans.SetLocation(this->SpawnBulletLocation);

	AFlyItemActor* L_AFlyItemActor_ = Cast<AFlyItemActor>(this->Pool->GetObjectActor());
	L_AFlyItemActor_->SetActorTransform(Trans);
	L_AFlyItemActor_->SetMouseActorLocation(this->M_Owner->GetPlayerActor()->GetCurrentMouse());
	L_AFlyItemActor_->SetATK(this->M_Owner->GetPlayerFirstWeaponData().ATK);
	L_AFlyItemActor_->SetSecondATK(0.f);
	L_AFlyItemActor_->SetLine(this->M_Owner->GetPlayerActor()->GetLine().Row);
	L_AFlyItemActor_->SetFloatModeEnable(this->M_Owner->GetPlayerActor()->GetMapMeshe()->GetMove());
	L_AFlyItemActor_->Init();
	L_AFlyItemActor_->OnInit();
	//L_AFlyItemActor_->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	//AFlyItemActor* L_AFlyItemActor_ = Cast<AFlyItemActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this->M_Owner->GetWorld(), LoadClass<AFlyItemActor>(0, *_Path_C), Trans));
	//UGameplayStatics::FinishSpawningActor(L_AFlyItemActor_, Trans);
	*/
	return nullptr;
}

void UFirstWeaponProjectionComponent::UpdateAutoAttack(float _DeltaTime)
{
	if (AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->IsMouseExist())
	{
		this->SetAttackModEnabled(true);
	}
}

void UFirstWeaponProjectionComponent::OnAnimationComplete(class UTrackEntry* Track)
{
	this->OnAnimationPlayEnd();
}
