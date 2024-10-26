// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "UI_GamePrepare.generated.h"

/**
 *   游戏准备界面
 */

class UUniformGridPanel;
class UUI_PlayerBagCardGrid;
class UHorizontalBox;
class UVerticalBox;
class UUI_TableTextBlock;
class UUI_PlayerShow;
class UImage;

UCLASS()
class FVM_API UUI_GamePrepare : public UWidgetBase
{
	GENERATED_BODY()
private:
	//时间分段加载结构
	FTimeClip M_FTimeClip;
	FTimeClip M_FTimeClip_Mouse;
private:
	UPROPERTY()
	TArray<FString> M_MouseNameList;
	UPROPERTY()
	int32 M_MouseNameListSize = 0;
private:
	//卡片列表
	UPROPERTY()
	UUniformGridPanel* M_CardGrid = nullptr;
	//选择的卡片列表
	UPROPERTY()
	UUniformGridPanel* M_SelectCardList = nullptr;
	//背包格子
	UPROPERTY()
	TArray<UUI_PlayerBagCardGrid*> M_BagGrid;
	//角色形象界面
	UPROPERTY()
	UUI_PlayerShow* M_UUI_PlayerShow = nullptr;
private:
	//选择卡片的数量->Max 18
	UPROPERTY()
	int32 M_SelectCardNum = 0;
public:
	//卡片
	TArray<FItemCard>  _CardItems_;
	//卡片栏的背包格子
	int32 M_CardBagCount = 0;
public:
	//卡片数据
	UPROPERTY()
	TArray<FItemCard> M_CardDatas_Copy;
	//当前存在列表的卡片
	UPROPERTY()
	TMap<int32, FString> CardNames;
	UPROPERTY()
	TArray<int32> CardID;
	UPROPERTY()
	TArray<FString> CardNames_Array;

public:
	//玩家名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_PlayerName;
	//地图名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MapName;
	//主要的地图名称(大地图名称)->美味一区 火山一区 遗迹一区 天空一区 海洋一区 奇境一区
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MainMapName;
	//地图中老鼠的进攻回合总计
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MapRound;
	//显示地图是白天还是夜晚
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MapDayState;
	//老鼠的名称列表
	UPROPERTY()
	UVerticalBox* M_MouseList = nullptr;
	//地图头像
	UPROPERTY()
	UImage* M_MapHead = nullptr;
	//等级限制头像
	UPROPERTY()
	UImage* M_CardGradeMaxHead = nullptr;
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;
	//选择卡片
	UFUNCTION()
	void SelectCard(const FItemCard& _CardData);
	//设置指定的卡片禁用
	UFUNCTION()
	void SetCardEnable(const TArray<int32>& _Names, bool _bEnable);
public:
	//取消卡片选择数量减1
	UFUNCTION(BlueprintCallable)
	void CancelCardNum();
	//获取选卡的数量
	UFUNCTION(BlueprintCallable)
	int32 GetSelectCardNums();
	//从角色背包中加载拥有的所有卡片
	UFUNCTION(BlueprintCallable)
	void LoadCardList();
	//加载老鼠列表
	UFUNCTION(BlueprintCallable)
	void LoadMouseList();
	//初始化界面数据
	UFUNCTION(BlueprintCallable)
	void InitPanelData();
	//关闭当前界面
	UFUNCTION(BlueprintCallable)
	void CloseCurrentPanel();
public:
	//卡片界面加载器(共用于 卡片强化，1卡片转职，2金卡进化等等)
	UPROPERTY()
	UItemLoadManager* ItemLoadManager_Card = nullptr;
	//卡片（强化，转职，等）统一共用滑动界面
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox_Card;
	//卡片（强化，转职，等）统一共用格子界面
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* Uniform_Card;
	//创建材界面
	UFUNCTION()
	class UWidget* WidgetCreate_InitCards(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
	void WidgetRefresh_UpdateCards(UItemDataTable* _Data, int32 _Index, class UWidget* _UWidget);
	//设置卡片数据
	UFUNCTION()
	void SetCardData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _Data, int32 _Index);
	//加载卡片
	void LoadCards();
};
