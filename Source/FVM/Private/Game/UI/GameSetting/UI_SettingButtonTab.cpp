// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/GameSetting/UI_SettingButtonTab.h"
#include "Game/UI/UI_GameSetting.h"

bool UUI_SettingButtonTab::Initialize()
{
	if (!Super::Initialize())
		return false;
	return true;
}

void UUI_SettingButtonTab::SetUUI_GameSetting(UUI_GameSetting* _UI_)
{
	this->M_UI_UUI_GameSetting = _UI_;
}

void UUI_SettingButtonTab::SetNeedShowWidget(UWidgetBase* _Widget)
{
	this->M_NeedShowWidget = _Widget;
}

void UUI_SettingButtonTab::SetButtonTitle(const FString& _Title)
{
	this->M_ButtTitle = _Title;
}

void UUI_SettingButtonTab::SetSelect(bool _bSelect)
{
	this->M_bSelect = _bSelect;
}

bool UUI_SettingButtonTab::GetSelect()
{
	return this->M_bSelect;
}

void UUI_SettingButtonTab::Show()
{
	this->M_UI_UUI_GameSetting->Refresh();
	this->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
	this->M_NeedShowWidget->SetVisibility(ESlateVisibility::Visible);
	this->M_bSelect = true;
}
