// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_HeroItem.h"
#include "Game/UI/UI_Heroes.h"
#include <Components/Image.h>
#include <Components/Button.h>

void UUI_HeroItem::SetWeaponData(const FItemHeroBase& Data, class UUI_Heroes* UIHeroes)
{
	this->HeroData = Data;
	this->UI_Heroes = UIHeroes;
}

void UUI_HeroItem::InitWeaponData()
{
	//开启
	this->Item->SetIsEnabled(true);
	//隐藏
	this->SelectBox->SetVisibility(ESlateVisibility::Collapsed);
	//设置头像
	UItemBaseStruct::GetTextureResource(
		this->HeroData.M_ItemID,
		this->HeroData.ItemTexturePath
	);
	this->SetButtonStyleSoft(this->Item, TSoftObjectPtr<UTexture2D>(this->HeroData.ItemTexturePath));

	if (IsValid(this->UI_Heroes) && IsValid(this->UI_Heroes->CurHeroItem))
	{
		if (this->HeroData.M_ItemID == this->UI_Heroes->CurHeroItem->GetHeroDataByRef().M_ItemID)
		{
			this->Select();
		}
		return;
	}
	else {
		//如果当前选择的数据与之匹配，则进行选择
		FItemHeroBase& PlayerHeroData = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData;
		if (this->HeroData.M_ItemID == PlayerHeroData.M_ItemID)
		{
			this->Select();
		}
	}
}

void UUI_HeroItem::Select()
{
	if (IsValid(UI_Heroes))
	{
		if (IsValid(this->UI_Heroes->CurHeroItem))
		{
			this->UI_Heroes->CurHeroItem->Cancel();
			this->UI_Heroes->CurHeroItem = this;
			this->Item->SetIsEnabled(false);
			this->SelectBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			this->OnSelect(this, this->UI_Heroes);
			return;
		}
		this->UI_Heroes->CurHeroItem = this;
		this->Item->SetIsEnabled(false);
		this->SelectBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->OnSelect(this, this->UI_Heroes);
	}
}

void UUI_HeroItem::Cancel()
{
	this->SelectBox->SetVisibility(ESlateVisibility::Collapsed);
	this->Item->SetIsEnabled(true);
}

void UUI_HeroItem::GetHeroData(FItemHeroBase& Data)
{
	Data = this->HeroData;
}

FItemHeroBase& UUI_HeroItem::GetHeroDataByRef()
{
	return this->HeroData;
}
