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

void UUI_Heroes_WeaponSlot::InitData(UUI_Weapons* UI_Weapon, const FItemWeaponBase& WeaponBaseData)
{
	//设置类
	this->UIWeapon = UI_Weapon;
	this->WeaponData = WeaponBaseData;

	//获取缓存对象
	UHeroItemDataAssetCache* CacheData = GetGameDataAssetCache<UHeroItemDataAssetCache>(HEROITEM_HEROITEM);
	FSoftObjectPath LevelPath = CacheData->GetResource(HEROITEM_ITEMLEVEL, TEXT("道具星星等级"), WeaponBaseData.StarsLevel);
	//设置外观
	this->WeaponName->SetText(WeaponBaseData.ItemName);
	this->SetButtonStyleSoft(
		this->ButtWeaponHead,
		TSoftObjectPtr<UTexture2D>(WeaponBaseData.ItemTexturePath), false, false
	);
	this->SetImageBrushByTexture(
		this->WepaonLevel,
		TSoftObjectPtr<UTexture2D>(LevelPath)
	);

	this->ButtWeaponHead->SetVisibility(ESlateVisibility::Visible);
	this->WeaponName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->WepaonLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UUI_Heroes_WeaponSlot::EquipWeapon(bool bFirst)
{
	if (IsValid(this->UIWeapon))
	{
		if (bFirst)
		{
			this->UIWeapon->MainWeaponSlot->InitData(nullptr, this->WeaponData);
		}
		else {
			this->UIWeapon->SecondaryWeaponSlot->InitData(nullptr, this->WeaponData);
		}
	}
}

void UUI_Heroes_WeaponSlot::RemoveWeapon()
{
	this->ButtWeaponHead->SetVisibility(ESlateVisibility::Collapsed);
	this->WeaponName->SetVisibility(ESlateVisibility::Collapsed);
	this->WepaonLevel->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_Heroes_WeaponSlot::Decompose(UUI_Heroes_WeaponSlot* OtherWeapon)
{
	if(IsValid(OtherWeapon))
	{
		OtherWeapon->RemoveWeapon();
		OtherWeapon->DecomposeWeapon();
		return;
	}
	this->DecomposeWeapon();
}

void UUI_Heroes_WeaponSlot::GetWeaponData(FItemWeaponBase& WeaponBaseData)
{
	WeaponBaseData = this->WeaponData;
}

void UUI_Heroes_WeaponSlot::DecomposeWeapon()
{
	
}

void UUI_Heroes_WeaponItem::SetWeaponData(const FItemWeaponBase& Data, UUI_Weapons* UIWeapon)
{
	this->WeaponData = Data;
	this->UI_Weapon = UIWeapon;
}

void UUI_Heroes_WeaponItem::InitWeaponData()
{
	if (this->WeaponData.WeaponLevel < 0)
	{
		this->WeaponData.WeaponLevel = 0;
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
	ItemLevel->SetText(FText::FromString(FString::FromInt(this->WeaponData.WeaponLevel + 1)));

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
