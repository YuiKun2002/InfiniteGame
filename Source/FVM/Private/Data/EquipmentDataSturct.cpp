// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EquipmentDataSturct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Math/UnrealMathUtility.h"

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

	//数据名称
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

TArray<FEquipment_Hero_Data>& UEquipmentDataAssetCache::GetHeroes()
{
	return GetDataTableSourceData(this->HeroesData, this->Heroes, GLOBALASSET_EQUIP, TEXT("Heroes"));
}

TArray<FEquipment_Weapon_Data>& UEquipmentDataAssetCache::GetWeapons()
{
	return GetDataTableSourceData(this->WeaponsData, this->Weapons, GLOBALASSET_EQUIP, TEXT("Weapons"));
}

FMainWeaponData UMainWeaponDataFunc::Calculate(const FMainWeaponData& InputData)
{
	FMainWeaponData Data = InputData;

	//计算攻击力
	int32 ATK = Data.WeaponLevel == 1 ? Data.ATK :
		Data.ATK * FMath::Pow(Data.ATKRCoefficient, (Data.WeaponLevel - 1));
	Data.ATK = ATK;

	//初始速度
	float CurSpeed = Data.AttackCoolingTime;

	//计算速度
	float Speed = Data.WeaponLevel == 1 ? Data.AttackCoolingTime :
		Data.AttackCoolingTime - (
			Data.AttackCoolingTime * FMath::Pow(Data.AttackCoolingTimeCoefficient, (Data.WeaponLevel - 1)) - Data.AttackCoolingTime
			);
	Data.AttackCoolingTime = Speed;

	//攻击前摇
	float FirstSpeed = Data.WeaponLevel == 1 ? Data.AttackFristTime :
		Data.AttackFristTime - (
			Data.AttackFristTime * FMath::Pow(Data.AttackCoolingTimeCoefficient, (Data.WeaponLevel - 1)) - Data.AttackFristTime
			);
	Data.AttackFristTime = FirstSpeed;

	//子弹间隔
	float BackSpeed = Data.WeaponLevel == 1 ? Data.AttackBackTime :
		Data.AttackBackTime - (
			Data.AttackBackTime * FMath::Pow(Data.AttackCoolingTimeCoefficient, (Data.WeaponLevel - 1)) - Data.AttackBackTime
			);
	Data.AttackBackTime = BackSpeed;

	if (Data.AttackFristTime < 0.f)
	{
		Data.AttackFristTime = 0.f;
	}

	if (Data.AttackBackTime < 0.f)
	{
		Data.AttackBackTime = 0.f;
	}

	if (Data.AttackCoolingTime < Data.AttackFristTime)
	{
		Data.AttackCoolingTime = Data.AttackFristTime;
	}


	//速度提升比例
	Data.AttackSpeedUpRate = 1.f - (Data.AttackCoolingTime / CurSpeed);

	//计算暴击率
	float Critical = Data.WeaponLevel == 1 ? Data.CriticalStrikeRate :
		Data.CriticalStrikeRate * FMath::Pow(Data.CriticalStrikeRateCoefficient, (Data.WeaponLevel - 1));

	if (Critical > 1.f)
	{
		Critical = 1.f;
	}
	Data.CriticalStrikeRate = Critical;

	return Data;
}

FMainWeaponData UMainWeaponDataFunc::CalculateNext(const FMainWeaponData& InputData)
{
	FMainWeaponData Data = InputData;
	Data.WeaponLevel += 1;
	return UMainWeaponDataFunc::Calculate(Data);
}

FItemHeroBase UItemHeroDataFunc::Calculate(const FItemHeroBase& InputData)
{
	FItemHeroBase Data = InputData;

	//计算攻击力
	int32 ATK = Data.HeroLevel == 1 ? Data.ATK :
		Data.ATK * FMath::Pow(Data.ATKRate, (Data.HeroLevel - 1));
	Data.ATK = ATK;

	//计算血量
	int32 HP = Data.HeroLevel == 1 ? Data.HP :
		Data.HP * FMath::Pow(Data.HPCofficient, (Data.HeroLevel - 1));
	Data.HP = HP;

	//计算血量
	int32 HPRate = Data.HeroLevel == 1 ? Data.HPRate :
		Data.HPRate * FMath::Pow(Data.HPRateCofficient, (Data.HeroLevel - 1));
	Data.HPRate = HPRate;

	//计算CD
	int32 CD = Data.HeroLevel == 1 ? Data.CD :
		Data.CD * FMath::Pow(Data.CDCoefficient, (Data.HeroLevel - 1));
	Data.CD = CD;

	return Data;
}

FItemHeroBase UItemHeroDataFunc::CalculateNext(const FItemHeroBase& InputData)
{
	FItemHeroBase Data = InputData;
	Data.HeroLevel += 1;
	return UItemHeroDataFunc::Calculate(Data);
}
