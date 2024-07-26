// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "LogItemPanel.generated.h"

/**
 * 日志信息项
 */

 // WidgetBlueprint'/Game/Resource/BP/Game/UI/LogPanel/BP_LogItem.BP_LogItem'

class URichTextBlock;

UCLASS()
class FVM_API ULogItemPanel : public UWidgetBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual bool Initialize() override;
public:
	UFUNCTION(BlueprintCallable)
		void SetText(const FString& Content);
private:
	UPROPERTY()
		URichTextBlock* CurText = nullptr;
};
