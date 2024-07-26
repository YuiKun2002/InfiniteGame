// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseTab.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>

bool UGameMapUI_MouseTab::Initialize()
{
	Super::Initialize();

	//初始化老鼠数据表
	this->MouseData = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'"));

	return true;
}

void UGameMapUI_MouseTab::InitMouseTab(UFVMEditUI_GameMapEdit* Class)
{
	this->FVMEditUI_GameMapEdit = Class;

	this->SetConfig(this->FVMEditUI_GameMapEdit->GetCurEditData().M_FMouseConfig);

	this->UpdateView();
}

void UGameMapUI_MouseTab::SetConfig(FMouseConfig MouseConfig)
{
	this->MouseConfigBuffer = MouseConfig;
}

FMouseConfig UGameMapUI_MouseTab::GetConfig()
{
	return this->MouseConfigBuffer;
}

FMouseConfig& UGameMapUI_MouseTab::GetConfigRef()
{
	return this->MouseConfigBuffer;
}

void UGameMapUI_MouseTab::UpdateView()
{
	this->InitData(this->MouseConfigBuffer);
}

UFVMEditUI_GameMapEdit* UGameMapUI_MouseTab::GetEditor()
{
	return this->FVMEditUI_GameMapEdit;
}

UDataTable* UGameMapUI_MouseTab::GetMouseData()
{
	return this->MouseData;
}
