// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Weapons.h"
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "Game/UI/UI_Heroes_WeaponItem.h"

void UUI_Weapons::InitWeapons()
{
	if (!IsValid(this->ItemLoadManagerItem))
	{
		//卡片加载器
		this->ItemLoadManagerItem = NewObject<UItemLoadManager>(this, TEXT("ItemLoadManager_Weapon"));
		this->ItemLoadManagerItem->InitWidget_First(this->Uniform_Item, this->Scroll_Item);
		this->ItemLoadManagerItem->InitRange_Second(4, 4, 800, 180.f, 162.f);
		this->ItemLoadManagerItem->OnCreateItem.BindUFunction(this, TEXT("WidgetCreateInitWeapons"));
		this->ItemLoadManagerItem->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefreshUpdateWeapons"));
	}

	if (!this->Weapons.Num())
	{
		FMainWeaponData A;
		A.ItemName = FText(FText::FromName(TEXT("CatSword")));
		A.M_ItemID = 15;
		UItemBaseStruct::GetTextureResource(A.M_ItemID, A.ItemTexturePath);
		A.WeaponType = EWeaponType::MainWeapon;
		A.WeaponLevel = 1;
		A.BagID = TEXT("0");

		FMainWeaponData B;
		B.ItemName = FText(FText::FromName(TEXT("CatShield")));
		B.M_ItemID = 17;
		B.WeaponLevel = 4;
		UItemBaseStruct::GetTextureResource(B.M_ItemID, B.ItemTexturePath);
		B.WeaponType = EWeaponType::MainWeapon;
		B.BagID = TEXT("2");

		FMainWeaponData C;
		C.ItemName = FText(FText::FromName(TEXT("CatGun")));
		C.M_ItemID = 16;
		UItemBaseStruct::GetTextureResource(C.M_ItemID, C.ItemTexturePath);
		C.WeaponType = EWeaponType::MainWeapon;
		C.WeaponLevel = 2;
		C.BagID = TEXT("1");

		FMainWeaponData D;
		D.ItemName = FText(FText::FromName(TEXT("CatShield")));
		D.M_ItemID = 17;
		D.WeaponLevel = 4;
		UItemBaseStruct::GetTextureResource(D.M_ItemID, D.ItemTexturePath);
		D.WeaponType = EWeaponType::MainWeapon;
		D.BagID = TEXT("3");

		FMainWeaponData E;
		E.ItemName = FText(FText::FromName(TEXT("CatSword")));
		E.M_ItemID = 15;
		UItemBaseStruct::GetTextureResource(E.M_ItemID, E.ItemTexturePath);
		E.WeaponType = EWeaponType::MainWeapon;
		E.WeaponLevel = 1;
		E.StarsLevel = 2;
		E.BagID = TEXT("4");

		//UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Weapon.Empty();
		this->Weapons.Append(
			{ A,B,C,D,E }
		);
	}

	this->ItemLoadManagerItem->UpdateDatatable(
		this->Weapons
	);
	this->ItemLoadManagerItem->SetLoadItemMaxCount(
		this->Weapons.Num()
	);
	this->ItemLoadManagerItem->ContinueRun();


	FPlayerEquipWeapon& Weapon = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerEquipWeaponData;

	bool bA = false;
	bool bB = false;
	//加载玩家装备的武器
	if (Weapon.bMainEquip)
	{
		for (const auto& A : this->Weapons)
		{
			if (A.BagID.Equals(Weapon.MainWeapon.BagID))
			{
				Weapon.MainWeapon = A;
				this->MainWeaponSlot->InitData(this, Weapon.MainWeapon, true);
				bA = true;
				break;
			}
		}
	}

	if (!bA)
	{
		Weapon.bMainEquip = false;
	}

	if (Weapon.bSecondaryEquip)
	{
		for (const auto& A : this->Weapons)
		{
			if (A.BagID.Equals(Weapon.SecondaryWeapon.BagID))
			{
				Weapon.SecondaryWeapon = A;
				this->SecondaryWeaponSlot->InitData(this, Weapon.SecondaryWeapon, false);
				bB = true;
				break;
			}
		}
	}

	if (!bB)
	{
		Weapon.bSecondaryEquip = false;
	}

	if (!bA || !bB)
	{
		UGameSystemFunction::SaveCurrentPlayerData(TEXT("武器A或者B未找到，自动卸载武器"));
	}


	UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("武器界面-数据初始化"));
}

UWidget* UUI_Weapons::WidgetCreateInitWeapons(UItemDataTable* _Data, int32 _Index)
{
	UUI_Heroes_WeaponItem* ItemWidget = CreateWidget<UUI_Heroes_WeaponItem>(this,
		UGameSystemFunction::GetUserInterClassByName(UI_WEAPONSUINAME,
			TEXT("Weapon"))
	);

	ItemWidget->SetWeaponData(_Data->GetTransValue<FMainWeaponData>(), this);
	ItemWidget->InitWeaponData();

	return ItemWidget;
}

void UUI_Weapons::WidgetRefreshUpdateWeapons(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	Cast<UUI_Heroes_WeaponItem>(_UWidget)->SetWeaponData(_Data->GetTransValue<FMainWeaponData>(), this);
	Cast<UUI_Heroes_WeaponItem>(_UWidget)->InitWeaponData();
}