// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "Data/CardSkillBookStruct.h"
#include "GameSystem/PlayerStructManager.h"
#include "UI_PlayerBag.generated.h"

/**
 *
 */

class UUI_PlayerBagRemove;
class UUI_PlayerInformationShow;
class UCanvasPanel;
class UUniformGridPanel;
class UUI_CardSkillTable;
class UVerticalBox;
class UImage;
class UWidget;


UCLASS()
class FVM_API UUI_PlayerBag : public UWidgetBase
{
	GENERATED_BODY()
private:
	//加载结构(角色背包加载器)
	FTimerLoad<UUI_PlayerBag, FEquipmentBase*> TimeLoad_PlayerBags;
	//装备加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_Equipment_Manager = nullptr;
	//防御卡加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_Cards_Manager = nullptr;
	//材料加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_Materials_Manager = nullptr;
	//----------------------------------------------------------------------------------------------------
	//技能书加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_CardSkillBooks_Manager = nullptr;
	//技能书索引存储器
	UPROPERTY()
		TArray<FSkillBookData> M_FItemData_Show_CardSkillBooks;
	//----------------------------------------------------------------------------------------------------
	//当前选择的界面->索引值 ->装备界面 0  卡片界面1  材料界面2
	uint8 M_Panel_Index;
private:
	//道具删除界面
	UPROPERTY()
		TArray<UUI_PlayerBagRemove*> M_UUI_PlayerBagRemove_Panels;
private:
	//加载道具的格子
	UPROPERTY()
		TArray<UUniformGridPanel*> M_PlayerItemPanels;
	//背包选择界面
	UPROPERTY()
		UCanvasPanel* M_PlayerBagSelectPanel = nullptr;
	//背包道具内容
	UPROPERTY()
		UUniformGridPanel* M_PlayerBagContent = nullptr;
public:
	//选项卡tab默认图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> M_BagTableDefImg;
	//选项卡tab选择图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> M_BagTableSelectImg;
public:
	//背包名称
	FString M_BagVectorName;
	//背包号
	uint8 M_BagNumber;
public:
	//背包金币文字显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Bag_Coin0_Count_Text = FString(TEXT("0"));
	//背包礼券文字显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Bag_Coin1_Count_Text = FString(TEXT("0"));
	//背包点券文字显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Bag_Coin2_Count_Text = FString(TEXT("0"));
	//---------------------------------------
		//背包格子文字总容量显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagTotalCount_Text_1;
	//背包格子的文字——1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_1;
	//背包格子的文字——2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_2;
	//背包格子的文字——3
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_3;
	//背包格子的文字——4
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_4;

	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_1 = nullptr;
	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_2 = nullptr;
	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_3 = nullptr;
	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_4 = nullptr;

	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_1 = nullptr;
	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_2 = nullptr;
	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_3 = nullptr;
	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_4 = nullptr;

	//背包物品删除按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagItemUniformRemoveButton = nullptr;

	//------------------------------------------------------------------------------------------------------------------------
	//   界面打开
	//------------------------------------------------------------------------------------------------------------------------
private:
	//技能书分类
	UPROPERTY()
		ESkillBookTipCategory M_CardSkillBooksCategory = ESkillBookTipCategory::S_Normal;
	//按钮组
	UPROPERTY()
		TArray<UButton*> M_SkillBookButtons;
	//默认图片
	UPROPERTY()
		TArray<FString> M_SkillBookDefStyle;
	//点击图片
	UPROPERTY()
		TArray<FString> M_SkillBookClickStyle;
	//功能
	UFUNCTION(BlueprintCallable)
		void Open_SkillBooks_Normal();
	UFUNCTION()
		void Open_SkillBooks_Style0();
	UFUNCTION()
		void Open_SkillBooks_Style1();
	UFUNCTION()
		void Open_SkillBooks_Style2();
	//------------------------------------------------------------------------------------------------------------------------
	//   卡片技能书
	//------------------------------------------------------------------------------------------------------------------------
public:
	//技能书滑动容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UScrollBox* M_CardSkillBook_Scroll = nullptr;
	//技能书格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUniformGridPanel* M_CardSkillBook_Panel = nullptr;
public:
	//------------------------------------------------------------------------------------------------------------------------
	//   角色形象开始
	//------------------------------------------------------------------------------------------------------------------------	

	//-角色基础着装---------------------------------------------------------------------------------

	//角色创建点位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCanvasPanel* M_PlayerPoint = nullptr;

	//角色形象界面
	UPROPERTY()
		UUI_PlayerInformationShow* M_PlayerInformationShow = nullptr;
public:
	UFUNCTION()
		void RefreshPlayerSuit();
	//------------------------------------------------------------------------------------------------------------------------	
	//   角色形象结束
	//------------------------------------------------------------------------------------------------------------------------
public:
	virtual bool Initialize() override;

	//加载器初始化
	UFUNCTION(BlueprintCallable)
		void LoaderInit();

	//设置背包格子图片以及文字
	int32 SetBagGridStyle(int32 M_Count, const TArray<FPlayerBagGird>& _BagGrid);

	//清除
	void Clear();
public:
	//加载结束时做什么
	void LoadTimerEndToDo(UUI_PlayerBag* _Bag);

	//创建装备道具界面
	UFUNCTION()
		UWidget* WidgetCreate_Equipment(UItemDataTable* _Data, int32 _Index);
	//刷新装备道具界面
	UFUNCTION()
		void WidgetRefresh_Equipment(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建卡片道具界面
	UFUNCTION()
		UWidget* WidgetCreate_Cards(UItemDataTable* _Data, int32 _Index);
	//刷新卡片道具界面
	UFUNCTION()
		void WidgetRefresh_Cards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建材料道具界面
	UFUNCTION()
		UWidget* WidgetCreate_Materials(UItemDataTable* _Data, int32 _Index);
	//刷新材料道具界面
	UFUNCTION()
		void WidgetRefresh_Materials(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建防御卡技能书道具界面
	UFUNCTION()
		UWidget* WidgetCreate_CardSkillBooks(UItemDataTable* _Data, int32 _Index);
	//刷新防御卡技能书道具界面
	UFUNCTION()
		void WidgetRefresh_CardSkillBooks(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//加载角色装备的背包
	void LoadPlayerBags(UUI_PlayerBag* _Class, TArray<FEquipmentBase*>& _Items_, int32 _Index);
	//加载背包格子的数据
	TArray<FPlayerBagGird>& LoadBagGridData(uint8 _PanelNumber);
public:
	//购买格子
	void BagGridButton_BuyGrid(uint8 _Panel_Index, uint8 _Gird_Index, int32 _Price);
	//使用格子
	void UseCurrentGrid(uint8 _PanelIndex, uint8 _BagNumber, int32 _Price);
	//添加背包到格子
	void AddBagGrid(int32 _PanelNumber, int32 _BagNumber, TArray<FPlayerBagGird>& _Grid, FEquipmentBase* _Data);
public:
	UFUNCTION()
		void BagGridButton_1_Func();
	UFUNCTION()
		void BagGridButton_2_Func();
	UFUNCTION()
		void BagGridButton_3_Func();
	UFUNCTION()
		void BagGridButton_4_Func();
	UFUNCTION()
		void OpenRemovePanel();
	//关闭选择背包的界面
	UFUNCTION()
		void CloseSelectBagPanel();
public:
	//更新货币显示
	UFUNCTION(BlueprintCallable)
		void UpdateShowCoinText();
	//获取当前界面的索引
	UFUNCTION(BlueprintCallable)
		uint8 GetPanel_Index();
	//加载道具界面->道具
	UFUNCTION(BlueprintCallable)
		void LoadItemsData();
	//加载道具结束
	UFUNCTION(BlueprintCallable)
		void LoadItemsDataEnd(UUI_PlayerBag* _bag);
	//加载卡片数据->防御卡界面
	UFUNCTION(BlueprintCallable)
		void LoadCardData();
	//加载材料数据->材料界面 
	UFUNCTION(BlueprintCallable)
		void LoadMaterialData();
	//加载防御卡技能书选项卡(并且设置加载分类)
	UFUNCTION(BlueprintCallable)
		void LoadCardSkillBookTabs(ESkillBookTipCategory M_ESkillBookTipCategory);
	//加载防御卡技能书选项卡(使用默认的加载分类或者使用上一次的分类加载)
	UFUNCTION(BlueprintCallable)
		void LoadCardSkillBookTabs_DefCategory();
	//更新背包容量[返回总容量]
	UFUNCTION(BlueprintCallable)
		int32 UpdateBagCount(uint8 _UpdatePanel, int32 Count);
	//关闭界面
	UFUNCTION(BlueprintCallable)
		void CloseBagPanel();
public:
	//设置装备属性
	void SetEquipmentAttribute(class UUI_PlayerBagEquipmentGrid* _Grid, UItemDataTable* _Items_, int32 _Index);
	//设置卡片属性
	void SetCardAttribute(class UUI_PlayerBagCardGrid* _TempCardGrid, UItemDataTable* _Items_, int32 _Index);
	//设置材料属性
	void SetMaterialAttribute(class UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _Items_, int32 _Index);
};
