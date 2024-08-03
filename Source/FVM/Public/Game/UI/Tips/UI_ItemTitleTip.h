// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_ItemTitleTip.generated.h"

/**
 * 争对Windows平台   当鼠标移入某个地方，显示标题
 */

UCLASS()
class FVM_API UUI_ItemTitleTip : public UWidgetBase
{
	GENERATED_BODY()
public:
	//显示的标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_Title = TEXT("冻布丁");
public:
	//是否显示
	UFUNCTION(BlueprintCallable)
	void SetShow(bool _bShow, const FString& _Title);
	//--------------------------------------------------[UI界面]-------------------------------------------------------
	//创建一个基础标题提示(显示名称，是否显示)
	UFUNCTION(BlueprintCallable)
	void CreateBaseTipWidget(const FString& _ShowTitle, bool _bShow);
	//--------------------------------------------------[UI界面]-------------------------------------------------------
	UFUNCTION(BlueprintImplementableEvent)
	void SetText(const FString& Title);
private:
	//是否隐藏
	UPROPERTY()
	bool M_bHidd = true;
};
