// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CardData/CardDataStruct.h"

void UCardDataAssetCache::Unload_Implementation()
{
	this->CardATK.InitDataTable(nullptr);
	this->CardSpawn.InitDataTable(nullptr);
	this->CardDefence.InitDataTable(nullptr);
	this->CardFunction.InitDataTable(nullptr);
	this->CardRangeATK.InitDataTable(nullptr);

	this->CardATKData.Empty();
	this->CardSpawnData.Empty();
	this->CardDefenceData.Empty();
	this->CardFunctionData.Empty();
	this->CardRangeATKData.Empty();
}

TArray<FCard_ATK_Data>& UCardDataAssetCache::GetCardATK()
{
	return GetDataTableSourceData(this->CardATKData, this->CardATK, GLOBALASSET_CARD, TEXT("ATK"));
}

TArray<FCard_Spawn_Data>& UCardDataAssetCache::GetCardSpawn()
{
	return GetDataTableSourceData(this->CardSpawnData, this->CardSpawn, GLOBALASSET_CARD, TEXT("Spawn"));
}

TArray<FCard_Defence_Data>& UCardDataAssetCache::GetCardDefence()
{
	return GetDataTableSourceData(this->CardDefenceData, this->CardDefence, GLOBALASSET_CARD, TEXT("Defence"));
}

TArray<FCard_Function_Data>& UCardDataAssetCache::GetCardFunction()
{
	return GetDataTableSourceData(this->CardFunctionData, this->CardFunction, GLOBALASSET_CARD, TEXT("Function"));
}

TArray<FCard_RangeATK_Data>& UCardDataAssetCache::GetCardRangeATK()
{
	return GetDataTableSourceData(this->CardRangeATKData, this->CardRangeATK, GLOBALASSET_CARD, TEXT("RangeATK"));
}
