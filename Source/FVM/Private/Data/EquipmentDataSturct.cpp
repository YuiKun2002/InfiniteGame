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

TArray<FItemHeroBase_Data>& UEquipmentDataAssetCache::GetHeroeDetailDatas()
{
	return GetDataTableSourceData(this->HeroeDetailDatas, this->HeroeDetails, GLOBALASSET_EQUIP, TEXT("HeroeDetails"));
}

TArray<FEquipment_Weapon_Data>& UEquipmentDataAssetCache::GetWeapons()
{
	return GetDataTableSourceData(this->WeaponsData, this->Weapons, GLOBALASSET_EQUIP, TEXT("Weapons"));
}

TArray<FMainWeaponData_Data>& UEquipmentDataAssetCache::GetWeaponDetailDatas()
{
	return GetDataTableSourceData(this->WeaponDetailDatas, this->WeaponDetails, GLOBALASSET_EQUIP, TEXT("WeaponDetails"));
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
	if (Speed < 0.1f)
	{
		Speed = 0.1f;
	}

	Data.AttackCoolingTime = Speed;

	//子弹间隔
	float BackSpeed = Data.WeaponLevel == 1 ? Data.AttackBackTime :
		Data.AttackBackTime - (
			Data.AttackBackTime *
			FMath::Pow(Data.AttackCoolingTimeCoefficient, (Data.WeaponLevel - 1)) - Data.AttackBackTime
			);

	Data.AttackBackTime = BackSpeed;

	if (Data.AttackBackTime < 0.f)
	{
		Data.AttackBackTime = 0.f;
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

FMainWeaponData UMainWeaponDataFunc::Calculate_AttackSpeed(const FMainWeaponData& InputData, float Rate)
{
	if (Rate < 0.f)
	{
		Rate = 0.f;
	}
	else if (Rate > 1.f)
	{
		Rate = 1.f;
	}

	float CurRate = 1.f - Rate;
	if (Rate >= 1.f)
	{
		CurRate = 1.f;
	}
	else if (Rate <= 0.f) {
		CurRate = 0.f;
	}

	FMainWeaponData Data = InputData;
	float CurSpeed = Data.AttackCoolingTime;
	Data.AttackCoolingTime = Data.AttackCoolingTime * CurRate;
	if (Data.AttackCoolingTime < 0.1f)
	{
		Data.AttackCoolingTime = 0.1f;
	}
	Data.AttackSpeedUpRate = 1.f - (Data.AttackCoolingTime / CurSpeed);

	return Data;
}

FMainWeaponData UMainWeaponDataFunc::Calculate_Attack(const FMainWeaponData& InputData, float Rate)
{

	FMainWeaponData Data = InputData;

	Data.ATK = Data.ATK * (1.f + Rate);

	return Data;

}

FMainWeaponData UMainWeaponDataFunc::Calculate_AttackCount(const FMainWeaponData& InputData, int32 AttackCount)
{
	FMainWeaponData Data = InputData;

	Data.AttackCount = AttackCount;

	return Data;
}

FMainWeaponData UMainWeaponDataFunc::Calculate_LineTraceSettings(const FMainWeaponData& InputData, const TArray<FLineTraceSetting>& LineTraceSettings)
{
	FMainWeaponData Data = InputData;

	Data.LineTraceSettings = LineTraceSettings;

	return Data;
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

	return Data;
}

FItemHeroBase UItemHeroDataFunc::CalculateNext(const FItemHeroBase& InputData)
{
	FItemHeroBase Data = InputData;
	Data.HeroLevel += 1;
	return UItemHeroDataFunc::Calculate(Data);
}

float UItemHeroDataFunc::CalculateRate(int32 BaseLevel, int32 Level, float Rate, float Base)
{
	return (Level <= BaseLevel) ? Base :
		Base * FMath::Pow(Rate, (Level - 1));
}

int32 UItemHeroDataFunc::GetHeroLevel(const FItemHeroBase& Data)
{
	return Data.HeroLevel;
}
