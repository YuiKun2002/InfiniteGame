// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MainFramework.generated.h"

/**
 *
 */

class UImage;

UCLASS()
class FVM_API UUI_MainFramework : public UWidgetBase
{
	GENERATED_BODY()
public:
	//静态主界面
	static UUI_MainFramework* M_UUI_MainFrameworkStatic;
private:
	UPROPERTY()
		UImage* M_MailTip = nullptr;
public:
	//界面初始化
	bool  Initialize() override;
public:
	//资源初始化
	UFUNCTION(BlueprintCallable)
		void Init();
};
