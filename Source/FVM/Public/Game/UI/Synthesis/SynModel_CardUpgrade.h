// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "SynModel_CardUpgrade.generated.h"



class UImage;
class UWidget;
class UButton;
class UScrollBox;
class UTextBlock;
class UItemDataTable;
class UItemLoadManager;
class UUniformGridPanel;
class UUI_PlayerSynthesis;


//卡片数据
USTRUCT()
struct FSynModelCardUpgradeData {
	GENERATED_USTRUCT_BODY()
public:
	//卡片格子
	UPROPERTY()
	class UUI_PlayerBagCardGrid* CardGrid = nullptr;
	//对应背包中的索引
	UPROPERTY()
	int32 PlayerBagIndex = -1;
	//第二ID
	UPROPERTY()
	int32 PlayerUIID = -1;
	//当前卡片的数据
	UPROPERTY()
	FItemCard CardData;
};

DECLARE_DELEGATE_ThreeParams(FOnCardLoad, UUI_PlayerBagCardGrid* Grid, UItemDataTable* Data, int32 Index);

/**
 * 卡片强化
 */
UCLASS(Blueprintable)
class FVM_API USynModel_CardUpgrade : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitializeBySynthesis(UUI_PlayerSynthesis* Class);
	UFUNCTION()
	void WidgetReset();
	UFUNCTION()
	void WidgetResetLoadData();

	//选择四叶草
	UFUNCTION()
	void SetSelectClover(FMaterialBase CopyData, float CloverRate);
	//选择四叶草的UI
	UFUNCTION()
	void SetSelectCloverUI(class UUI_PlayerBagMaterialGrid* UIClover);
	//取消对四叶草的选择
	UFUNCTION()
	void CancelSelectClover();

	//获取选择的四叶草
	UFUNCTION()
	int32 GetSelectClover(FString& OutCloverName);
	//获取四叶草按钮
	UFUNCTION()
	UButton* GetCloverButton();
	//获取选择的四叶草UI
	UFUNCTION()
	class UUI_PlayerBagMaterialGrid* GetSelectCloverUI();

	UFUNCTION()
	FString GetCurrentCardName();

	//获取概率最大数
	UFUNCTION()
	float GetUpGradeRate();
	//通过卡片价格和等级返回概率（价格-等级-等级差-衰减比例）
	UFUNCTION()
	float GetCardPriceGradeRate(float _Price, int32 _CardGrade, int32 MainCard_CurrentCard_Grade);

	//添加主卡
	UFUNCTION()
	void AddCardDataMain(FSynModelCardUpgradeData CopyData);
protected:
	//刷新卡片加载
	UPROPERTY()
	bool bRefreshCards = false;
	//当加载卡片时
	UFUNCTION()
	void OnLoadCardFunc(class UUI_PlayerBagCardGrid* Grid, class UItemDataTable* Data, int32 Index);
	//取消主卡
	UFUNCTION()
	void CancelSelectCardMain();
	//取消副卡1
	UFUNCTION()
	void CancelSelectCardSecond1();
	//取消副卡2
	UFUNCTION()
	void CancelSelectCardSecond2();
	//取消副卡3
	UFUNCTION()
	void CancelSelectCardSecond3();
	//取消卡片选择
	UFUNCTION()
	void CancelSelectCard(
		const int32 Index,
		UButton* LMainCard_But,
		UImage* LMainCard_GradeImg,
		UTextBlock* LMainCard_GradeText
	);
	//更新卡片列表外观
	UFUNCTION()
	void UpdateCardListStyle();
	//检查能否强化
	UFUNCTION()
	void CheckCardUpgrade();
	//卡片强化
	UFUNCTION()
	void CardUpgrade();
	//重新加载
	UFUNCTION(BlueprintCallable)
	void ReLoadCardUpgrade(const FString& MainCardID, bool Result);
private:
	UPROPERTY()
	UUI_PlayerSynthesis* PlayerSynthesis = nullptr;
private:
	//主卡
	UPROPERTY()
	UButton* MainCard_But = nullptr;
	//主卡等级
	UPROPERTY()
	UImage* MainCard_GradeImg = nullptr;
	//主卡价格
	UPROPERTY()
	UTextBlock* MainCard_GradeText = nullptr;

	//副卡1
	UPROPERTY()
	UButton* SecondCard1_But = nullptr;
	//副卡1等级
	UPROPERTY()
	UImage* SecondCard1_GradeImg = nullptr;
	//副卡1价格
	UPROPERTY()
	UTextBlock* SecondCard1_GradeText = nullptr;

	//副卡2
	UPROPERTY()
	UButton* SecondCard2_But = nullptr;
	//副卡2等级
	UPROPERTY()
	UImage* SecondCard2_GradeImg = nullptr;
	//副卡2价格
	UPROPERTY()
	UTextBlock* SecondCard2_GradeText = nullptr;

	//副卡3
	UPROPERTY()
	UButton* SecondCard3_But = nullptr;
	//副卡3等级
	UPROPERTY()
	UImage* SecondCard3_GradeImg = nullptr;
	//副卡3价格
	UPROPERTY()
	UTextBlock* SecondCard3_GradeText = nullptr;


	//强化价格
	UPROPERTY()
	UTextBlock* UpgradePriceText = nullptr;
	//强化成功率
	UPROPERTY()
	UTextBlock* UpgradeRateText = nullptr;

	//强化按钮
	UPROPERTY()
	UButton* M_UpGradeCard_Butt = nullptr;
	//四叶草按钮
	UPROPERTY()
	UButton* M_Clover_Butt = nullptr;

	//强化所用概率
	UPROPERTY()
	TArray<float> M_UpGradeRate_0_Arrays_2_Hei = { 0.f,0.f, 60.0, 42.90f, 24.20f,  20.10f,  13.20f,  10.6f,  6.0f, 2.20f,  1.80f, 1.70f, 1.60f , 1.40f,1.3f, 1.0f };
	UPROPERTY()
	TArray<float> M_UpGradeRate_0_Arrays_1_Hei = { 0.f,88.0f, 79.2f, 55.0f,  40.3f,  33.0f, 26.40f, 21.2f, 13.2f,   4.50f, 4.60f,  4.3f,   4.0f ,  4.0f,   3.40f, 3.0f , 0.0f ,0.0f };
	UPROPERTY()
	TArray<float> M_UpGradeRate_0_Arrays_0_Hei = { 100.0f,   100.0f, 96.80f,  68.60f,   49.50f,   39.60f,  31.90f,   26.40f,    22.0f,    13.50f,    12.50f,    11.60f,   10.7f,    10.1f ,    9.5f,  8.80f , 0.0f };
	UPROPERTY()
	TArray<float> M_UpGradeRate_0_Arrays_P1_Hei = { 100.0f,   100.0f, 100.0f,   88.0f,   88.0f,     88.0f,   88.0f,   88.0f,    88.0f,    88.0f,    88.0f,    88.0f,   88.0f,    88.0f,    88.0f,  88.0f, 88.0f };

	//保险金金额
//	UPROPERTY()
//	TArray<int32> LInsuranceCoinNum = { 0,20,40,80,100,200,500,500,600,1000,3000,10000,18000,32000,45000,50000 ,0 };
private:
	//当前强化卡片的名称
	UPROPERTY()
	FString CurUpCardName;
	//选择的四叶草名称
	UPROPERTY()
	FString SelectCloverName = TEXT("");
	//选择的四叶草对应的索引
	UPROPERTY()
	int32 SelectCloverIndex = -1;
	//四叶草概率
	UPROPERTY()
	float SelectCloverRate = 0.f;
	UPROPERTY()
	FMaterialBase CloverData;

	//四叶草的格子
	UPROPERTY()
	class UUI_PlayerBagMaterialGrid* UICloverGrid = nullptr;

	//卡片数据数组
	UPROPERTY()
	TArray<FSynModelCardUpgradeData> CardDatas;
private:
	//初始化组件
	void InitWidgets(
		UButton*& CardButt, const FString& WidgetName1,
		UImage*& CardGradeImg, const FString& WidgetName2,
		UTextBlock*& CardPriceText, const FString& WidgetName3
	);
	//更新概率文字
	void UpdateUpRateText(const FString& Content);
public:
	/************************************************************************/
	/*                          四叶草显示界面                               */
	/************************************************************************/
	//材料显示界面
	UPROPERTY()
	UScrollBox* ScrollBox_Clover = nullptr;
	//四叶草界面
	UPROPERTY()
	UUniformGridPanel* UniformGridPanel_Clover = nullptr;
	//四叶草加载器
	UPROPERTY()
	UItemLoadManager* ItemLoadManager_Clover = nullptr;
	//绑定函数的名称
	UPROPERTY()
	TArray<FMaterialsSerachTypeBind> M_BindFunctionName_Materials;
	//创建材界面
	UFUNCTION()
	UWidget* WidgetCreate_InitMaterial(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
	void WidgetRefresh_UpdateMaterial(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置->材料数据->四叶草-转职道具-等【材料区域的统一材料设置】
	UFUNCTION()
	void SetMaterialsData(
		UUI_PlayerBagMaterialGrid* _Grid,
		UItemDataTable* _CardData,
		int32 _Index,
		const TArray<FMaterialsSerachTypeBind>& _BindFuncName
	);
	//加载四叶草-卡片制作界面
	UFUNCTION()
	void LoadCloversToMakeCard(
		const TArray<FMaterialsSerachTypeBind>& _BindFuncName,
		const FMaterialsSerachKeyWordsIgnore& IgnoreKeyWords
	);


	/************************************************************************/
	/*                          卡片显示界面                                 */
	/************************************************************************/

	FOnCardLoad OnCardLoad;
	//卡片界面加载器(共用于 卡片强化，1卡片转职，2金卡进化等等)
	UPROPERTY()
	UItemLoadManager* ItemLoadManager_Card = nullptr;
	//卡片（强化，转职，等）统一共用滑动界面
	UPROPERTY()
	UScrollBox* ScrollBox_Card_Uniform = nullptr;
	//卡片（强化，转职，等）统一共用格子界面
	UPROPERTY()
	UUniformGridPanel* Uniform_CardGrid = nullptr;
	//创建材界面
	UFUNCTION()
	UWidget* WidgetCreate_InitCards(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
	void WidgetRefresh_UpdateCards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置卡片数据
	UFUNCTION()
	void SetCardData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _Data, int32 _Index);
	//加载卡片
	void LoadCards(const TArray<FItemCard>& CardsData, const int32& LoadCount);
	//加载卡片
	void LoadCards();
	//添加卡片
	bool AddCard(const int32 Index, FSynModelCardUpgradeData CopyData);
};
