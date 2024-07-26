// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Custom/MouseCustomBase.h"

AMouseCustomBase::AMouseCustomBase()
{

}

void AMouseCustomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMouseCustomBase::BeginPlay()
{
	Super::BeginPlay();


}

EMouseBaseType AMouseCustomBase::GetMouseBaseType()
{
	Super::GetMouseBaseType();

	return EMouseBaseType::Other;
}

bool AMouseCustomBase::BeHit(UObject* CurHitMouseObj,float _HurtValue,EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj,_HurtValue,AttackType))
		return false;

	return true;
}

void AMouseCustomBase::BeAttakedBegin()
{
	Super::BeAttakedBegin();
}


void AMouseCustomBase::MouseDeathed()
{
	Super::MouseDeathed();
}
