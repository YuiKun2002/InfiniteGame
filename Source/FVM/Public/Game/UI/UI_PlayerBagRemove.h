// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_PlayerBagRemove.generated.h"

class UUI_PlayerBag;
class UVerticalBox;
class UUniformGridPanel;
class UScrollBox;
class UButton;
class UEditableTextBox;
class UUI_BagRemove_Equipment;

UCLASS()
class FVM_API UUI_PlayerBagRemove : public UWidgetBase
{
	GENERATED_BODY()
private:
	//当前被标记的卡片集合
	UPROPERTY()
		TMap<int32, FString> M_CurrentSelect;
public:
	//添加当前选项
	UFUNCTION()
		void AddCurrentSelect(int32 _Key, const FString& _ItemName);
	//移出当前选项
	UFUNCTION()
		void RemoveCurrentSelect(int32 _Key);
	//查询（当前列表是否有被选择的道具）
	UFUNCTION()
		bool FindCurrentSelect(int32 _Key);
private:
	//搜索物品名称
	UPROPERTY()
		UEditableTextBox* M_InputItemNameBox = nullptr;
	//搜索按钮
	UPROPERTY()
		UButton* M_SearchButton = nullptr;
	//删除按钮
	UPROPERTY()
		UButton* M_RemoveButton = nullptr;
	//关闭按钮
	UPROPERTY()
		UButton* M_ClosePanelButton = nullptr;
private:
	//输出物品列表
	UPROPERTY()
		UUniformGridPanel* M_UUniformGridPanel = nullptr;
	UPROPERTY()
		UScrollBox* M_UScrollBox = nullptr;
	//是否使用搜索模式
	UPROPERTY()
		bool M_bUseSerach = false;
	//搜索结果的索引
	UPROPERTY()
		TArray<FItemData_Index> M_FUI_PlayerBagRemove_Item_Index;
private:
	//加载器
	UPROPERTY()
		UItemLoadManager* M_UItemLoadManager_Equipment = nullptr;
	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_InitEquipment(UItemDataTable* _Data, int32 L_Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_UpdateEquipment(UItemDataTable* _Data, int32 L_Index, UWidget* _UWidget);

	//加载器
	UPROPERTY()
		UItemLoadManager* M_UItemLoadManager_Cards = nullptr;
	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_InitCards(UItemDataTable* _Data, int32 L_Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_UpdateCards(UItemDataTable* _Data, int32 L_Index, UWidget* _UWidget);
public:
	//初始化加载器
	UFUNCTION()
		void InitLoader(uint8 _PanelNumber);
public:
	//界面序号
	uint8 M_PanelNumber = 0u;
	//是否标记
	bool M_bSelect_Tag = false;
	//装备重叠

	//背包UI
	UUI_PlayerBag* M_UIBag = nullptr;
public:
	virtual bool Initialize() override;
	//搜索背包数据
	void SearchBag(uint8 _PanelNumber);
	//获取搜索按钮
	UButton* GetSearchButton();
	//销毁卡片
	UFUNCTION()
		void Remove();
public:
	//搜索卡片数据
	UFUNCTION()
		void SearchBag_Card();

	//搜索装备数据
	UFUNCTION()
		void SearchBag_Equipment();

	//搜索材料数据
	UFUNCTION()
		void SearchBag_Materials();

	//关闭当前界面
	UFUNCTION()
		void CloseCurrentPanel();
};
