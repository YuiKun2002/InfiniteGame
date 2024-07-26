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
private:
	//是否隐藏
	bool M_bHidd = true;
public:
	//显示的标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Title = TEXT("冻布丁");
public:
	//初始化
	bool Initialize() override;
public:
	//是否显示
	UFUNCTION(BlueprintCallable)
		void SetShow(bool _bShow, const FString& _Title);
};
