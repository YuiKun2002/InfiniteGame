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
		FMainWeaponData Aa;
		FEquipmentBase* AaSourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("CokeGun"),
			AaSourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			Aa = *(FMainWeaponData*)(AaSourceData);
		}
		Aa.M_ItemID = 15;
		Aa.WeaponLevel = 1;
		Aa.BagID = TEXT("7");

		FMainWeaponData A;
		FEquipmentBase* ASourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("FreezeGun"),
			ASourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			A = *(FMainWeaponData*)(ASourceData);
		}
		A.M_ItemID = 16;
		A.WeaponLevel = 1;
		A.BagID = TEXT("0");


		FMainWeaponData B;
		FEquipmentBase* BSourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("CokeMachineGun"),
			BSourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			B = *(FMainWeaponData*)(BSourceData);
		}
		B.M_ItemID = 17;
		B.WeaponLevel = 4;
		B.BagID = TEXT("1");


		FMainWeaponData C;
		FEquipmentBase* CSourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("ThreeWireGun"),
			CSourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			C = *(FMainWeaponData*)(CSourceData);
		}
		C.M_ItemID = 18;
		C.WeaponLevel = 2;
		C.StarsLevel = 3;
		C.BagID = TEXT("2");

		FMainWeaponData D;
		FEquipmentBase* DSourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("FireGun"),
			DSourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			D = *(FMainWeaponData*)(DSourceData);
		}
		D.M_ItemID = 19;
		D.WeaponLevel = 4;
		D.StarsLevel = 2;
		D.BagID = TEXT("3");

		FMainWeaponData E;
		FEquipmentBase* ESourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("FreezeLaserGun"),
			ESourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			E = *(FMainWeaponData*)(ESourceData);
		}
		E.M_ItemID = 20;
		E.WeaponLevel = 1;
		E.StarsLevel = 2;
		E.BagID = TEXT("4");

		FMainWeaponData F1;
		FEquipmentBase* F1SourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("BubbleGun"),
			F1SourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			F1 = *(FMainWeaponData*)(F1SourceData);
		}
		F1.M_ItemID = 21;
		F1.WeaponLevel = 1;
		F1.StarsLevel = 2;
		F1.BagID = TEXT("5");

		FMainWeaponData G;
		FEquipmentBase* GSourceData;
		if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(
			TEXT("LaserTrackerGun"),
			GSourceData, true, EEquipment::E_PlayerWeaponFirst
		))
		{
			G = *(FMainWeaponData*)(GSourceData);
		}
		G.M_ItemID = 22;
		G.WeaponLevel = 1;
		G.StarsLevel = 2;
		G.BagID = TEXT("6");

		//UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Weapon.Empty();
		this->Weapons.Append(
			{ Aa,A,B,C,D,E,F1,G }
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