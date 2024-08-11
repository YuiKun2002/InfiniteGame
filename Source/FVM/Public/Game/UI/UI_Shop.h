// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "Data/ShopItemPriceStruct.h"
#include "Game/UI/UI_ShopGrid.h"
#include "UI_Shop.generated.h"


class UButton;
class UCanvasPanel;
class UUniformGridPanel;
class UUI_PlayerInformationShow;
class UScrollBox;
class UUI_ShopGrid;


//商城选项卡
UENUM(BlueprintType)
enum class EShopTab : uint8 {
	ESnapUp UMETA(DisplayName = "SnapUp"),
	ESelling UMETA(DisplayName = "Selling"),
	EDiscount UMETA(DisplayName = "Discount"),
	EWeapon UMETA(DisplayName = "Weapon"),
	ECards UMETA(DisplayName = "Cards"),
	EItem UMETA(DisplayName = "Item"),
	EMakeup UMETA(DisplayName = "Makeup"),
	EFashion UMETA(DisplayName = "Fashion"),
};


UCLASS()
class FVM_API UUI_Shop : public UWidgetBase
{
	GENERATED_BODY()
private:
	//加载器(开始界面)
	UPROPERTY()
	UItemLoadManager* M_UItemLoadManager_0 = nullptr;
	UPROPERTY()
	UItemLoadManager* M_UItemLoadManager_1 = nullptr;
	//其他道具-加载器
	UPROPERTY()
	UItemLoadManager* M_UItemLoadManager_2 = nullptr;
private:
	//-------------------------------------服装区【角色形象界面】----------------------------------------
	UPROPERTY()//角色界面容器
		UCanvasPanel* M_PlayerSuit_BoxPanel = nullptr;
	UPROPERTY()//角色形象界面
		UUI_PlayerInformationShow* M_UUI_PlayerInformationShow = nullptr;
	//-------------------------------------服装区【角色形象界面】----------------------------------------
public:
	//创建材界面
	UFUNCTION()
	UWidget* WidgetCreate_Init_0(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
	void WidgetRefresh_Update_0(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建材界面
	UFUNCTION()
	UWidget* WidgetCreate_Init_1(UItemDataTable* _Data, int32 _Index);

	//初始化商品数据
	void InitShopGridData(UItemDataTable* _Data, UUI_ShopGrid* _UWidget);
public:
	//数据表数据(抢购左-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_Left;
	//数据表数据(抢购右-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_Right;
	//数据表数据(热卖-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_SellingRange;
	//数据表数据(折扣-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_DiscountRange;
	//数据表数据(武器-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_WeaponRange;
	//数据表数据(防御卡-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_Cards;
	//数据表数据(道具-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_ItemRange;
	//数据表数据(妆容-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_MakeupRange;
	//数据表数据(时装-区域)
	UPROPERTY()
	TArray<FItem_Price_Data> M_ShopItemDatas_FashionRange;
private:
	//商城选项卡
	UPROPERTY()
	TArray<UButton*> M_ShopTabs;
	//商城选项卡默认路径
	UPROPERTY()
	TArray<FString> M_ShopTabsDef;
private:
	//抢购区1
	UPROPERTY()
	UUniformGridPanel* M_SnapUp_0 = nullptr;
	//抢购区2
	UPROPERTY()
	UUniformGridPanel* M_SnapUp_1 = nullptr;
	//抢购区1(滑动界面)
	UPROPERTY()
	UScrollBox* M_SnapUp_ScrollBox_0 = nullptr;
	//抢购区2(滑动界面)
	UPROPERTY()
	UScrollBox* M_SnapUp_ScrollBox_1 = nullptr;

	//其他区域-格子
	UPROPERTY()
	UUniformGridPanel* M_ContentGrid_2 = nullptr;
	//其他区域(滑动界面)
	UPROPERTY()
	UScrollBox* M_Content_ScrollBox_2 = nullptr;
private:
	//其他购买区域（集合）
	UPROPERTY()
	TArray<UUniformGridPanel*> M_Item_Contents;
private:
	int32 M_ItemMoney = 0;
	uint8 M_ItemMoneyType = 0;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_Coin_Text = TEXT("金币:[0] 礼券[0] 点券[0] 威望[0]");
public:
	//初始化
	virtual bool Initialize() override;
	//更新金币资源
	void UpdateCoinText();
	//剔除不显示的商品
	void TransformData(const TArray<FItem_Price_Data>& _Datas, TArray<FItem_Price_Data>& _OutResult);
public:
	//数据表初始化
	UFUNCTION(BlueprintCallable)
	void InitDataTable();
public:
	//界面切换
	UFUNCTION(BlueprintCallable)
	void SelectShopTab(EShopTab Tab, const FString& URL, const FString& Json);
public:
	//加载抢购区域的物品（左边）
	UFUNCTION(BlueprintCallable)
	void LoadSnapUpLeft();

	//加载抢购区域的物品（右边）
	UFUNCTION(BlueprintCallable)
	void LoadSnapUpRight();

	//加载热卖区域的物品
	UFUNCTION(BlueprintCallable)
	void LoadSellingItemRange();

	//加载折扣区域的物品
	UFUNCTION(BlueprintCallable)
	void LoadDiscountItemRange();

	//加载武器区域的物品
	UFUNCTION(BlueprintCallable)
	void LoadWeaponRange();

	//加载防御卡区域
	UFUNCTION(BlueprintCallable)
	void LoadCardRange();

	//加载道具区域的物品
	UFUNCTION(BlueprintCallable)
	void LoadItemRange();

	//加载道具区域的物品
	UFUNCTION(BlueprintCallable)
	void LoadMakeupRange();

	//加载时装区域的物品
	UFUNCTION(BlueprintCallable)
	void LoadFashionRange();
public:
	//加载角色服装显示界面
	UFUNCTION(BlueprintCallable)
	void ShowPlayerSuitPanel(bool _bHidden);
public:
	//加载数据表
	UFUNCTION()
	void LoadList();

	//关闭界面
	UFUNCTION(BlueprintCallable)
	void ClosePanel();
};