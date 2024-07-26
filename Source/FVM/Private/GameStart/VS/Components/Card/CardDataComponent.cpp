// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardDataComponent.h"

// Sets default values for this component's properties
UCardDataComponent::UCardDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


FItemCardATK UCardDataComponent::GetCardATKData(const FString& _CardName)
{
	return GetCardData<FItemCardATK>(_CardName, ECardType::E_ATK);
}

FItemCardSpawn UCardDataComponent::GetCardSpawnData(const FString& _CardName)
{
	return GetCardData<FItemCardSpawn>(_CardName, ECardType::E_SPAWN);
}

FItemCardDefence UCardDataComponent::GetCardDefenceData(const FString& _CardName)
{
	return  GetCardData<FItemCardDefence>(_CardName, ECardType::E_DEFENCE);
}

FItemCardFunction UCardDataComponent::GetCardFunction(const FString& _CardName)
{
	return  GetCardData<FItemCardFunction>(_CardName, ECardType::E_Function);
}

FItemCardRangeATK UCardDataComponent::GetCardRangeATKData(const FString& _CardName)
{
	return GetCardData<FItemCardRangeATK>(_CardName, ECardType::E_RangeATK);
}

FItemCard UCardDataComponent::GetCardBaseData(const FString& _CardName)
{
	FItemCard CurData;
	UCardBaseStruct::SearchCardFromDataTable(_CardName,CurData);

	return CurData;
}

