// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Heroes_WeaponItem.h"
#include <Components/Image.h>
#include "Game/UI/UI_Heroes.h"
#include "Game/UI/UI_Weapons.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "GameSystem/GameDataSubsystem.h"

FSoftObjectPath UHeroItemDataAssetCache::GetResource(const FName& Name, const FName& RowName, const int32& Key)
{
	if (this->ResourceName.IsEqual(Name))
	{
		if (this->Datas.Num())
		{
			//查询
			FSoftObjectPath* TargetData = Datas.Find(Key);
			if (TargetData)
			{
				return *TargetData;
			}
			return FSoftObjectPath();
		}
	}
	//清理数据
	this->Datas.Empty();
	//初始化
	this->Data.InitDataTable(
		UGameDataSubsystem::GetGameDataSubsystemStatic()->GetAsset()->
		GetDataByName(GLOBALASSET_LEVELTEXTURE_RESOURCE)->GetDataByName(Name)
	);
	//获取行
	FItemResourceData RowData = this->Data.GetDataByName(RowName);
	//释放
	this->Data.InitDataTable(nullptr);
	//填充数据
	Datas.Append(RowData.Resource.Resources);
	//查询
	FSoftObjectPath* TargetData = Datas.Find(Key);
	if (TargetData)
	{
		return *TargetData;
	}
	return FSoftObjectPath();
}

void UUI_Heroes_WeaponSlot::InitData(UUI_Weapons* UI_Weapon, const FItemWeaponBase& WeaponBaseData, bool bFrist)
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
			Weapon.MainWeaponData;
			Weapon.bMainEquip = true;
		}
		else {
			this->UIWeapon->SecondaryWeaponSlot->InitData(nullptr, this->WeaponData);
			Weapon.SecondaryWeapon = this->WeaponData;
			Weapon.SecondaryWeaponData;
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
		Weapon.MainWeaponData = {};
		Weapon.bMainEquip = false;
	}
	else {
		Weapon.SecondaryWeapon = {};
		Weapon.SecondaryWeaponData = {};
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
	else if (Weapon.bSecondaryEquip)
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

}

void UUI_Heroes_WeaponSlot::GetWeaponData(FItemWeaponBase& WeaponBaseData)
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
	FMainWeaponData A = { this->WeaponData };

	MainWeaponData = A;
}

void UUI_Heroes_WeaponSlot::SetWeaponLevel(int32 Level)
{
	if (Level < 1)
	{
		Level = 1;
	}
	else if (Level > 50)
	{
		Level = 50;
	}
	this->WeaponData.WeaponLevel = Level;
}

void UUI_Heroes_WeaponSlot::SetWeaponStars(int32 Level)
{
	if (Level < 0)
	{
		Level = 0;
	}
	else if (Level > 4)
	{
		Level = 4;
	}
	this->WeaponData.StarsLevel = Level;
}

void UUI_Heroes_WeaponSlot::DecomposeWeapon()
{
	//销毁武器
}

void UUI_Heroes_WeaponSlot::UpdageWeapon(FItemWeaponBase& Data)
{
	if (Data.WeaponLevel < 50)
	{
		Data.WeaponLevel++;
	}
}

void UUI_Heroes_WeaponItem::SetWeaponData(const FItemWeaponBase& Data, UUI_Weapons* UIWeapon)
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
	else if (this->WeaponData.WeaponLevel > 50)
	{
		this->WeaponData.WeaponLevel = 50;
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
}

void UUI_Heroes_WeaponItem::SelectWeapon()
{
	if (IsValid(this->UI_Weapon))
	{
		this->OnSelectWeaponData(this->UI_Weapon, this->WeaponData);
	}
}
