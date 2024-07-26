// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/DJ/DJ_Channel.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_DongJun.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"


FLine ADJ_Channel::GetChannel_In()
{
	return this->M_TargetLine;
}

FLine ADJ_Channel::GetChannel_Out()
{
	return this->M_CurrentLine;
}

ADJ_Channel::ADJ_Channel()
{
	this->M_MouseTypeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DJ_Mehse_Type"));
	this->M_MouseBodyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DJ_Channel_Body"));

	this->M_MouseTypeBox->SetupAttachment(this->M_MousePosition);
	this->M_MouseBodyBox->SetupAttachment(this->M_MouseTypeBox);
}

void ADJ_Channel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADJ_Channel::BeginPlay()
{
	Super::BeginPlay();
}

void ADJ_Channel::OnAnimationPlayEnd()
{
	
}

void ADJ_Channel::SetMouseDongJun(ABoss_DongJun* _Owner)
{
	this->M_Owner = _Owner;
}

bool ADJ_Channel::LinkChannel(FLine _CurrentLine)
{
	

	return false;
}

void ADJ_Channel::SetLinkCol(AMapMeshe* _AMapMeshe)
{
	this->M_AMapMeshe = _AMapMeshe;
}

void ADJ_Channel::Apply()
{
	
}

void ADJ_Channel::HPReset()
{
	
}

void ADJ_Channel::MesheReset()
{

}

bool ADJ_Channel::BeHit(UObject* CurHitMouseObj,float _HurtValue,EFlyItemAttackType AttackType)
{
	

	return true;
}

void ADJ_Channel::MouseDeathed()
{
	Super::MouseDeathed();

	
}