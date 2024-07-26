// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_TreviFountain2Panel.generated.h"

/**
 * 8连抽奖界面
 */

class UImage;

UCLASS()
class FVM_API UUI_TreviFountain2Panel : public UWidgetBase
{
	GENERATED_BODY()
private:
	//图片集合
	UPROPERTY()
		TArray<UImage*> M_Heads;
public:
	bool Initialize() override;
public:
	//播放动画
	void TPlayAnimation();
	//设置图片
	void SetItemHeads(TArray<FString>& _Heads);
};
