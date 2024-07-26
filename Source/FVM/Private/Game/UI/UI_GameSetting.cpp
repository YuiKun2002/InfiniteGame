// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_GameSetting.h"
//按钮选项卡
#include "Game/UI/GameSetting/UI_SettingButtonTab.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include <Components/ScrollBox.h>

#include "GameFramework/GameUserSettings.h"

#include <kismet/KismetSystemLibrary.h>
#include <kismet/KismetTextLibrary.h>
#include <Kismet/GameplayStatics.h>


void UUI_GameSetting::LoaddingFunc(UUI_GameSetting* _Widget, TArray<FSetting_DataTabel>& _Datas, int32 _Index)
{
	UUI_SettingButtonTab* LButtonTab = CreateWidget<UUI_SettingButtonTab>(_Widget, LoadClass<UUI_SettingButtonTab>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Setting/BPUI_SettingButtonTab.BPUI_SettingButtonTab_C'")));
	UWidgetBase* LWidget = CreateWidget<UWidgetBase>(_Widget, LoadClass<UWidgetBase>(0, *_Datas[_Index].M_TargetBlueprintRefPath));

	LButtonTab->SetButtonTitle(_Datas[_Index].M_TabName);
	LButtonTab->SetUUI_GameSetting(_Widget);
	LButtonTab->SetNeedShowWidget(LWidget);

	//添加按钮
	_Widget->M_ButtTab.Emplace(LButtonTab);

	//加载到显示列表
	this->M_UScrollBox_Content->AddChild(LButtonTab);
	this->M_UScrollBox_MainContent->AddChild(LWidget);
}

void UUI_GameSetting::LoaddingEnd(UUI_GameSetting* _Widget)
{
	if (_Widget->M_ButtTab.Num() > 0)
		_Widget->M_ButtTab[0]->Show();
}

bool UUI_GameSetting::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UScrollBox_Content = GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_62"));
	this->M_UScrollBox_MainContent = GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_34"));

	return true;
}

void UUI_GameSetting::LoadWidgets()
{
	//初始化数据
	TArray<FSetting_DataTabel> Data;
	UGameSystemFunction::GetDataTableRows(
		TEXT("DataTable'/Game/Resource/BP/Game/UI/UI_Setting/GameSettingDataTabel.GameSettingDataTabel'"),
		Data);

	//加载列表
	this->WidgetLoadding.InitClass(this);
	this->WidgetLoadding.SetClass(this, &UUI_GameSetting::LoaddingFunc);
	this->WidgetLoadding.SetEndFunction(&UUI_GameSetting::LoaddingEnd);
	this->WidgetLoadding.SetDataList(Data);
	this->WidgetLoadding.Run(0.02f);
}

void UUI_GameSetting::Refresh()
{
	for (auto LWidget : this->M_UScrollBox_MainContent->GetAllChildren())
		LWidget->SetVisibility(ESlateVisibility::Collapsed);

	for (auto LButton : this->M_ButtTab)
	{
		LButton->SetSelect(false);
		LButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[1]);
}
