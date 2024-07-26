// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MapTab/GameMapUI_MapTab.h"
#include "Data/MapData/Editor/MapTab/GameMapUI_MapViewEditor.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>

bool UGameMapUI_MapTab::Initialize()
{
	Super::Initialize();



	return true;
}

void UGameMapUI_MapTab::InitMapTab(UFVMEditUI_GameMapEdit* Class)
{
	this->EditorClass = Class;

	this->UpdateConfig(Class->GetCurEditData().M_FGameMapStruct);

	this->OnInit();
}

UFVMEditUI_GameMapEdit* UGameMapUI_MapTab::GetEditor()
{
	return this->EditorClass;
}

UGameMapUI_MapTab* UGameMapUI_MapTab::GetMapTab()
{
	return this;
}

void UGameMapUI_MapTab::UpdateConfig(FGameMapStruct GameMapStruct)
{
	this->CurGameMapStructBuffer = GameMapStruct;
}

FGameMapStruct UGameMapUI_MapTab::GetConfig()
{
	return  this->CurGameMapStructBuffer;
}

FGameMapStruct& UGameMapUI_MapTab::GetConfigRef()
{
	return this->CurGameMapStructBuffer;
}
