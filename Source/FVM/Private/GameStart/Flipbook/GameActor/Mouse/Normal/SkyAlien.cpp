// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/SkyAlien.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>

void ASkyAlien::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞网格位置
	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->BodyCollisionOffset);
}

void ASkyAlien::MouseInit()
{
	Super::MouseInit();

	UTrackEntry* Trac = this->SetAnimation(0,
		this->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName().ToString(), true
	);
}

void ASkyAlien::MouseDeathed()
{
	Super::MouseDeathed();

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

void ASkyAlien::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}
