// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Heroes_WeaponItem.h"
#include <Components/Button.h>

void UUI_Heroes_WeaponItem::SetWeaponData(const FItemWeaponBase& Data)
{
	this->WeaponData = Data;
}

void UUI_Heroes_WeaponItem::InitWeaponData()
{
	UWidgetBase::SetButtonStyleSoft(this->Item, this->WeaponData.ItemTexturePath,false,false);
}
