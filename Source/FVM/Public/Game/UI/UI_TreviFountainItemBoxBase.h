// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_TreviFountainItemBoxBase.generated.h"

/**
 *   UI_TreviFountainItemBoxBase 许愿池盒子基础->用于获取奖励名称  个数 描述等
 */
UCLASS()
class FVM_API UUI_TreviFountainItemBoxBase : public UWidgetBase
{
	GENERATED_BODY()
public:
	//设置名称
	UFUNCTION(BlueprintCallable)
	void SetItemName(const FString& _ItemName);
	virtual FString ToString_Implementation() override;
public:
	//界面初始化
	bool Initialize() override;
private:
	//物品名称
	UPROPERTY()
	FString M_ItemName = TEXT("");
};
