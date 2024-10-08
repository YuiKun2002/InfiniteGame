// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Alien/WaterSceneComponent.h"

// Sets default values for this component's properties
UWaterSceneComponent::UWaterSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UWaterSceneComponent::SpawnWater()
{
	if (IsValid(this->Water))
	{
		this->Water->SetActorHiddenInGame(false);
		this->Water->OnMove(
			FVector(
				this->GetComponentLocation().X + this->GetRelativeLocation().Y,
				this->GetComponentLocation().Y + this->GetRelativeLocation().X,
				this->GetComponentLocation().Z + this->GetRelativeLocation().Z
			)
		);
		this->Water->OnInit();
	}
	else {
		//生成水面
		this->Water = this->OwnerActor->GetWorld()->SpawnActor<AWaterActor>(
			LoadClass<AWaterActor>(nullptr,
				TEXT("Blueprint'/Game/Resource/SpineData/水动画/BP_WaterUp.BP_WaterUp_C'"))
		);
		this->Water->OnMove(
			this->GetComponentLocation()
		);
		this->Water->OnInit();
	}
}

void UWaterSceneComponent::RenderLayer(const int32& Layer)
{
	if (IsValid(this->Water))
	{
		this->Water->OnSetRenderLayer(Layer);
	}
}

void UWaterSceneComponent::Move(const float& Tick)
{
	if (IsValid(this->Water))
	{
		this->Water->OnMove(FVector(
			this->GetComponentLocation().X + this->GetRelativeLocation().Y,
			this->GetComponentLocation().Y + this->GetRelativeLocation().X,
			this->GetComponentLocation().Z + this->GetRelativeLocation().Z
		));
	}

}

void UWaterSceneComponent::End()
{

	if (IsValid(this->Water))
	{
		this->Water->SetActorHiddenInGame(true);
		this->Water->OnEnd();
	}
}

void UWaterSceneComponent::Dead()
{
	if (IsValid(this->Water))
	{
		this->Water->OnDead();
		this->Water = nullptr;
	}
}

// Called when the game starts
void UWaterSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	this->OwnerActor = this->GetOwner();
}
