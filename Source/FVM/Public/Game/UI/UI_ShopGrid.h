// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_ShopGrid.generated.h"

class UImage;
class UButton;
class UUI_Shop;
class UTextBlock;
class UCanvasPanelSlot;


//商品格子
UCLASS()
class FVM_API UUI_ShopGrid : public UWidgetBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual bool Initialize() override;
	//显示支付界面
	UFUNCTION(BlueprintCallable)
	void ShowPayPanel();
	//设置物品提示标签
	UFUNCTION(BlueprintCallable)
	void SetItemTipsType(EItemTipsImageType _EItemTipsImageType);
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
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemName;
	//物品价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemPrice;
	//物品提示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemTips;
public:
	//数据
	UPROPERTY()
	FItemPrice M_FItemPrice;
	//商城实例(无需管理)
	UPROPERTY()
	UUI_Shop* M_UUI_Shop = nullptr;
protected:
	UFUNCTION()
	void PlayAnimation_1();
protected:
	//支付按钮
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemPay;
	//商品图片
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;
	//折扣提示
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemTips;
private:
	//物品价格类型
	UPROPERTY()
	EItemTipsImageType M_EItemTipsImageType;
};
