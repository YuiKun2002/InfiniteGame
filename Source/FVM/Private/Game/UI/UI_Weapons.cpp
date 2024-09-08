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


	FItemWeaponBase A;
	A.ItemName = FText(FText::FromName(TEXT("Cat Sword")));
	A.M_ItemID = 2;
	UItemBaseStruct::GetTextureResource(2, A.ItemTexturePath);
	A.WeaponType = EWeaponType::MainWeapon;
	A.WeaponLevel = 0;

	FItemWeaponBase C;
	C.ItemName = FText(FText::FromName(TEXT("Cat Gun")));
	C.M_ItemID = 4;
	UItemBaseStruct::GetTextureResource(4, C.ItemTexturePath);
	C.WeaponType = EWeaponType::MainWeapon;
	C.WeaponLevel = 2;

	FItemWeaponBase B;
	B.ItemName = FText(FText::FromName(TEXT("Cat Shield")));
	B.M_ItemID = 3;
	B.WeaponLevel = 4;
	UItemBaseStruct::GetTextureResource(3, B.ItemTexturePath);
	B.WeaponType = EWeaponType::MainWeapon;

	//UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Weapon.Empty();
	this->Weapons.Append(
		{ A,B,C,B,A,B,A,B,A,B,A,B,C,B,A,B,A,B,A,B,C,B,A,B,A,B,A,B,A,B,A,B,A,C,A,B }
	);

	this->ItemLoadManagerItem->UpdateDatatable(
		this->Weapons
	);
	this->ItemLoadManagerItem->SetLoadItemMaxCount(
		this->Weapons.Num()
	);
	this->ItemLoadManagerItem->ContinueRun();
}

/*
void UUI_Weapons::EquipWeapon(const FItemWeaponBase& Data)
{
	// FMainWeaponData
	// FSecondaryWeaponData

	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();

	if (Data.WeaponType == EWeaponType::MainWeapon)
	{
		

		//装备到角色->检查背包
		if (Player->PlayerEquipWeaponData.MainWeapon.M_ItemID != -1)
		{

		}
		else {

		}


		this->MainWeaponSlot->ButtWeaponHead->OnClicked.Clear();
		this->MainWeaponSlot->ButtWeaponHead->OnClicked.AddDynamic(this, &UUI_Weapons::CancelMainWeapon);
	}
	else {

		

		//装备到角色->检查背包


		this->SecondaryWeaponSlot->ButtWeaponHead->OnClicked.Clear();
		this->SecondaryWeaponSlot->ButtWeaponHead->OnClicked.AddDynamic(this, &UUI_Weapons::CancelSecondaryWeapon);
	}
}

void UUI_Weapons::EquipCurrentWeapon(
	const FItemWeaponBase& BaseData,
	UUI_Heroes_WeaponSlot* WeaponSlot,
	UTextBlock* Text1,
	const FText& CText1,
	UTextBlock* Text2,
	const FText& CText2,
	UTextBlock* Text3,
	const FText& CText3
)
{
	//获取缓存对象
	UHeroItemDataAssetCache* CacheData = GetGameDataAssetCache<UHeroItemDataAssetCache>(HEROITEM_HEROITEM);
	FSoftObjectPath LevelPath = CacheData->GetResource(HEROITEM_ITEMLEVEL, TEXT("道具星星等级"), BaseData.WeaponLevel);

	//设置外观
	WeaponSlot->WeaponName->SetText(BaseData.ItemName);
	WeaponSlot->SetButtonStyleSoft(
		WeaponSlot->ButtWeaponHead,
		TSoftObjectPtr<UTexture2D>(BaseData.ItemTexturePath), false, false
	);

	WeaponSlot->SetImageBrushByTexture(
		WeaponSlot->WepaonLevel,
		TSoftObjectPtr<UTexture2D>(LevelPath)
	);

	Text1->SetText(CText1);
	Text2->SetText(CText2);
	Text3->SetText(CText3);

	WeaponSlot->ButtWeaponHead->SetVisibility(ESlateVisibility::Visible);
	WeaponSlot->WeaponName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	WeaponSlot->WepaonLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUI_Weapons::CancelMainWeapon()
{
	this->MainWeaponSlot->ButtWeaponHead->SetVisibility(ESlateVisibility::Collapsed);
	this->MainWeaponSlot->WeaponName->SetVisibility(ESlateVisibility::Collapsed);
	this->MainWeaponSlot->WepaonLevel->SetVisibility(ESlateVisibility::Collapsed);

	this->Text_Attack->SetText(FText::FromString(TEXT("???")));
	this->Text_Burst->SetText(FText::FromString(TEXT("???")));
	this->WeaponCostCoin0->SetText(FText::FromString(TEXT("0")));
}

void UUI_Weapons::CancelSecondaryWeapon()
{
	this->SecondaryWeaponSlot->ButtWeaponHead->SetVisibility(ESlateVisibility::Collapsed);
	this->SecondaryWeaponSlot->WeaponName->SetVisibility(ESlateVisibility::Collapsed);
	this->SecondaryWeaponSlot->WepaonLevel->SetVisibility(ESlateVisibility::Collapsed);

	this->Text_Defence->SetText(FText::FromString(TEXT("???")));
	this->Text_HealRate->SetText(FText::FromString(TEXT("???")));
	this->WeaponCostCoin1->SetText(FText::FromString(TEXT("0")));
}
*/

UWidget* UUI_Weapons::WidgetCreateInitWeapons(UItemDataTable* _Data, int32 _Index)
{
	UUI_Heroes_WeaponItem* ItemWidget = CreateWidget<UUI_Heroes_WeaponItem>(this,
		UGameSystemFunction::GetUserInterClassByName(UI_HEROESUINAME,
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