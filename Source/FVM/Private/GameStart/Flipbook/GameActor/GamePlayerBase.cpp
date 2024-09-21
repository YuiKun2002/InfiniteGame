// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/GamePlayerBase.h"
#include "GameStart/Flipbook/Components/FlipbookBaseComponent.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "GameSystem/FVMGameInstance.h"

// Sets default values
AGamePlayerBase::AGamePlayerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGamePlayerBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGamePlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePlayerBase::InitPlayerData()
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();
	//初始化数据
	this->HeroData = PlayerData->PlayerData;
}

void AGamePlayerBase::GetPlayerData(FItemHeroBase& OutData)
{
	OutData = UItemHeroDataFunc::Calculate(this->HeroData);
}

