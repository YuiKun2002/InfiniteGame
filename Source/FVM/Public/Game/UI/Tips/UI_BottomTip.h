// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_BottomTip.generated.h"

/**
 * 底部 提示框
 */

UCLASS()
class FVM_API UUI_BottomTip : public UWidgetBase
{
	GENERATED_BODY()
public:
	//设置标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Title;
public:
	//设置标题
	UFUNCTION(BlueprintCallable)
		void SetTitle(const FString& _Title);
	//设置标题并播放
	UFUNCTION(BlueprintCallable)
		void SetTitleAndPlay(const FString& _Title);
public:
	//播放动画
	UFUNCTION(BlueprintCallable)
		void PlayAnim();
public:
	//移出界面
	UFUNCTION(BlueprintCallable)
		void RemoveThisWidget();
public:
	bool Initialize() override;
};
