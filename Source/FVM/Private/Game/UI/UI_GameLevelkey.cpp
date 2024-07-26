// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_GameLevelkey.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"
#include <Components/CanvasPanel.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

bool UUI_GameLevelkey::Initialize()
{
	Super::Initialize();

	this->KeyPanel = this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_141"));

	this->Key0 = this->GetWidgetComponent<UImage>(this, TEXT("Key"));
	this->KeyTip = this->GetWidgetComponent<UTextBlock>(this, TEXT("KeyTip"));

	this->Key1 = this->GetWidgetComponent<UImage>(this, TEXT("B1"));
	this->Key2 = this->GetWidgetComponent<UImage>(this, TEXT("B2"));

	this->Item1 = this->GetWidgetComponent<UImage>(this, TEXT("Head1"));
	this->Item2 = this->GetWidgetComponent<UImage>(this, TEXT("Head2"));
	this->Item3 = this->GetWidgetComponent<UImage>(this, TEXT("Head3"));

	return true;
}

void UUI_GameLevelkey::Init()
{
	//加载关卡配置
	const FLevelConfig& LevelData = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;

	//显示界面
	if (LevelData.LevelKeyConfig.bKey)
	{
		this->KeyPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->KeyPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	//初始化默认显示
	if (LevelData.LevelTimeWidthLevelConfig.bEnable)
	{
		this->KeyTip->SetText(FText::FromString(TEXT("限时挑战")));

		UWidgetBase::SetImageBrush(
			this->Key0, TEXT("Texture2D'/Game/Resource/Texture/UI/GamePrepare/Key/LevelKey_3.LevelKey_3'"),
			COLOR_WHITE);
	}
	else {
		UWidgetBase::SetImageBrush(
			this->Key0, TEXT("Texture2D'/Game/Resource/Texture/UI/GamePrepare/Key/LevelKey_1.LevelKey_1'"),
			COLOR_WHITE);
	}

	this->KeyTip->SetText(FText::FromString(TEXT("???")));
	UWidgetBase::SetImageBrush(this->Key1, TEXT(""), COLOR_WHITE);
	UWidgetBase::SetImageBrush(this->Key2, TEXT(""), COLOR_WHITE);

	UWidgetBase::SetImageBrush(this->Item1,
		TEXT("Texture2D'/Game/Resource/Texture/UI/GamePrepare/Key/LevelKey_2.LevelKey_2'"), COLOR_WHITE);
	UWidgetBase::SetImageBrush(this->Item2,
		TEXT("Texture2D'/Game/Resource/Texture/UI/GamePrepare/Key/LevelKey_1.LevelKey_1'"), COLOR_WHITE);
	UWidgetBase::SetImageBrush(this->Item3,
		TEXT("Texture2D'/Game/Resource/Texture/UI/GamePrepare/Key/LevelKey_1.LevelKey_1'"), COLOR_WHITE);


	if (LevelData.LevelKeyConfig.bKey)
	{
		if (!LevelData.LevelTimeWidthLevelConfig.bEnable)
		{
			float Rate = 1.f;
			//查询钥匙是否存在
			if (UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(LevelData.LevelKeyConfig.Key1Name) == -1)
			{
				Rate = 0.1f;
			}

			this->KeyTip->SetText(FText::FromString(LevelData.LevelKeyConfig.Key1Name));

			//加载对应图片
			UWidgetBase::SetImageBrush(this->Key0, LevelData.LevelKeyConfig.Key1.ToString(), COLOR_WHITE * Rate);
		}
	}
	else {
		float Rate = 1.f;
		float Rate2 = 1.f;
		//查询钥匙是否存在
		if (UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(
			LevelData.LevelKeyConfig.Key1Name) == -1)
		{
			Rate = 0.1f;
		}

		//查询钥匙是否存在
		if (UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(
			LevelData.LevelKeyConfig.Key2Name) == -1)
		{
			Rate2 = 0.1f;
		}
		UWidgetBase::SetImageBrush(this->Key1, LevelData.LevelKeyConfig.Key1.ToString(), COLOR_WHITE * Rate);
		UWidgetBase::SetImageBrush(this->Key2, LevelData.LevelKeyConfig.Key2.ToString(), COLOR_WHITE * Rate2);
	}

	if (!LevelData.LevelKeyConfig.Item1.ToString().Equals(TEXT("")))
	{
		UWidgetBase::SetImageBrush(this->Item1, LevelData.LevelKeyConfig.Item1.ToString(), COLOR_WHITE);
	}

	if (!LevelData.LevelKeyConfig.Item2.ToString().Equals(TEXT("")))
	{
		UWidgetBase::SetImageBrush(this->Item2, LevelData.LevelKeyConfig.Item2.ToString(), COLOR_WHITE);
	}

	if (!LevelData.LevelKeyConfig.Item3.ToString().Equals(TEXT("")))
	{
		UWidgetBase::SetImageBrush(this->Item3, LevelData.LevelKeyConfig.Item3.ToString(), COLOR_WHITE);
	}
}
