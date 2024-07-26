// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/MainMenu/PlayerCreate2DCatch.h"
#include "GameStart/Flipbook/Components/FlipbookBaseComponent.h"


// Sets default values
APlayerCreate2DCatch::APlayerCreate2DCatch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	this->M_UFlipbookBaseComponent = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("FlipAnimations"));
	this->M_LocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("LocationComponent"));

	this->SetRootComponent(this->M_LocationComp);
	this->M_UFlipbookBaseComponent->SetupAttachment(this->M_LocationComp);
}

// Called when the game starts or when spawned
void APlayerCreate2DCatch::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCreate2DCatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

