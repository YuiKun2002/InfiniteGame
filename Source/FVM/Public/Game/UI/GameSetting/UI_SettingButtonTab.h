// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_SettingButtonTab.generated.h"

/**
 * 设置按钮选项卡
 */

class UUI_GameSetting;

UCLASS()
class FVM_API UUI_SettingButtonTab : public UWidgetBase
{
	GENERATED_BODY()
private:
	//设置界面
	UPROPERTY()
		UUI_GameSetting* M_UI_UUI_GameSetting = nullptr;
	//需要显示的界面
	UPROPERTY()
		UWidgetBase* M_NeedShowWidget = nullptr;
public:
	//标题文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ButtTitle = TEXT("");
	//是否被选中
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bSelect = false;
public:
	//初始化
	bool Initialize() override;
public:
	//设置UI
	void SetUUI_GameSetting(UUI_GameSetting* _UI_);
	//设置显示的界面
	void SetNeedShowWidget(UWidgetBase* _Widget);
	//设置标题名称
	void SetButtonTitle(const FString& _Title);
	//是否被选择
	void SetSelect(bool _bSelect);
public:
	//是否被选择
	UFUNCTION(BlueprintCallable)
		bool GetSelect();
public:
	//ShowPanel->显示界面
	UFUNCTION(BlueprintCallable)
		void Show();
};
