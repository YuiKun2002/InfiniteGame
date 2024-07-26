// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_TicketShopGrid.generated.h"

/**
 * 券兑换格子
 */

class UImage;
class UTextBlock;

UCLASS()
class FVM_API UUI_TicketShopGrid : public UWidgetBase
{
	GENERATED_BODY()
public:
	//当支付成功时触发
	FPayFinish OnPayFinish;
	//当券支付成功并且券归零（角色背包材料数组地址发生变化）时触发（更先于FPayFinish 触发）
	FTicketPayFinishOver OnPayFinishOver;
private:
	//商品数据
	FItemTicketPrice M_ItemTicketPrice_Data;
private:
	//券货币图标
	UPROPERTY()
		UImage* M_Ticket_Img = nullptr;
	//物品的头像
	UPROPERTY()
		UImage* M_ItemHead_Img = nullptr;
public:
	//商品展示名称
	UPROPERTY()
		UTextBlock* M_ItemNameText = nullptr;
	//商品展示的价格
	UPROPERTY()
		UTextBlock* M_TicketCountText = nullptr;
public:
	//设置券的图标
	void SetTicketHead(const FString& _HeadPath);
	//设置物品的头像
	void SetItemHead(const FString& _HeadPath);
	//设置商品数据
	void SetItemData(FItemTicketPrice& _data);
	//基本数据初始化
	void InitBase();
	//动画播放
	void Anim();
public:
	bool Initialize() override;
public:
	//显示支付界面
	UFUNCTION(BlueprintCallable)
		void TicketShowPayPanel();
public:
	//自动调用支付成功
	UFUNCTION()
		void AutoCall_PayFinish();
	//自动调用券支付成功
	UFUNCTION()
		void AutoCall_TicketPayFinishOver();
};
