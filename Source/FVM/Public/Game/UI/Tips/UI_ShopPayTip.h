// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_ShopPayTip.generated.h"

class UTextBlock;
class UButton;
class UEditableTextBox;
class UUI_Shop;

//-----------------------------------------   商城购买类型->默认购买->券购买
UENUM(BlueprintType)
enum class EShopPayTipBuyType : uint8 {
	//默认购买
	E_DefShopBuy UMETA(DisplayName = "DefShop"),
	//券购买
	E_TicketShopBuy UMETA(DisplayName = "TicketShopBuy"),
};


UCLASS()
class FVM_API UUI_ShopPayTip : public UWidgetBase
{
	GENERATED_BODY()
private:
	//通过键盘输入的物品数量
	UPROPERTY()
	TArray<FString> _Arrays_CoinNums;
public:
	//货币资源支付成功
	FPayFinish OnPayFinish;
	//券支付成功并且券个数归0(内部数组内存地址变化)
	FTicketPayFinishOver OnTicketPayFinishOver;
private:
	//输入的需要支付的物品个数
	UPROPERTY()
	UEditableTextBox* M_PlayerPayCountTextBox = nullptr;
	//支付按钮
	UPROPERTY()
	UButton* M_PlayerPayButton = nullptr;
	//输入个数按钮
	UPROPERTY()
	UButton* M_PlayerInputButton = nullptr;
public:
	//通过键盘按钮输入的数字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString _InputNumText = TEXT("1");
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemName;
	//物品描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemDescribe;
	//物品价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_ItemPirceText;
	//物品价格类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemMoneyType;
	//购买一次发送道具的数量[针对(券)购买]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_BuyCountSendNums = 1;
public:
	//默认商城数据
	UPROPERTY()
	FItemPrice M_FItemPrice;
	//券商城数据
	UPROPERTY()
	FItemTicketPrice M_FItemTicketPrice;
	//购买方式
	UPROPERTY()
	EShopPayTipBuyType M_EShopPayTipBuyType = EShopPayTipBuyType::E_DefShopBuy;
private:
	UUI_Shop* Shop_UClass = nullptr;
public:
	//设置商城类
	void SetShopUI(UUI_Shop* _UClass);
public:
	//支付
	UFUNCTION()
	void PayButton();
	//获取输入框
	UFUNCTION()
	UEditableTextBox* GetEditableTextBox();
	//获取输入数字的按钮
	UFUNCTION()
	UButton* GetInputTextButton();
public:
	//输入数字
	UFUNCTION(BlueprintCallable)
	void InputNum(FString _value);
	//删除一个
	UFUNCTION(BlueprintCallable)
	void PopText();
	//确认
	UFUNCTION(BlueprintCallable)
	void Sure();

public:
	virtual bool Initialize() override;
};
