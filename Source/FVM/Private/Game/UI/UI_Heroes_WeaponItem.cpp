// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Heroes_WeaponItem.h"
#include <Components/Image.h>
#include "Game/UI/UI_Heroes.h"
#include "Game/UI/UI_Weapons.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "GameSystem/GameDataSubsystem.h"

void UUI_Heroes_WeaponSlot::InitData(UUI_Weapons* UI_Weapon, const FMainWeaponData& WeaponBaseData, bool bFrist)
{
	//设置类
	this->UIWeapon = UI_Weapon;
	this->WeaponData = WeaponBaseData;
	this->bFristWeapon = bFrist;

	//获取缓存对象
	UHeroItemDataAssetCache* CacheData = GetGameDataAssetCache<UHeroItemDataAssetCache>(HEROITEM_HEROITEM);
	FSoftObjectPath LevelPath = CacheData->GetResource(HEROITEM_ITEMLEVEL, TEXT("道具星星等级"), WeaponBaseData.StarsLevel);
	//设置名称
	this->WeaponName->SetText(WeaponBaseData.ItemName);
	//设置头像外观
	this->SetButtonStyleSoft(
		this->ButtWeaponHead,
		TSoftObjectPtr<UTexture2D>(WeaponBaseData.ItemTexturePath), false, false
	);
	//设置星星外观
	this->SetImageBrushByTexture(
		this->WepaonLevel,
		TSoftObjectPtr<UTexture2D>(LevelPath)
	);
	//设置等级
	this->ItemLevel->SetText(FText::FromString(FString::FromInt(WeaponBaseData.WeaponLevel)));

	this->ButtWeaponHead->SetVisibility(ESlateVisibility::Visible);
	this->WeaponName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->WepaonLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->ItemLV->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->ItemLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UUI_Heroes_WeaponSlot::InitEquipPosition(bool bFrist)
{
	this->bFristWeapon = bFrist;
}

void UUI_Heroes_WeaponSlot::EquipWeapon(bool bFirst)
{
	if (IsValid(this->UIWeapon))
	{
		//
		this->bFristWeapon = bFirst;
		//装备此武器
		FPlayerEquipWeapon& Weapon = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerEquipWeaponData;
		if (bFirst)
		{
			this->UIWeapon->MainWeaponSlot->InitData(nullptr, this->WeaponData);
			Weapon.MainWeapon = this->WeaponData;
			Weapon.bMainEquip = true;
		}
		else {
			this->UIWeapon->SecondaryWeaponSlot->InitData(nullptr, this->WeaponData);
			Weapon.SecondaryWeapon = this->WeaponData;
			Weapon.bSecondaryEquip = true;
		}
		//存档
		UGameSystemFunction::SaveCurrentPlayerData(TEXT("角色武器装备") + FString(this->bFristWeapon ? TEXT("A") : TEXT("B")));
	}
}

void UUI_Heroes_WeaponSlot::RemoveWeapon()
{
	this->ButtWeaponHead->SetVisibility(ESlateVisibility::Collapsed);
	this->WeaponName->SetVisibility(ESlateVisibility::Collapsed);
	this->WepaonLevel->SetVisibility(ESlateVisibility::Collapsed);
	this->ItemLV->SetVisibility(ESlateVisibility::Collapsed);
	this->ItemLevel->SetVisibility(ESlateVisibility::Collapsed);

	//装备此武器
	FPlayerEquipWeapon& Weapon = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerEquipWeaponData;
	if (this->bFristWeapon)
	{
		Weapon.MainWeapon = {};
		Weapon.bMainEquip = false;
	}
	else {
		Weapon.SecondaryWeapon = {};
		Weapon.bSecondaryEquip = false;
	}
	//存档
	UGameSystemFunction::SaveCurrentPlayerData(TEXT("角色武器卸载：") + FString(this->bFristWeapon ? TEXT("A") : TEXT("B")));
}

void UUI_Heroes_WeaponSlot::Decompose(UUI_Heroes_WeaponSlot* OtherWeapon)
{
	if (IsValid(OtherWeapon))
	{
		OtherWeapon->RemoveWeapon();
		OtherWeapon->DecomposeWeapon();
		return;
	}
	this->DecomposeWeapon();
}

void UUI_Heroes_WeaponSlot::Upgrade()
{
	if (!IsValid(this->UIWeapon))
	{
		return;
	}

	//修改玩家装备的武器数据
	FPlayerEquipWeapon& Weapon = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerEquipWeaponData;
	if (Weapon.bMainEquip)
	{
		if (Weapon.MainWeapon.BagID.Equals(this->WeaponData.BagID))
		{
			this->UpdageWeapon(Weapon.MainWeapon);
		}
	}

	if (Weapon.bSecondaryEquip)
	{
		if (Weapon.SecondaryWeapon.BagID.Equals(this->WeaponData.BagID))
		{
			this->UpdageWeapon(Weapon.SecondaryWeapon);
		}
	}

	//修改存储武器的背包数据
	for (auto& SourceWeaponData : this->UIWeapon->Weapons)
	{
		if (SourceWeaponData.BagID.Equals(this->WeaponData.BagID))
		{
			this->UpdageWeapon(SourceWeaponData);
			break;
		}
	}

	//修改当前槽位的武器数据
	this->UpdageWeapon(this->WeaponData);
}

void UUI_Heroes_WeaponSlot::Evolve()
{
	if (!IsValid(this->UIWeapon))
	{
		return;
	}

	//修改玩家装备的武器数据
	FPlayerEquipWeapon& Weapon = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerEquipWeaponData;
	if (Weapon.bMainEquip)
	{
		if (Weapon.MainWeapon.BagID.Equals(this->WeaponData.BagID))
		{
			this->EvolveWeapon(Weapon.MainWeapon);
		}
	}

	if (Weapon.bSecondaryEquip)
	{
		if (Weapon.SecondaryWeapon.BagID.Equals(this->WeaponData.BagID))
		{
			this->EvolveWeapon(Weapon.SecondaryWeapon);
		}
	}

	//修改存储武器的背包数据
	for (auto& SourceWeaponData : this->UIWeapon->Weapons)
	{
		if (SourceWeaponData.BagID.Equals(this->WeaponData.BagID))
		{
			this->EvolveWeapon(SourceWeaponData);
			break;
		}
	}

	//修改当前槽位的武器数据
	this->EvolveWeapon(this->WeaponData);
}

void UUI_Heroes_WeaponSlot::GetWeaponData(FMainWeaponData& WeaponBaseData)
{
	WeaponBaseData = this->WeaponData;
}

int32 UUI_Heroes_WeaponSlot::GetWeaponLevel()
{
	return this->WeaponData.WeaponLevel;
}

int32 UUI_Heroes_WeaponSlot::GetWeaponStars()
{
	return this->WeaponData.StarsLevel;
}

void UUI_Heroes_WeaponSlot::GetWeaponDetailData(FMainWeaponData& MainWeaponData)
{
	MainWeaponData = UMainWeaponDataFunc::Calculate(this->WeaponData);
}

bool UUI_Heroes_WeaponSlot::GetUpgradeWeapon()
{
	if (!IsValid(this->UIWeapon))
	{
		return false;
	}

	if (this->WeaponData.WeaponLevel < WEAPON_MAXLEVEL)
	{
		return true;
	}

	return false;
}

bool UUI_Heroes_WeaponSlot::GetEvolveWeapon()
{
	if (!IsValid(this->UIWeapon))
	{
		return false;
	}

	if (this->WeaponData.StarsLevel < 4)
	{
		int32 CurStarsLevel = this->WeaponData.WeaponLevel / 10;
		if (CurStarsLevel - this->WeaponData.StarsLevel > 0)
		{
			return true;
		}
	}

	return false;
}

class UUI_Weapons* UUI_Heroes_WeaponSlot::GetUIWeapon()
{
	return this->UIWeapon;
}

void UUI_Heroes_WeaponSlot::DecomposeWeapon()
{
	//销毁武器
	if (!IsValid(this->UIWeapon))
	{
		return;
	}

	for (auto Pt = this->UIWeapon->Weapons.CreateIterator(); Pt; ++Pt)
	{
		if (Pt->BagID.Equals(this->WeaponData.BagID))
		{
			Pt.RemoveCurrent();
			break;
		}
	}
}

void UUI_Heroes_WeaponSlot::UpdageWeapon(FMainWeaponData& Data)
{
	if (Data.WeaponLevel < WEAPON_MAXLEVEL)
	{
		Data.WeaponLevel++;
	}
}

void UUI_Heroes_WeaponSlot::EvolveWeapon(FMainWeaponData& Data)
{
	if (Data.StarsLevel < 4)
	{
		Data.StarsLevel++;
	}
}

void UUI_Heroes_WeaponItem::SetWeaponData(const FMainWeaponData& Data, UUI_Weapons* UIWeapon)
{
	this->WeaponData = Data;
	this->UI_Weapon = UIWeapon;
}

void UUI_Heroes_WeaponItem::InitWeaponData()
{
	if (this->WeaponData.WeaponLevel < 1)
	{
		this->WeaponData.WeaponLevel = 1;
	}
	else if (this->WeaponData.WeaponLevel > WEAPON_MAXLEVEL)
	{
		this->WeaponData.WeaponLevel = WEAPON_MAXLEVEL;
	}

	if (this->WeaponData.StarsLevel < 0)
	{
		this->WeaponData.StarsLevel = 0;
	}
	else if (this->WeaponData.StarsLevel > 5)
	{
		this->WeaponData.StarsLevel = 5;
	}

	//设置按钮外观
	UWidgetBase::SetButtonStyleSoft(this->Item,
		TSoftObjectPtr<UTexture2D>(this->WeaponData.ItemTexturePath)
		, false, false);

	//设置物品等级
	ItemLevel->SetText(FText::FromString(FString::FromInt(this->WeaponData.WeaponLevel)));

	//获取缓存对象
	UHeroItemDataAssetCache* Data = GetGameDataAssetCache<UHeroItemDataAssetCache>(HEROITEM_HEROITEM);
	FSoftObjectPath LevelPath = Data->GetResource(HEROITEM_ITEMLEVEL, TEXT("道具星星等级"), this->WeaponData.StarsLevel);

	//设置星级
	UWidgetBase::SetImageBrushByTexture(
		this->ItemStars,
		TSoftObjectPtr<UTexture2D>(LevelPath)
	);

	this->Item->SetIsEnabled(true);

	//修改玩家装备的武器数据
	FPlayerEquipWeapon& Weapon = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerEquipWeaponData;
	if (Weapon.bMainEquip)
	{
		if (Weapon.MainWeapon.M_ItemID == this->WeaponData.M_ItemID)
		{
			this->Item->SetIsEnabled(false);
		}
	}

	if (Weapon.bSecondaryEquip)
	{
		if (Weapon.SecondaryWeapon.M_ItemID == this->WeaponData.M_ItemID)
		{
			this->Item->SetIsEnabled(false);
		}
	}
}

void UUI_Heroes_WeaponItem::SelectWeapon()
{
	if (IsValid(this->UI_Weapon))
	{
		this->OnSelectWeaponData(this->UI_Weapon, this->WeaponData);
	}
}
