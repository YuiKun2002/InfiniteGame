// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_Tank.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"

void ABoss_Tank::MouseInit()
{
	Super::MouseInit();

	this->ChangeState(NewObject<UBoss_Tank_Walk>());
}

void UBoss_Tank_Walk::Init()
{
	Super::Init();

	//初始化发射时间
	this->LaunchTime = Cast<ABoss_Tank>(this->Boss)->LaunchTime;
}

void UBoss_Tank_Walk::MouseTick(const float& DeltaTime)
{
	if (this->LaunchTime > 0.f)
	{
		this->LaunchTime -= DeltaTime;
	}
	else {
		this->LaunchTime = 10.f;


		//切换攻击模式

		if (UGameSystemFunction::GetRange(50, 100))
		{
			this->Boss->ChangeState(NewObject<UBoss_Tank_AttackMissile>());
		}
		else {
			this->Boss->ChangeState(NewObject<UBoss_Tank_AttackLaser>());
		}
	}
}

void UBoss_Tank_AttackMissile::Init()
{
	this->Boss = Cast<ABoss_Tank>(this->Get());

	this->PlayAttackAnim();
}

void UBoss_Tank_AttackMissile::PlayAttackAnim()
{
	if (this->Boss->GetCurrentHP() > this->Boss->GetTotalHP() * 0.5f)
	{
		//播放攻击动画
		UTrackEntry* Track = this->Boss->SetAnimation(0,
			this->Boss->M_DefAnim_Anim.AttackAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		//绑定发射事件
		Track->AnimationEvent.AddDynamic(this, &UBoss_Tank_AttackMissile::Launch);
		//绑定动画播放完毕
		BINDANIMATION(Track, this, &UBoss_Tank_AttackMissile::AAnimPlayEnd);
		this->Boss->SetTrackEntry(Track);
	}
	else {
		//播放攻击动画
		UTrackEntry* Track = this->Boss->SetAnimation(0,
			this->Boss->M_DefAnim_Anim.AttackAnimDamageRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		//绑定发射事件
		Track->AnimationEvent.AddDynamic(this, &UBoss_Tank_AttackMissile::Launch);
		//绑定动画播放完毕
		BINDANIMATION(Track, this, &UBoss_Tank_AttackMissile::AAnimPlayEnd);
		this->Boss->SetTrackEntry(Track);
	}
}

void UBoss_Tank_AttackMissile::AAnimPlayEnd(UTrackEntry* Track)
{
	this->Boss->SetTrackEntry(nullptr);
	//切换状态
	this->Boss->ChangeState(NewObject<UBoss_Tank_Walk>());
}

void UBoss_Tank_AttackMissile::Launch(UTrackEntry* entry, FSpineEvent evt)
{
	if (evt.Name.Equals(TEXT("Launch")))
	{
		this->Boss->OnSpawnMissile();
	}
}

void UBoss_Tank_AttackLaser::Init()
{
	this->Boss = Cast<ABoss_Tank>(this->Get());

	this->PlayAttackAnim();
}

void UBoss_Tank_AttackLaser::PlayAttackAnim()
{
	if (this->Boss->GetCurrentHP() > this->Boss->GetTotalHP() * 0.5f)
	{
		//播放攻击动画
		UTrackEntry* Track = this->Boss->SetAnimation(0,
			this->Boss->LaserAttackAnimRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		//绑定发射事件
		Track->AnimationEvent.AddDynamic(this, &UBoss_Tank_AttackLaser::Launch);
		//绑定动画播放完毕
		BINDANIMATION(Track, this, &UBoss_Tank_AttackLaser::AAnimPlayEnd);
		this->Boss->SetTrackEntry(Track);
	}
	else {
		//播放攻击动画
		UTrackEntry* Track = this->Boss->SetAnimation(0,
			this->Boss->LaserAttackAnimDamageRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		//绑定发射事件
		Track->AnimationEvent.AddDynamic(this, &UBoss_Tank_AttackLaser::Launch);
		//绑定动画播放完毕
		BINDANIMATION(Track, this, &UBoss_Tank_AttackLaser::AAnimPlayEnd);
		this->Boss->SetTrackEntry(Track);
	}
}

void UBoss_Tank_AttackLaser::AAnimPlayEnd(class UTrackEntry* Track)
{
	this->Boss->SetTrackEntry(nullptr);
	//切换状态
	this->Boss->ChangeState(NewObject<UBoss_Tank_Walk>());
}

void UBoss_Tank_AttackLaser::Launch(UTrackEntry* entry, FSpineEvent evt)
{
	if (evt.Name.Equals(TEXT("Launch")))
	{
		this->Boss->OnSpawnLaser();
	}
}
