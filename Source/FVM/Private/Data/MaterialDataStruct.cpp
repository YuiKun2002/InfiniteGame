// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MaterialDataStruct.h"

void UMaterialDataAssetCache::Unload_Implementation()
{
	this->Blueprint.InitDataTable(nullptr);
	this->BlueprintData.Empty();

	this->BlueprintMater.InitDataTable(nullptr);
	this->BlueprintMaterData.Empty();

	this->Change.InitDataTable(nullptr);
	this->ChangeData.Empty();

	this->Spices.InitDataTable(nullptr);
	this->SpicesData.Empty();

	this->Clover.InitDataTable(nullptr);
	this->CloverData.Empty();

	this->SkillBook.InitDataTable(nullptr);
	this->SkillBookData.Empty();

	this->Ticket.InitDataTable(nullptr);
	this->TicketData.Empty();

	this->Crystal.InitDataTable(nullptr);
	this->CrystalData.Empty();

	this->Bit.InitDataTable(nullptr);
	this->BitData.Empty();

	this->LevelKey.InitDataTable(nullptr);
	this->LevelKeyData.Empty();
}

TArray<FMaterial_CardSynthesisBlueprint_Data>& UMaterialDataAssetCache::GetBlueprint()
{
	return GetDataTableSourceData(this->BlueprintData, this->Blueprint, GLOBALASSET_MATERIAL, TEXT("Blueprint"));
}

TArray<FMaterial_CardSynthesisMaterial_Data>& UMaterialDataAssetCache::GetBlueprintMater()
{
	return GetDataTableSourceData(this->BlueprintMaterData, this->BlueprintMater, GLOBALASSET_MATERIAL, TEXT("BlueprintMater"));
}

TArray<FMaterial_CardChangeJobMaterial_Data>& UMaterialDataAssetCache::GetChange()
{
	return GetDataTableSourceData(this->ChangeData, this->Change, GLOBALASSET_MATERIAL, TEXT("Change"));
}

TArray<FMaterial_SpicesMaterial_Data>& UMaterialDataAssetCache::GetSpices()
{
	return GetDataTableSourceData(this->SpicesData, this->Spices, GLOBALASSET_MATERIAL, TEXT("Spices"));
}

TArray<FMaterial_CloverMaterial_Data>& UMaterialDataAssetCache::GetClover()
{
	return GetDataTableSourceData(this->CloverData, this->Clover, GLOBALASSET_MATERIAL, TEXT("Clover"));
}

TArray<FCardSkillBooks_Data>& UMaterialDataAssetCache::GetSkillBook()
{
	return GetDataTableSourceData(this->SkillBookData, this->SkillBook, GLOBALASSET_MATERIAL, TEXT("SkillBook"));
}

TArray<FMaterial_TicketMaterial_Data>& UMaterialDataAssetCache::GetTicket()
{
	return GetDataTableSourceData(this->TicketData, this->Ticket, GLOBALASSET_MATERIAL, TEXT("Ticket"));
}

TArray<FMaterial_Crystal_Data>& UMaterialDataAssetCache::GetCrystal()
{
	return GetDataTableSourceData(this->CrystalData, this->Crystal, GLOBALASSET_MATERIAL, TEXT("Crystal"));
}

TArray<FMaterial_Bit_Data>& UMaterialDataAssetCache::GetBit()
{
	return GetDataTableSourceData(this->BitData, this->Bit, GLOBALASSET_MATERIAL, TEXT("Bit"));
}

TArray<FMaterial_LevelKey_Data>& UMaterialDataAssetCache::GetLevelKey()
{
	return GetDataTableSourceData(this->LevelKeyData, this->LevelKey, GLOBALASSET_MATERIAL, TEXT("LevelKey"));
}
