// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EquipmentDataSturct.h"
#include "GameSystem/Tools/GameSystemFunction.h"

void UEquipmentDataAssetCache::Unload_Implementation()
{
	this->Bag.InitDataTable(nullptr);
	this->BagData.Empty();

	this->GiftBox.InitDataTable(nullptr);
	this->GiftBoxData.Empty();

	this->WeaponFirst.InitDataTable(nullptr);
	this->WeaponFirstData.Empty();

	this->WeaponSecond.InitDataTable(nullptr);
	this->WeaponSecondData.Empty();

	this->WeaponSuper.InitDataTable(nullptr);
	this->WeaponSuperData.Empty();

	this->GemLaser.InitDataTable(nullptr);
	this->GemLaserData.Empty();

	this->GemAttack.InitDataTable(nullptr);
	this->GemAttackData.Empty();

	this->PlayerEquipmentData.Empty();

	this->Mail.InitDataTable(nullptr);
	this->MailData.Empty();
}

TArray<FEquipment_Bag_Data>& UEquipmentDataAssetCache::GetBag()
{
	return GetDataTableSourceData(this->BagData, this->Bag, GLOBALASSET_EQUIP, TEXT("Bag"));
}

TArray<FEquipment_PlayerEquipment_Data>& UEquipmentDataAssetCache::GetPlayerEquipment()
{
	if (this->PlayerEquipmentData.Num())
	{
		return this->PlayerEquipmentData;
	}

	//Êý¾ÝÃû³Æ
	FName DataName = GLOBALASSET_EQUIP;
	auto TempInitData = [&](const FName& Name) {
		UGameSystemFunction::GetDataTableRows(
			UGameDataSubsystem::GetGameDataSubsystemStatic()->GetAsset()->
			GetDataByName(DataName)->GetDataByName(Name), this->PlayerEquipmentData);
		};

	TempInitData(TEXT("BodyMan"));
	TempInitData(TEXT("BodyMen"));

	TempInitData(TEXT("HairMen"));

	TempInitData(TEXT("GlassesMen"));

	TempInitData(TEXT("CapMen"));
	TempInitData(TEXT("CapBase"));

	TempInitData(TEXT("EyeMen"));
	TempInitData(TEXT("EyeBase"));

	TempInitData(TEXT("FaceMen"));

	TempInitData(TEXT("FlyItemMan"));
	TempInitData(TEXT("FlyItemMen"));
	TempInitData(TEXT("FlyItemBase"));

	TempInitData(TEXT("SuitMan"));
	TempInitData(TEXT("SuitMen"));
	TempInitData(TEXT("SuitBase"));

	return this->PlayerEquipmentData;
}

TArray<FEquipment_GiftBox_Data>& UEquipmentDataAssetCache::GetGiftBox()
{
	return GetDataTableSourceData(this->GiftBoxData, this->GiftBox, GLOBALASSET_EQUIP, TEXT("GiftBox"));
}

TArray<FEquipment_WeaponFirst_Data>& UEquipmentDataAssetCache::GetWeaponFirst()
{
	return GetDataTableSourceData(this->WeaponFirstData, this->WeaponFirst, GLOBALASSET_EQUIP, TEXT("WeaponFirst"));
}

TArray<FEquipment_WeaponSecond_Data>& UEquipmentDataAssetCache::GetWeaponSecond()
{
	return GetDataTableSourceData(this->WeaponSecondData, this->WeaponSecond, GLOBALASSET_EQUIP, TEXT("WeaponSecond"));
}

TArray<FEquipment_WeaponSuper_Data>& UEquipmentDataAssetCache::GetWeaponSuper()
{
	return GetDataTableSourceData(this->WeaponSuperData, this->WeaponSuper, GLOBALASSET_EQUIP, TEXT("WeaponSuper"));
}

TArray<FEquipment_GemLaser_Data>& UEquipmentDataAssetCache::GetGemLaser()
{
	return GetDataTableSourceData(this->GemLaserData, this->GemLaser, GLOBALASSET_EQUIP, TEXT("GemLaser"));
}

TArray<FEquipment_GemAttack_Data>& UEquipmentDataAssetCache::GetGemAttack()
{
	return GetDataTableSourceData(this->GemAttackData, this->GemAttack, GLOBALASSET_EQUIP, TEXT("GemAttack"));
}

TArray<FEquipment_FMail_Data>& UEquipmentDataAssetCache::GetMail()
{
	return GetDataTableSourceData(this->MailData, this->Mail, GLOBALASSET_EQUIP, TEXT("Mail"));
}
