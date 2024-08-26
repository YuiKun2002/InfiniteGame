// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Heroes.h"
#include "Game/UI/UI_Heroes_WeaponItem.h"

void UUI_Heroes::InitHeroes()
{

}

void UUI_Heroes::InitWeapons()
{
	if (!IsValid(this->ItemLoadManagerItem))
	{
		//卡片加载器
		this->ItemLoadManagerItem = NewObject<UItemLoadManager>(this, TEXT("ItemLoadManager_Weapon"));
		this->ItemLoadManagerItem->InitWidget_First(this->Uniform_Item, this->Scroll_Item);
		this->ItemLoadManagerItem->InitRange_Second(4, 3, 800, 180.f, 162.f);
		this->ItemLoadManagerItem->OnCreateItem.BindUFunction(this, TEXT("WidgetCreateInitWeapons"));
		this->ItemLoadManagerItem->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefreshUpdateWeapons"));
	}

	TArray<FItemWeaponBase> Weapons;
	FItemWeaponBase A;
	A.M_ItemID = 2;
	UItemBaseStruct::GetTextureResource(2, A.ItemTexturePath);
	A.WeaponType = EWeaponType::MainWeapon;

	FItemWeaponBase B;
	A.M_ItemID = 3;
	UItemBaseStruct::GetTextureResource(3, B.ItemTexturePath);
	A.WeaponType = EWeaponType::SecondaryWeapon;
	Weapons.Append({ A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B,A,B });

	this->ItemLoadManagerItem->UpdateDatatable(Weapons);
	this->ItemLoadManagerItem->SetLoadItemMaxCount(
		Weapons.Num()
	);
	this->ItemLoadManagerItem->ContinueRun();
}

UWidget* UUI_Heroes::WidgetCreateInitWeapons(UItemDataTable* _Data, int32 _Index)
{
	UUI_Heroes_WeaponItem* ItemWidget = CreateWidget<UUI_Heroes_WeaponItem>(this,
		UGameSystemFunction::GetUserInterClassByName(UI_HEROESUINAME,
			TEXT("Weapon"))
	);

	ItemWidget->SetWeaponData(_Data->GetTransValue<FItemWeaponBase>());
	ItemWidget->InitWeaponData();

	return ItemWidget;
}

void UUI_Heroes::WidgetRefreshUpdateWeapons(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	Cast<UUI_Heroes_WeaponItem>(_UWidget)->SetWeaponData(_Data->GetTransValue<FItemWeaponBase>());
	Cast<UUI_Heroes_WeaponItem>(_UWidget)->InitWeaponData();
}
