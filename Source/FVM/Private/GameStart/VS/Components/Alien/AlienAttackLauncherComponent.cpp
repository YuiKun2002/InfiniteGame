// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Alien/AlienAttackLauncherComponent.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/GunAlien.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/GunAlienWidget.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"

#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "GameSystem/GameDataSubsystem.h"
#include "SpineSkeletonAnimationComponent.h"

void UAlienAttackLauncherComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化攻击卡片
	this->GunAlien = Cast<AGunAlien>(this->GetOwner());
	this->GunAlienWidegt = Cast<AGunAlienWidget>(this->GetOwner());
	if (!IsValid(this->GunAlien) && !IsValid(this->GunAlienWidegt))
	{
		UGameSystemFunction::FVMLog(TEXT("UCardAttackComponent"), TEXT("攻击卡片初始化失败"));
		this->SetComponentTickEnabled(false);
	}
}

void UAlienAttackLauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (IsValid(this->GunAlien))
	{
		if (GunAlien->GetbShootEnabled())
		{
			Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		}
	}

	if (IsValid(this->GunAlienWidegt))
	{
		if (GunAlienWidegt->GetbShootEnabled())
		{
			Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		}
	}

}

void UAlienAttackLauncherComponent::SpawnBullet(AFlyItemActor* NewBullet)
{
	//获取对象的变换位置
	FTransform NewTrans;

	if (IsValid(this->GunAlien))
	{
		NewTrans.SetLocation(this->GunAlien->GetBulletLauncherLocation());
		//新生成的对象设置自定义拥有者(CardActor)
		NewBullet->SetLine(this->GunAlien->GetMouseLine().Row);
		NewBullet->SetObjectActorLocation(this->GunAlien->GetCurrentAttackCard());
		NewBullet->SetATK(this->GunAlien->GetCurrentATK());
	}

	if (IsValid(this->GunAlienWidegt))
	{
		NewTrans.SetLocation(this->GunAlienWidegt->GetBulletLauncherLocation());
		//新生成的对象设置自定义拥有者(CardActor)
		NewBullet->SetLine(this->GunAlienWidegt->GetMouseLine().Row);
		NewBullet->SetObjectActorLocation(this->GunAlienWidegt->GetCurrentAttackCard());
		NewBullet->SetATK(this->GunAlienWidegt->GetCurrentATK());
	}

	NewTrans.SetLocation(
		FVector(
			550.f,
			NewTrans.GetLocation().Y,
			NewTrans.GetLocation().Z
		)
	);
	NewBullet->SetActorTransform(NewTrans);
	NewBullet->SetSecondATK(0.f);
	NewBullet->SetFloatModeEnable(true);
	NewBullet->Init();
}

void UAlienAttackLauncherComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	if (IsValid(this->GunAlien))
	{
		if (!this->GunAlien->GetMouseIsDeath())
		{
			this->GunAlien->PlayAttackAnim();
		}
	}

	if (IsValid(this->GunAlienWidegt))
	{
		if (!this->GunAlienWidegt->GetMouseIsDeath())
		{
			this->GunAlienWidegt->PlayAttackAnim();
		}
	}

}

void UAlienAttackLauncherComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	if (IsValid(this->GunAlien))
	{
		if (!this->GunAlien->GetMouseIsDeath())
		{
			this->GunAlien->PlayIdleAnim();
		}
	}

	if (IsValid(this->GunAlienWidegt))
	{
		if (!this->GunAlienWidegt->GetMouseIsDeath())
		{
			this->GunAlienWidegt->PlayIdleAnim();
		}
	}
}

void UAlienAttackLauncherComponent::LoadResource()
{
	Super::LoadResource();

	if (IsValid(this->GunAlien))
	{
		//初始化条件
		this->InitLaunchProperty(
			this->GunAlien->LauncherConfig.AttackCount,
			this->GunAlien->LauncherConfig.AttackSpeed,
			this->GunAlien->LauncherConfig.FirstAttackSpeed,
			this->GunAlien->LauncherConfig.SecondAttackSpeed
		);

		//初始化默认子弹
		this->InitLaunchBulletByDef(this->GunAlien->AlienDefBulletClass);
	}

	if (IsValid(this->GunAlienWidegt))
	{
		//初始化条件
		this->InitLaunchProperty(
			this->GunAlienWidegt->LauncherConfig.AttackCount,
			this->GunAlienWidegt->LauncherConfig.AttackSpeed,
			this->GunAlienWidegt->LauncherConfig.FirstAttackSpeed,
			this->GunAlienWidegt->LauncherConfig.SecondAttackSpeed
		);

		//初始化默认子弹
		this->InitLaunchBulletByDef(this->GunAlienWidegt->AlienDefBulletClass);
	}
}

void UAlienAttackLauncherComponent::OnAnimationComplete(class UTrackEntry* Track)
{
	this->OnAnimationPlayEnd();
}
