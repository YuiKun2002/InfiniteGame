// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "Data/MaterialDataStruct.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_TicketShop_0.generated.h"

/**
 * 生肖兑换商城
 */

class UUI_TicketShopGrid;
class UUniformGridPanel;
class UScrollBox;
class UButton;

UCLASS()
class FVM_API UUI_TicketShop_0 : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		TArray<UButton*> M_ButtonTables;

	//切换样式
	TArray<FString> M_ButtTab_Def_ = { FString(TEXT("1")),FString(TEXT("3")),FString(TEXT("5")) ,FString(TEXT("7")) };
	TArray<FString> M_ButtTab_Click_ = { FString(TEXT("2")),FString(TEXT("4")) ,FString(TEXT("6")) ,FString(TEXT("8")) };
private:
	//商品1（卡片）
	UPROPERTY()
		TArray<FItem_TicketPrice_Data> M_Cards;
	//商品2（卡片技能书）
	UPROPERTY()
		TArray<FItem_TicketPrice_Data> M_CardSkillbooks;
	//商品3（卡片转职）
	UPROPERTY()
		TArray<FItem_TicketPrice_Data> M_CardChangedJobs;
	//商品4
	UPROPERTY()
		TArray<FItem_TicketPrice_Data> M_All;
	//加载器
	UPROPERTY()
		UItemLoadManager* M_Ticket_UItemLoadManager = nullptr;
public:
	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_Init(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_Update(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
public:
	//剔除不显示的商品
	void TransformData(const TArray<FItem_TicketPrice_Data>& _Datas, TArray<FItem_TicketPrice_Data>& _OutResult);
private:
	//商品展示区
	UPROPERTY()
		UUniformGridPanel* M_ShopItemPanel = nullptr;
	//滑动区
	UPROPERTY()
		UScrollBox* M_UScrollBox = nullptr;
public:
	//货币数量文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CoinText;
public:
	bool Initialize() override;
	void UpdateCoin();
public:
	//卡片区域
	UUI_TicketShopGrid* CardsRange();
public:
	//初始化数据
	UFUNCTION(BlueprintCallable)
		void Init();
	//卡片展示区
	UFUNCTION(BlueprintCallable)
		void Cards();
	//卡片技能书展示区
	UFUNCTION(BlueprintCallable)
		void CardSkillbooks();
	//卡片转职展示区
	UFUNCTION(BlueprintCallable)
		void CardChangedJobs();
	//展示区->全部
	UFUNCTION(BlueprintCallable)
		void TicketShop_0_All();
};
