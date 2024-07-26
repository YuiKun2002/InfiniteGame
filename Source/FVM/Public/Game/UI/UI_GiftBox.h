// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_GiftBox.generated.h"

/**
 *
 */

class UImage;
class UButton;
class UScrollBox;
class UVerticalBox;
class UUniformGridPanel;
class UUI_PlayerBagEquipmentGrid;

UCLASS()
class FVM_API UUI_GiftBox : public UWidgetBase
{
	GENERATED_BODY()
private:
	float _Current_Offset = 0.f;
private:
	UPROPERTY()
		UScrollBox* M_ScrollBox = nullptr;
	UPROPERTY()
		UUniformGridPanel* M_UUniformGridPanel = nullptr;
	UPROPERTY()
		UItemLoadManager* M_ListLoaderManager = nullptr;
private:
	UPROPERTY()
		UButton* M_OpenOnce = nullptr;
	UPROPERTY()
		UButton* M_OpenAll = nullptr;
private:
	//背包格子装备格子
	UUI_PlayerBagEquipmentGrid* M_UI_PlayerBagEquipmentGrid = nullptr;
	//礼盒数据
	FGiftBox M_GiftBoxData;
	//时间分段加载结构
	FTimeClip M_FTimeClip_0;
	//文字显示内容
	TArray<FString> M_TipText;
	//文字显示颜色
	TArray<FVector> M_TipTextColor;
private:
	//内容
	UPROPERTY()
		TArray<FItemBase> M_ItemBase;
public:
	//用于加载
	static FTimeClip M_FTimeClip_1;
	//用于界面的显示
	static UWidgetBase* M_ShowTextTipsPanel;
public:
	//标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Title;
	//内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Content;
public:
	virtual bool Initialize() override;
public:
	//加载器初始化
	UFUNCTION(BlueprintCallable)
		void InitLoader();
	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_Init(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_Update(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
public:
	//设置背包装备格子UI
	void SetPlayerBagEquipmentGrid(UUI_PlayerBagEquipmentGrid* _GridUI);
	//获取背包装备格子UI
	UUI_PlayerBagEquipmentGrid* GetPlayerBagEquipmentGrid();
	//设置礼盒名称
	void SetGiftBoxData(const FString& _Name);
	//开启礼盒一次
	UFUNCTION()
		void OpenGiftOnce();
	//全部开启礼盒
	UFUNCTION()
		void OpenGiftAll();
	//解析数据
	UFUNCTION()
		void AnalysisGiftBoxData();
	//创建列表->显示礼盒中所有的奖励
	UFUNCTION(BlueprintCallable)
		void ShowGiftList();
	//显示提升文字
	UFUNCTION()
		void ShowTipText();
public:
	//关闭信息提示
	UFUNCTION(BlueprintCallable)
		static void CloseTextTips();
public:
	//关闭界面
	UFUNCTION(BlueprintCallable)
		void TClosePanel();
public:
	//开箱
	void AnalysisBegin(int32 _OpenCount);
	//将礼盒中描述的物品解析成一个单独盒子对象存储在数组中(等待抽奖或者领取)
	void AnalysisGift(TArray<FGiftBox_Item>& Items);
	//根据模式提取对应礼盒中的道具(抽奖次数(开启了多少个礼盒),需要抽奖的奖品列表，最终得到的道具列表)
	void AnalysisGiftBoxMode(int32 _ItemCount, TArray<FGiftBox_Item>& Get_Items, TArray<FGiftBox_Item>& Items);
	//将道道具发送到背包
	void AnalysisItem(TArray<FGiftBox_Item>& Items);
	void CardToBag(FGiftBox_Item& Item);
	void EquipmentToBag(FGiftBox_Item& Item);
	void MaterialToBag(FGiftBox_Item& Item);
	void CoinToBag(FGiftBox_Item& Item);
};
