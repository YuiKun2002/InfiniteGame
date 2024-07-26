// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/TreviFountainStruct.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_ExchangeAndSplit.generated.h"

/**
 * 券兑换->兑换与分解
 */


class UTextBlock;
class UImage;
class UButton;
class UUniformGridPanel;
class UScrollBox;
class UUI_TicketShopGrid;
class UItemDataTable;
class UItemLoadManager;



/*
使用流程：
设置券名称 SetTicketName -> 角色道具库是道具名称【生肖兑换券】
设置兑换的奖励——>许愿池->原生券 SetTicket
设置分解关键词->材料->卡片 SetKeywords
显示全部（收尾） ShowAll
*/

UCLASS()
class FVM_API UUI_ExchangeAndSplit : public UWidgetBase
{
	GENERATED_BODY()
public:
	//设置券数据转换通过许愿池奖励
	UFUNCTION()
		void SetTicketsByTreviFountain(const TArray<FTreviFountainItemStruct>& _Input, TArray<FItemTicketPrice>& _Out);
	//设置货币转换许愿池奖励转换券物品
	UFUNCTION()
		void SetTicketsTransformByTreviFountain(const TArray<FTreviFountainItemStruct>& _Input);
	//设置券数据
	UFUNCTION(BlueprintCallable)
		void SetTicketsNative(const FSoftObjectPath& _DataTabel, TArray<FItemTicketPrice>& _Out);
public:
	//设置兑换券的名称
	UFUNCTION(BlueprintCallable)
		void SetTicketName(const FString& _TicketName, const FString& _TicketTexturePath);
	//设置关键词(材料)
	UFUNCTION(BlueprintCallable)
		void SetKeywordsByMaterials(const TArray<FString>& _Keys, const TArray<FString>& _KeyIgnore);
	//设置关键词（卡片）
	UFUNCTION(BlueprintCallable)
		void SetKeywordsByCards(const TArray<FString>& _Keys, const TArray<FString>& _KeyIgnore);
public:
	//通过数据表设置卡片数据
	UFUNCTION(BlueprintCallable, Category = "NativeTicket")
		void SetCardsTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel);
	//通过数据表设置技能书数据
	UFUNCTION(BlueprintCallable, Category = "NativeTicket")
		void SetSkillBooksTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel);
	//通过数据表设置转职材料数据
	UFUNCTION(BlueprintCallable, Category = "NativeTicket")
		void SetChangeJobMaterialsTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel);
	//通过数据表设置其他材料数据
	UFUNCTION(BlueprintCallable, Category = "NativeTicket")
		void SetOtherMaterialsTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel);
public:
	//通过许愿池奖励设置券兑换
	UFUNCTION(BlueprintCallable, Category = "TreviFountainTicket")
		void SetTicketByTreviFountain(const TSoftObjectPtr<UDataTable>& _DataTabel);
public:
	//通过名称获得当前选择物品的价格
	const int32 GetCurrentSplitItemCountByName(const FString& _ItemName) const;

public:
	//设置分解按钮的开启或者关闭
	UFUNCTION()
		void SetSplitEnable(bool _State);
	//设置当前选择的分解物品对象
	UFUNCTION(BlueprintCallable, Category = "SplitItem")
		void SetCurrentSlipItem(const EItemType& _CurrentSelectType, const int32& _Index, UItemDataTable* const _SelectData, UButton* _CurrentSelectButt);
	//设置分解道具数量加成率
	UFUNCTION(BlueprintCallable, Category = "SplitRandom")
		void SetSplitItemAddedRate(float _Value);
	//获取分解道具的数量加成率
	UFUNCTION(BlueprintCallable, Category = "SplitRandom")
		float GetSplitItemAddedRate();
public:
	//获取当前选择的按钮
	UFUNCTION()
		UButton* GetCurrentSelectButt();
public:
	//更新列表
	UFUNCTION()
		void UpdateListByExchange();
	//更新货币资源显示
	UFUNCTION(BlueprintCallable)
		void UpdateResourceText();
public:
	//支付成功
	UFUNCTION()
		void PayFinish();
	//券支付成功自动调用
	UFUNCTION()
		void TicketPayFinishOver();
public:
	//显示全部
	UFUNCTION(BlueprintCallable, Category = "ShowItems")
		void ShowAll();
	//显示防御卡
	UFUNCTION(BlueprintCallable, Category = "ShowItems")
		void ShowCard();
	//显示技能书
	UFUNCTION(BlueprintCallable, Category = "ShowItems")
		void ShowSkillBooks();
	//显示转职材料
	UFUNCTION(BlueprintCallable, Category = "ShowItems")
		void ShowChangeJobMaterials();
public:
	//初始化
	bool Initialize() override;
	//当读取了角色存档时
	UFUNCTION(BlueprintImplementableEvent)
		void OnInitPlayerData(FPlayerExchangeAndSplit Data);
	//读取存档存储信息
	UFUNCTION(BlueprintCallable)
		void InitPlayerData(FString _Title = TEXT("法老宝藏"));
	//存储存档信息
	UFUNCTION(BlueprintCallable)
		void SaveCurInfor(FPlayerExchangeAndSplit Data);

	//初始化材料的数据
	UFUNCTION(BlueprintCallable)
		void InitPlayerMaterialData(bool _IsCard);
	//初始化角色的材料列表
	UFUNCTION(BlueprintCallable)
		void InitPlayerMaterialsList();
	//初始化卡片列表
	UFUNCTION(BlueprintCallable)
		void InitPlayerCardsList();

	//分解当前选择的道具
	UFUNCTION(BlueprintCallable)
		void SplitCurSelectItem();
public:
	//游戏初始化
	UFUNCTION(BlueprintCallable)
		void GameInit();
	//关闭
	UFUNCTION(BlueprintCallable)
		void GameClose();
public:
	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_ExchangeInit(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_ExchangeUpdate(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_SplitMaterialInit(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_SplitMaterialUpdate(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置材料属性
	UFUNCTION()
		void SetSplitMaterials(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_CardInit(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_CardUpdate(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置卡片属性
	UFUNCTION()
		void SetSplitCards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
private:
	//界面标题
	UPROPERTY()
		FString M_Title = TEXT("法老宝藏");
	//当前选择的分解道具类型
	UPROPERTY()
		EItemType M_CurrentSelectType = EItemType::E_Equipment;
	//当前选择的索引
	UPROPERTY()
		int32 M_CurrentSelectIndex = -1;
	//当前选择的数据对象
	UPROPERTY()
		UItemDataTable* M_CurrentSelect = nullptr;
	//当前选择的按钮
	UPROPERTY()
		UButton* M_CurrentSelectButt = nullptr;

	//当前分解的数量
	UPROPERTY()
		int32 M_CurrentSplitCount = 0;
	//当前额外分解的数量
	UPROPERTY()
		int32 M_CurrentOutSplitCount = 0;
	//最终的值
	UPROPERTY()
		float M_RandomAddedRate = 0.f;
private:
	//兑换券的图片组件
	UPROPERTY()
		UImage* M_ResourceHead = nullptr;
	//分解券的图片
	UPROPERTY()
		UImage* M_Split_Ticket_LeftImg = nullptr;
	UPROPERTY()
		UImage* M_Split_Ticket_RightImg = nullptr;
	//点券文字组件
	UPROPERTY()
		UTextBlock* M_ResourceD = nullptr;
	//券资源
	UPROPERTY()
		UTextBlock* M_ResourceTicket = nullptr;
	//分解券产出资源的数量的文本 例如: x15
	UPROPERTY()
		UTextBlock* M_Split_Generate_ResourceTicketCount_Left_Text = nullptr;
	//分解券额外产出资源的数量的文本 例如: x15
	UPROPERTY()
		UTextBlock* M_Split_Generate_ResourceTicketCount_Right_Text = nullptr;
	//分解券额外产出资源的文本 例如: 30%
	UPROPERTY()
		UTextBlock* M_Split_Generate_ResourceTicket_Text = nullptr;
	//需要被分解的卡片
	UPROPERTY()
		UButton* M_NeedSplitCard_Butt = nullptr;
	//分解卡片
	UPROPERTY()
		UButton* M_SplitCard_Butt = nullptr;
private:
	//技能书/转职材料/关键词（用于检索材料）
	UPROPERTY()
		TArray<FString> M_MaterialsKeywords;
	//技能书/转职材料/忽略关键词（用于检索材料）
	UPROPERTY()
		TArray<FString> M_MaterialsKeywords_Ignore;
	//卡片/关键词（用于检索卡片）
	UPROPERTY()
		TArray<FString> M_CardsKeywords;
	//卡片/忽略关键词（用于检索卡片）
	UPROPERTY()
		TArray<FString> M_CardsKeywords_Ignore;
private:
	//卡片
	UPROPERTY()
		TArray<FItemTicketPrice> M_Cards;
	//技能书
	UPROPERTY()
		TArray<FItemTicketPrice> M_CardBooks;
	//转职材料
	UPROPERTY()
		TArray<FItemTicketPrice> M_CardChangePosition;
	//其他材料
	UPROPERTY()
		TArray<FItemTicketPrice> M_OtherItems;
	//所有材料集合
	UPROPERTY()
		TArray<FItemTicketPrice> M_AllItems;
private:
	//需要的材料名称
	UPROPERTY()
		FString NeedResourceName = TEXT("生肖兑换券");
	//兑换券的图片
	UPROPERTY()
		FString NeedResourceTexturePath = TEXT("");
private:
	//滚动框（兑换区）
	UPROPERTY()
		UScrollBox* M_Exchange_Scroll = nullptr;
	//统一界面（兑换区）
	UPROPERTY()
		UUniformGridPanel* M_Exchange_Uniform = nullptr;
	//（兑换）加载区
	UPROPERTY()
		UItemLoadManager* M_Exchange_Loadding = nullptr;

	//滚动框（分解材料区）
	UPROPERTY()
		UScrollBox* M_SplitMaterial_Scroll = nullptr;
	//统一界面（分解材料区）
	UPROPERTY()
		UUniformGridPanel* M_SplitMaterial_Uniform = nullptr;
	//（材料）加载区
	UPROPERTY()
		UItemLoadManager* M_SplitMaterial_Loadding = nullptr;

	//滚动框（分解卡片区）
	UPROPERTY()
		UScrollBox* M_Card_Scroll = nullptr;
	//统一界面（分解卡片区）
	UPROPERTY()
		UUniformGridPanel* M_Card_Uniform = nullptr;
	//（卡片）加载区
	UPROPERTY()
		UItemLoadManager* M_Card_Loadding = nullptr;
};
