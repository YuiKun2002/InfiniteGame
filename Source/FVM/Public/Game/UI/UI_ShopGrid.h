// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_ShopGrid.generated.h"

class UButton;
class UImage;
class UCanvasPanelSlot;

class UUI_Shop;

UCLASS()
class FVM_API UUI_ShopGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UButton* M_BuyItem_Butt = nullptr;
	UPROPERTY()
		UImage* M_ItemTips_Image = nullptr;
	UPROPERTY()
		UImage* M_ItemHead_Image = nullptr;
private:
	//物品价格类型
	EItemTipsImageType M_EItemTipsImageType;
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemName;
	//物品提示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemTips;
	//物品价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemPrice;
public:
	//数据
	FItemPrice M_FItemPrice;
	//商城实例(无需管理)
	UUI_Shop* M_UUI_Shop = nullptr;
public:
	virtual bool Initialize() override;
	
	void PlayAnimation_1();
public:
	//显示支付界面
	UFUNCTION()
		void ShowPayPanel();
public:
	//获取按钮组件
	UFUNCTION(BlueprintCallable)
		UButton* GetBuyItemButton();
	//获取头像组件
	UFUNCTION(BlueprintCallable)
		UImage* GetItemHeadImage();
	//获取物品价格类型
	UFUNCTION(BlueprintCallable)
		EItemTipsImageType GetEItemTipsImageType();
public:
	//设置物品提示标签
	UFUNCTION(BlueprintCallable)
		void SetItemTipsType(EItemTipsImageType _EItemTipsImageType);
};
