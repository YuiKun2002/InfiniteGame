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
			this->GetComponentLocation()		
		);
		this->Water->OnInit();
	}
	else {
		//生成水面
		this->Water = this->OwnerActor->GetWorld()->SpawnActor<AWaterActor>(
			LoadClass<AWaterActor>(nullptr,
				TEXT(""))
		);
		this->Water->OnMove(
			this->GetComponentLocation()
		);
		this->Water->OnInit();
	}
}

void UWaterSceneComponent::Move(const float& Tick)
{
	this->Water->OnMove(this->GetComponentLocation());
}

void UWaterSceneComponent::End()
{
	this->Water->OnEnd();
}

// Called when the game starts
void UWaterSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	this->OwnerActor = this->GetOwner();
}
