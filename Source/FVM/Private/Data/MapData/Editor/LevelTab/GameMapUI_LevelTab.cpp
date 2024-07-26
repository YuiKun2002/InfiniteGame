// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/LevelTab/GameMapUI_LevelTab.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>

bool UGameMapUI_LevelTab::Initialize()
{
	Super::Initialize();



	return false;
}

void UGameMapUI_LevelTab::InitLevelTab(UFVMEditUI_GameMapEdit* Class)
{
	this->FVMEditUI_GameMapEdit = Class;

	//初始化关卡配置
	this->InitData(Class->GetCurEditData().M_FLevelConfig);
	//缓存配置
	this->LevelConfigChanged(Class->GetCurEditData().M_FLevelConfig);
}

FLevelConfig& UGameMapUI_LevelTab::GetConfig()
{
	return this->LevelConfigBuffer;
}

void UGameMapUI_LevelTab::LevelConfigChanged(FLevelConfig Config)
{
	this->LevelConfigBuffer = Config;
}
