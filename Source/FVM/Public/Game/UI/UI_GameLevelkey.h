// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_GameLevelkey.generated.h"

/**
 * 钥匙显示界面
 */

class UCanvasPanel;
class UImage;

UCLASS()
class FVM_API UUI_GameLevelkey : public UWidgetBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual bool Initialize() override;
	//初始化数据
	UFUNCTION(BlueprintCallable)
		void Init();
private:
	//钥匙界面
	UPROPERTY()
		UCanvasPanel* KeyPanel = nullptr;

	//钥匙
	UPROPERTY()
		UImage* Key0 = nullptr;
	UPROPERTY()
		class UTextBlock* KeyTip = nullptr;

	//徽章A
	UPROPERTY()
		UImage* Key1 = nullptr;
	//徽章B
	UPROPERTY()
		UImage* Key2 = nullptr;

	//物品1
	UPROPERTY()
		UImage* Item1 = nullptr;
	//物品2
	UPROPERTY()
		UImage* Item2 = nullptr;
	//物品3
	UPROPERTY()
		UImage* Item3 = nullptr;
};
