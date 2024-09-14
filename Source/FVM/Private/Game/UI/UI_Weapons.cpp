﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


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
		FItemWeaponBase A;
		A.ItemName = FText(FText::FromName(TEXT("Cat Sword")));
		A.M_ItemID = 2;
		UItemBaseStruct::GetTextureResource(2, A.ItemTexturePath);
		A.WeaponType = EWeaponType::MainWeapon;
		A.WeaponLevel = 1;
		A.BagID = TEXT("0");

		FItemWeaponBase C;
		C.ItemName = FText(FText::FromName(TEXT("Cat Gun")));
		C.M_ItemID = 4;
		UItemBaseStruct::GetTextureResource(4, C.ItemTexturePath);
		C.WeaponType = EWeaponType::MainWeapon;
		C.WeaponLevel = 2;
		C.BagID = TEXT("1");

		FItemWeaponBase B;
		B.ItemName = FText(FText::FromName(TEXT("Cat Shield")));
		B.M_ItemID = 3;
		B.WeaponLevel = 4;
		UItemBaseStruct::GetTextureResource(3, B.ItemTexturePath);
		B.WeaponType = EWeaponType::MainWeapon;
		B.BagID = TEXT("2");

		FItemWeaponBase D;
		D.ItemName = FText(FText::FromName(TEXT("Cat Shield")));
		D.M_ItemID = 3;
		D.WeaponLevel = 4;
		UItemBaseStruct::GetTextureResource(3, D.ItemTexturePath);
		D.WeaponType = EWeaponType::MainWeapon;
		D.BagID = TEXT("3");

		FItemWeaponBase E;
		E.ItemName = FText(FText::FromName(TEXT("Cat Sword")));
		E.M_ItemID = 2;
		UItemBaseStruct::GetTextureResource(2, E.ItemTexturePath);
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
	
	bool bA,bB;
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
		UGameSystemFunction::SaveCurrentPlayerData(TEXT("武器未找到，自动卸载武器"));
	}


	UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("武器界面-数据初始化"));
}

UWidget* UUI_Weapons::WidgetCreateInitWeapons(UItemDataTable* _Data, int32 _Index)
{
	UUI_Heroes_WeaponItem* ItemWidget = CreateWidget<UUI_Heroes_WeaponItem>(this,
		UGameSystemFunction::GetUserInterClassByName(UI_WEAPONSUINAME,
			TEXT("Weapon"))
	);

	ItemWidget->SetWeaponData(_Data->GetTransValue<FItemWeaponBase>(), this);
	ItemWidget->InitWeaponData();

	return ItemWidget;
}

void UUI_Weapons::WidgetRefreshUpdateWeapons(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	Cast<UUI_Heroes_WeaponItem>(_UWidget)->SetWeaponData(_Data->GetTransValue<FItemWeaponBase>(), this);
	Cast<UUI_Heroes_WeaponItem>(_UWidget)->InitWeaponData();
}