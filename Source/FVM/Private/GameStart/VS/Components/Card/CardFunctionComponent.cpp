// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include "GameStart/Flipbook/GameActor/Card/FunctionCardActor.h"
#include "GameStart/VS/Components/Card/CardFuncCompImplement.h"
#include "GameStart/VS/Components/CardManagerComponent.h"

#include "GameStart/VS/MapBaseType.h"
#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include  <Kismet/GameplayStatics.h>
// Sets default values for this component's properties
UCardFunctionComponent::UCardFunctionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardFunctionComponent::BeginPlay()
{
	Super::BeginPlay();

	//获取卡片拥有者
	this->FunctionCardActor = Cast<AFunctionCardActor>(this->GetOwner());

	if (!this->FunctionCardActor)
	{
		this->SetComponentTickEnabled(false);
		return;
	}
	//设置功能组件
	this->FunctionCardActor->SetFunctionComponent(this);

	//获取上一次使用的卡片
	this->M_PlayerLastSelectCardActorClass = AGameMapInstance::GetGameMapInstance()->
		M_CardManagerComponent->M_LastSelectCardClass;

	this->M_PlayerLastSelectCardActorName = AGameMapInstance::GetGameMapInstance()->
		M_CardManagerComponent->M_LastSelectCardName;

	//获取卡片格子
	this->M_CardMapMeshe = this->FunctionCardActor->GetUIMapMesh();

	//播放默认动画
	if (UGameSystemFunction::LoadRes(this->FunctionCardActor->CardActor_DefAniml))
	{
		this->FunctionCardActor->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FunctionCardActor->CardActor_DefAniml));
	}

	//绑定动画播放完毕函数
	this->FunctionCardActor->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(
		this, &UCardFunctionComponent::OnAnimationPlayEnd
	);

	//运行功能函数
	this->FunctionCardActor->ExecuteCardFuncClassByCardFunction(this);
}


void UCardFunctionComponent::OnAnimationPlayEnd()
{
	if (FVM_VS_GamePause())
	{
		return;
	}

	//卡片休息
	if (!this->FunctionCardActor->GetCardDay() && this->FunctionCardActor->GetMapDay())
	{
		return;
	}

	//运行功能函数
	this->FunctionCardActor->ExecuteCardFuncClassByCardFunctionOnAnimPlayEnd(this);
}

FString UCardFunctionComponent::GetLastCardName() const
{
	return this->M_PlayerLastSelectCardActorName;
}

UClass* UCardFunctionComponent::GetLasrCardResourceClass() const
{
	return this->M_PlayerLastSelectCardActorClass;
}

AFunctionCardActor* UCardFunctionComponent::GetCardActor()
{
	return this->FunctionCardActor;
}

UUI_MapMeshe* UCardFunctionComponent::GetCardMeshe()
{
	return this->M_CardMapMeshe;
}

void UCardFunctionComponent::EventTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//卡片休息
	if (!this->FunctionCardActor->GetCardDay() && this->FunctionCardActor->GetMapDay())
	{
		return;
	}

	if (IsValid(OtherActor))
	{

		//运行功能函数
		this->FunctionCardActor->ExecuteCardFuncClassByCardFunctionOnOverlapBegin(this, OtherActor);
	}
}

// Called every frame
void UCardFunctionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FVM_VS_GamePause())
	{
		return;
	}

	//卡片休息
	if (!this->FunctionCardActor->GetCardDay() && this->FunctionCardActor->GetMapDay())
	{
		return;
	}

	//运行功能函数
	this->FunctionCardActor->ExecuteCardFuncClassByCardFunctionUpdate(this, DeltaTime);
}

