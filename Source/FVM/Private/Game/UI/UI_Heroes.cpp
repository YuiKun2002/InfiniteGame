// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Heroes.h"
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "Game/UI/UI_HeroItem.h"


UWidget* UUI_Heroes::WidgetCreateInitHeroes(UItemDataTable* _Data, int32 _Index)
{
	UUI_HeroItem* ItemWidget = CreateWidget<UUI_HeroItem>(this,
		UGameSystemFunction::GetUserInterClassByName(UI_HEROESUINAME,
			TEXT("HeroItem"))
	);

	ItemWidget->SetWeaponData(_Data->GetTransValue<FItemHeroBase>(), this);
	ItemWidget->InitWeaponData();
	return ItemWidget;
}

void UUI_Heroes::WidgetRefreshUpdateHeroes(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	UUI_HeroItem* ItemWidget = Cast<UUI_HeroItem>(_UWidget);
	ItemWidget->SetWeaponData(_Data->GetTransValue<FItemHeroBase>(), this);
	ItemWidget->InitWeaponData();
}

void UUI_Heroes::InitHeroes()
{
	if (!IsValid(this->ItemLoadManagerItem))
	{
		//卡片加载器
		this->ItemLoadManagerItem = NewObject<UItemLoadManager>(this, TEXT("ItemLoadManager_Heroes"));
		this->ItemLoadManagerItem->InitWidget_First(this->Uniform_Item, this->Scroll_Item);
		this->ItemLoadManagerItem->InitRange_Second(4, 3, 800, 180.f, 162.f);
		this->ItemLoadManagerItem->OnCreateItem.BindUFunction(this, TEXT("WidgetCreateInitHeroes"));
		this->ItemLoadManagerItem->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefreshUpdateHeroes"));
	}

	if (!this->Heroes.Num())
	{
		FItemHeroBase A;
		A.ItemName = FText(FText::FromName(TEXT("Cat Sword")));
		A.M_ItemID = 12;
		UItemBaseStruct::GetTextureResource(2, A.ItemTexturePath);
		A.HeroLevel = 1;
		A.BagID = TEXT("0");

		FItemHeroBase C;
		C.ItemName = FText(FText::FromName(TEXT("Cat Gun")));
		C.M_ItemID = 13;
		UItemBaseStruct::GetTextureResource(4, C.ItemTexturePath);
		C.HeroLevel = 1;
		C.BagID = TEXT("1");

		FItemHeroBase B;
		B.ItemName = FText(FText::FromName(TEXT("Cat Shield")));
		B.M_ItemID = 14;
		B.HeroLevel = 1;
		UItemBaseStruct::GetTextureResource(3, B.ItemTexturePath);
		B.BagID = TEXT("2");

		this->Heroes.Append(
			{ A,B,C }
		);
	}

	this->ItemLoadManagerItem->UpdateDatatable(
		this->Heroes
	);
	this->ItemLoadManagerItem->SetLoadItemMaxCount(
		this->Heroes.Num()
	);
	this->ItemLoadManagerItem->ContinueRun();

	this->InitData();
}

void UUI_Heroes::InitData()
{
	//初始化选择界面
	const FItemHeroBase& PlayerHeroData = this->GetCurrentHeroData();

	this->HeroName->SetText(PlayerHeroData.ItemName);


}

FItemHeroBase UUI_Heroes::GetCurrentHeroData()
{
	if (IsValid(this->CurHeroItem))
	{
		FItemHeroBase Data;
		this->CurHeroItem->GetHeroData(Data);
		return Data;
	}
	return UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData;
}
