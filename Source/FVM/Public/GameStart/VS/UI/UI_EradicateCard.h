// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_EradicateCard.generated.h"

/**
 * 铲除卡片
 */

class UButton;
class UImage;

UCLASS()
class FVM_API UUI_EradicateCard : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UImage* M_EradicateImage = nullptr;
	UPROPERTY()
		UImage* M_EradicateHead = nullptr;
public:
	virtual bool Initialize() override;
public:
	//选择
	UFUNCTION(BlueprintCallable)
		void Select();
	//取消
	UFUNCTION(BlueprintCallable)
		void Cancel();
public:
	//获取背景图
	UImage* GetBG();
};
