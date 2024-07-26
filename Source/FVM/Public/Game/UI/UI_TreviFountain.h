// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Data/TreviFountainStruct.h"
#include "UI_TreviFountain.generated.h"

class UButton;
class UImage;
class UVerticalBox;
class UHorizontalBox;
class UUniformGridPanel;
class URichTextBlock;
class UCanvasPanel;
class UUI_TreviFountianItemBox;

UCLASS()
class FVM_API UUI_TreviFountain : public UWidgetBase
{
	GENERATED_BODY()
private:
	//奖池奖励加载管理器
	FTimerLoad<UUI_TreviFountain, FTreviFountainItemStruct> ItemsLoadManager;
	//显示奖励的加载管理器
	FTimerLoad<UUI_TreviFountain, FTreviFountainItemStruct> ShowItemsLoadManager;
	//奖池结构
	FTreviFountainRandomCategoyStruct M_TreviFountainRandomCategoyStruct;

	//当前抽奖的形式(0单抽 1五连  2十连)
	UPROPERTY()
		int32 M_GetType = 0;
	//当前抽奖需要的不同金额(第一个界面的（0，1，2），第二个界面的（0，1，2），第三个界面的（0，1，2）)
	UPROPERTY()
		int32 M_GetCoinType = 0;
public:
	//金币
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerCoinType_0 = FString();
	//点券
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerCoinType_2 = FString();
private:
	//许愿池-奖池选项卡
	UPROPERTY()
		TArray<UButton*> M_TF_TabButton;
	//选项卡-默认样式
	UPROPERTY()
		TArray<FString> M_TF_TabDef;
	//选项卡-点击样式
	UPROPERTY()
		TArray<FString> M_TF_TabClick;
	//奖池界面切换(奖池的背景框(3种))
	UPROPERTY()
		TArray<UWidget*> M_TF_PoolBgPanel;
	//奖池抽取按钮切换(三种许愿池按钮(3种))
	UPROPERTY()
		TArray<UWidget*> M_TF_PoolGetButtonsPanel;
	//奖池图片按钮->所有的奖励都显示在按钮上
	UPROPERTY()
		TArray<UCanvasPanel*> M_ItemsPool;
	//奖池的所有奖励
	UPROPERTY()
		TArray<FTreviFountainItemStruct> M_ItemsPoolData;
	//当前获得的所有奖励
	UPROPERTY()
		TArray<FTreviFountainItemStruct> M_CurrentGetItems;
	//获取奖励显示的文字块
	UPROPERTY()
		URichTextBlock* M_GetContentText = nullptr;
	//显示奖励的展示框(单抽)
	UPROPERTY()
		UVerticalBox* M_GetOnceBox = nullptr;
	//显示奖励的展示框(五连抽)
	UPROPERTY()
		UHorizontalBox* M_GetFifthBox = nullptr;
	//显示奖励的展示框(十连抽)
	UPROPERTY()
		UUniformGridPanel* M_GetTenthBox = nullptr;
public:
	//初始化
	bool Initialize() override;
	//更新货币数量
	void UpdateCoin();
	//加载奖励并且显示(数据表的名称)
	void LoadItemsToPanel(const FString& _DataTableName);
	//加载物品
	void LoadItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index);

public:
	//--------------------------------------------------------------------------------------------
	//奖池选项卡功能按钮
	UFUNCTION(BlueprintCallable)
		void TF_Tab_0();
	UFUNCTION()
		void TF_Tab_1();
	UFUNCTION()
		void TF_Tab_2();
	//--------------------------------------------------------------------------------------------
	//抽奖  -> Count是表示抽n次
	UFUNCTION(BlueprintCallable)
		void BGetRandomItems(int32 _Count);
	//--------------------------------------------------------------------------------------------
	// 抽奖奖励显示
	UFUNCTION(BlueprintCallable)
		void ShowGet();

	//根据界面索引支付金额
	UFUNCTION(BlueprintCallable)
		bool SwitchPay(int32 _Index, int32 _FirstCoin, int32 _SecondCoin, int32 _ThirdCoin);

	//显示单抽奖励
	UFUNCTION(BlueprintCallable)
		void ShowGetFirstItem();
	//显示五连抽奖励
	UFUNCTION(BlueprintCallable)
		void ShowGetFifthItem();
	//显示十连抽奖励
	UFUNCTION(BlueprintCallable)
		void ShowGetTenthItem();

	//加载单抽奖励
	void LoadGetFirstItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index);
	//加载五连抽奖励
	void LoadGetFifthItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index);
	//加载十连抽奖励
	void LoadGetTenthItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index);

	//显示获奖文字
	void ShowItemText(UUI_TreviFountain* _Class);
	//--------------------------------------------------------------------------------------------
public:
	//关闭界面
	UFUNCTION(BlueprintCallable)
		void ClosePanel();
};
