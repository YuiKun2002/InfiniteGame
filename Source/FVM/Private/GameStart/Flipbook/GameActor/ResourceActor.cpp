// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/ResourceActor.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"

AResourceActor* AResourceActor::MakeResourceActorCoin(FString ResourceName, int32 index, int32 Count)
{
	FSoftClassPath CurPath(FString(TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Resource/MyResourceActor.MyResourceActor_C'")));
	AResourceActor* TargetActor = Cast<AResourceActor>(UResourceManagerComponent::CreateNewActor(CurPath));
	if (IsValid(TargetActor))
	{
		TargetActor->InitCoin(ResourceName, index, Count);

		return TargetActor;
	}

	return nullptr;
}

AResourceActor* AResourceActor::MakeResourceActor(FString ResourceName, FSoftObjectPath HeadPath, int32 Count)
{
	FSoftClassPath CurPath(FString(TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Resource/MyResourceActor.MyResourceActor_C'")));
	AResourceActor* TargetActor = Cast<AResourceActor>(UResourceManagerComponent::CreateNewActor(CurPath));
	if (IsValid(TargetActor))
	{
		TargetActor->InitResource(ResourceName, HeadPath, Count);

		return TargetActor;
	}

	return nullptr;
}

void AResourceActor::BeginPlay()
{
	Super::BeginPlay();
}

void AResourceActor::InitResource(FString ResourceName, FSoftObjectPath HeadPath, int32 Count)
{
	//添加到资源管理器
	UResourceManagerComponent::AddResource(ResourceName, Count);

	this->InitResource(Cast<UTexture2D>(HeadPath.TryLoad()));
}

void AResourceActor::InitCoin(FString ResourceName, int32 CoinIndex, int32 Count)
{
	//添加到资产管理器
	UResourceManagerComponent::AddResource(ResourceName, Count);

	this->InitCoin(CoinIndex);
}
