// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"

#include "Game/UI/Synthesis/SynModel_Structures.h"

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "GameSystem/Item/ItemStruct.h"

#include <Components/UniformGridPanel.h>
#include "UI_PlayerSynthesis.generated.h"

class USynModel_CardUpgrade;
class USynModel_WepaonGems;
class UUI_PlayerBagCardGrid;
class UUI_PlayerBagMaterialGrid;
class UUniformGridPanel;
class UScrollBox;
class UTextBlock;
class UButton;
class UCanvasPanel;
class UImage;

//单播代理，当加载每一张卡时
DECLARE_DELEGATE_ThreeParams(FOnCardLoad, UUI_PlayerBagCardGrid* Grid, UItemDataTable* Data, int32 Index);

UCLASS()
class FVM_API UUI_PlayerSynthesis : public UWidgetBase
{
	GENERATED_BODY()
public:
	//当加载每一张卡时
	FOnCardLoad OnCardLoad;
public:

	//-----------------------------------【合成屋大选项卡】宝石强化-选项卡-----------------------------------------


	/************************************************************************/
	/*                              卡片制作                                 */
	/************************************************************************/

	//显示制作卡片
	UFUNCTION(BlueprintCallable)
	void ShowMakeCard();

	//请求配方数据{点击配方的时候请求}
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectCardBlueprintDataRequest(
		USynModel_MakeCard* MakeCardIns,
		FMaterialBase Data,
		UUI_PlayerBagMaterialGrid* Grid,
		int32 Index
	);

	//卡片制作
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectMakeCardRequest(USynModel_MakeCard* MakeCardIns,const FString& composeId,const FString& prescriptionId);

	/************************************************************************/
	/*                              卡片强化                                 */
	/************************************************************************/

	//显示强化卡片
	UFUNCTION(BlueprintCallable)
	void ShowCardUpgrade();
	//显示卡片转职
	UFUNCTION(BlueprintCallable)
	void ShowChangeCard();
	//显示金卡进化界面
	UFUNCTION(BlueprintCallable)
	void ShowGoldCardUpPanel();
	//显示武器开槽镶嵌界面
	UFUNCTION(BlueprintCallable)
	void ShowWeaponGem();
	//显示宝石强化界面
	UFUNCTION(BlueprintCallable)
	void ShowGemUp();
	//显示宝石分解界面
	UFUNCTION(BlueprintCallable)
	void ShowGemSplit();
	/*-------------------------------------------合成屋-界面-成员-共享的-------------------------------------------------*/


	//选择加载宝石卡片界面
	UFUNCTION(BlueprintCallable)
	void Select_GemCardPanel();
	//选择加载金卡进化界面
	UFUNCTION(BlueprintCallable)
	void Select_GoldCardUpPanel();

	//显示界面[针对宝石卡片界面]
	UFUNCTION()
	void ShowCardGemPanel(UCanvasPanel* _UCanvasPanel, uint8 _Index);
	//显示卡片界面或者保险金界面
	UFUNCTION(BlueprintCallable)
	void ShowDataPanel(
		ESlateVisibility _ListDataPanel,//数据列表
		uint8 _ListData_ShowIndex,//数据列表界面索引
		uint8 _SynthesisPanel_ShowIndex,//合成屋界面选项卡界面
		ESlateVisibility _InsurancePanel_State,//保险金
		ESlateVisibility _ColoverPanel_State = ESlateVisibility::Collapsed//四叶草
	);
	//关闭界面
	UFUNCTION(BlueprintCallable)
	void ClearPanel();

	//选择或者取消保险
	UFUNCTION()
	void SelectInsurance();
	//是否勾选保险金
	UFUNCTION(BlueprintCallable)
	bool GetSelectInsurance();

	//设置保险金额文字
	UFUNCTION(BlueprintCallable)
	void SetInsuranceText(const FString& _Text);
	//更新金币文字
	UFUNCTION(BlueprintCallable)
	void Update_CoinText();

	//获取四叶草按钮
	UFUNCTION()
	UButton* GetCloverButton();

	/*----------------------------------------指定卡片加载----------------------------------------*/
	void LoadCards(const TArray<FItemCard*>& CardsData, const int32& LoadCount);
	/*----------------------------------------指定卡片加载----------------------------------------*/

	//加载 香料  四叶草 转职道具专属函数
	/*----------------------------------------指定材料加载----------------------------------------*/
	void LoadMaterials(const TArray<FMaterialsSerachTypeBind>& _BindFuncName, const FMaterialsSerachKeyWordsIgnore& IgnoreKeyWords = FMaterialsSerachKeyWordsIgnore());
	void LoadMaterials(const TArray<FMaterialsSerachTypeBind>& _BindFuncName, const TArray<FString>& IgnoreKeyWords);
	/*----------------------------------------指定材料加载----------------------------------------*/

	//初始化
	virtual bool Initialize() override;
private:
	//卡片强化-选项卡按钮
	UPROPERTY()
	UButton* M_MakeCard_TabButt = nullptr;
	//卡片强化界面
	UPROPERTY()
	UCanvasPanel* M_MakeCardP = nullptr;
	//卡片强化功能
	UPROPERTY()
	class USynModel_MakeCard* M_USynModel_MakeCard = nullptr;
public:
	//获取卡片强化界面功能
	UFUNCTION(BlueprintCallable)
	class USynModel_MakeCard* GetMakeCardFunction();
	/*----------------------------------------卡片制作----------------------------------------*/


	/*----------------------------------------卡片强化----------------------------------------*/
private:
	//卡片强化-选项卡按钮
	UPROPERTY()
	UButton* M_CardUpgrade_TabButt = nullptr;
	//卡片强化界面
	UPROPERTY()
	UCanvasPanel* M_CardUpgradeP = nullptr;
	//卡片强化功能
	UPROPERTY()
	class USynModel_CardUpgrade* M_USynModel_CardUpgrade = nullptr;
public:
	//获取卡片强化界面功能
	UFUNCTION(BlueprintCallable)
	class USynModel_CardUpgrade* GetCardUpgradeFunction();
	/*----------------------------------------卡片强化----------------------------------------*/


	/*----------------------------------------卡片转职----------------------------------------*/
private:
	//卡片转职-选项卡按钮
	UPROPERTY()
	UButton* M_CardChange_TabButt = nullptr;
	//卡片转职界面
	UPROPERTY()
	UCanvasPanel* M_CardChangeP = nullptr;
	//卡片转职功能
	UPROPERTY()
	class USynModel_CardChange* M_USynModel_CardChange = nullptr;
public:
	//获取卡片转职界面功能
	UFUNCTION(BlueprintCallable)
	class USynModel_CardChange* GetCardChangeFunction();
	/*----------------------------------------卡片转职----------------------------------------*/

public:

	/*----------------------------------------金卡进化----------------------------------------*/
	//记录卡片状态是否存放了卡片（名称为""表示没有存放卡片）
	UPROPERTY()
	TArray<FString> M_GoldCard_Name_State;
	//(二转普通卡，二转星座卡，选择与取消)
	UPROPERTY()
	TArray<UButton*> M_GoldCard_Select_Button;
	//进化凭证头像
	UPROPERTY()
	UImage* M_GoldCardUpHead = nullptr;
	//卡片进化结果头像
	UPROPERTY()
	UImage* M_GoldCardUpResultHead = nullptr;
	//进化按钮
	UPROPERTY()
	UButton* M_GoldCardUp_Butt = nullptr;
	//进化凭证（道具名称）
	UPROPERTY()
	FString M_GoldCardUp_ItemName = TEXT("");
	//进化对应的卡片（卡片名称）
	UPROPERTY()
	FString M_GoldCardUp_CardName = TEXT("");
public:
	//需要消耗掉的卡片
	UPROPERTY()
	TArray<FString> M_NeedCards;
	//需要消耗掉的卡片等级
	UPROPERTY()
	TArray<int32> M_NeedGradeCards;
	//对应卡片的UI索引
	UPROPERTY()
	TArray<int32> M_NeedCard_UI_Index;
public:
	//加载金卡进化卡片
	UFUNCTION(BlueprintCallable)
	void LoadGoldCard_Card();
	//移除卡槽卡片
	UFUNCTION(BlueprintCallable)
	void RemoveCardSlot(int32 _Index);
	//金卡进化
	UFUNCTION(BlueprintCallable)
	void GoldCardUp();
	/*----------------------------------------金卡进化----------------------------------------*/


		/*----------------------------------------金卡进化----------------------------------------*/
private:
	//金卡进化-选项卡按钮
	UPROPERTY()
	UButton* M_GoldCardUp_TabButt = nullptr;
	//金卡进化界面
	UPROPERTY()
	UCanvasPanel* M_GoldCardUpP = nullptr;
	//金卡进化功能
	UPROPERTY()
	class USynModel_GoldCardUp* M_USynModel_GoldCardUp = nullptr;
public:
	//获取金卡进化界面功能
	UFUNCTION(BlueprintCallable)
	class USynModel_GoldCardUp* GetGoldCardUpFunction();
	/*----------------------------------------金卡进化----------------------------------------*/





private:
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/
	//武器开槽镶嵌-选项卡按钮
	UPROPERTY()
	UButton* M_WepaonGem_TabButt = nullptr;
	//武器开槽镶嵌界面
	UPROPERTY()
	UCanvasPanel* M_WepaonGemP = nullptr;
	//武器开槽镶嵌功能
	UPROPERTY()
	USynModel_WepaonGems* M_USynModel_WepaonGems = nullptr;
public:
	//获取武器宝石界面功能
	UFUNCTION(BlueprintCallable)
	USynModel_WepaonGems* GetWepaonGemsFunction();
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/

	/*----------------------------------------宝石强化----------------------------------------*/
private:
	//宝石强化-选项卡按钮
	UPROPERTY()
	UButton* M_GemUp_TabButt = nullptr;
	//宝石强化界面
	UPROPERTY()
	UCanvasPanel* M_GemUpP = nullptr;
	//宝石强化功能
	UPROPERTY()
	class USynModel_GemsUpGrade* M_USynModel_GemsUpGrade = nullptr;
public:
	//获取武器宝石界面功能
	UFUNCTION(BlueprintCallable)
	class USynModel_GemsUpGrade* GetGemsUpGradeFunction();
	/*----------------------------------------宝石强化----------------------------------------*/


	/*----------------------------------------宝石分解----------------------------------------*/
private:
	//宝石分解-选项卡按钮
	UPROPERTY()
	UButton* M_GemSplit_TabButt = nullptr;
	//宝石分解界面
	UPROPERTY()
	UCanvasPanel* M_GemSplitP = nullptr;
	//宝石分解
	UPROPERTY()
	class USynModel_GemSplit* M_USynModel_GemSplit = nullptr;
public:
	//获取武器宝石界面功能
	UFUNCTION(BlueprintCallable)
	class USynModel_GemSplit* GetGemSplitFunction();
	/*----------------------------------------宝石分解----------------------------------------*/

private:
	//金币文本
	UPROPERTY()
	UTextBlock* Coin0Text = nullptr;
	//点券文本
	UPROPERTY()
	UTextBlock* Coin1Text = nullptr;

	//保险金文本
	UPROPERTY()
	UTextBlock* InsuranceText = nullptr;
	//是否选择了保险
	UPROPERTY()
	bool M_UpGrade_bSelectInsurance = false;

	//-------------------------卡片界面显示与隐藏-------------------------
	//(宝石卡片，神级进化)等界面   选项卡切换功能(宝石卡片，神级进化)
	UPROPERTY()
	TArray<UWidget*> M_TabSelectPanels;
	//[卡片界面主容器的子界面【卡片界面，装备界面】]
	UPROPERTY()
	TArray<UWidget*> M_TabWidgets_Panels;

	//数据界面1[主容器(包含：卡片界面，装备界面)]
	UPROPERTY()
	UWidget* M_ListDataPanel = nullptr;
	//数据列表显示界面2（集合）
	UPROPERTY()
	TArray<UUniformGridPanel*> M_Cards2_Box;

	//材料显示集合
	UPROPERTY()
	UUniformGridPanel* M_Materials_2_BoxArr = nullptr;
	//四叶草按钮
	UPROPERTY()
	UButton* M_Clover_Butt = nullptr;
	//保险按钮
	UPROPERTY()
	UButton* M_Insurance_Butt = nullptr;
	//卡片界面2---保险金
	UPROPERTY()
	UWidget* M_InsurancePanelPanel = nullptr;
	//强化四叶草界面---四叶草界面
	UPROPERTY()
	UWidget* M_ColoverPanel = nullptr;
	//-------------------------------------------------------------------



	//----------------------------------
	//当前角色背包容量
	UPROPERTY()
	int32 M_CardBagCount = 0;

	//加载器初始化
	UFUNCTION(BlueprintCallable)
	void Loader_Init();

	//----------------------------------四叶草+香料+转职材料【材料区域】--------------------------------------

	//材料加载器 
	UPROPERTY()
	UItemLoadManager* M_UItemLoadManager_Item_Material = nullptr;

	//材料函数绑定
	//绑定函数的名称
	TArray<FMaterialsSerachTypeBind> M_BindFunctionName_Materials;

	//卡片制作滑动界面
	UPROPERTY()
	UScrollBox* M_UScrollBox_Item_Material = nullptr;

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

	//----------------------------------四叶草+香料+转职材料【材料区域】--------------------------------------


	//----------------------------------（强化，转职，等）统一共用滑动界面【卡片区域】--------------------------------------

	//卡片界面加载器(共用于 卡片强化，1卡片转职，2金卡进化等等)
	UPROPERTY()
	UItemLoadManager* M_UItemLoadManager_Card = nullptr;
	//卡片（强化，转职，等）统一共用滑动界面
	UPROPERTY()
	UScrollBox* M_UScrollBox_Card_Uniform = nullptr;
	//卡片（强化，转职，等）统一共用格子界面
	UPROPERTY()
	UUniformGridPanel* M_Uniform_CardGrid = nullptr;
	//界面加载标识(0 卡片强化，1卡片转职，2金卡进化)
	UPROPERTY()
	int32 M_Uniform_LoadCardData_Panel_Index = 0;


	//创建材界面
	UFUNCTION()
	UWidget* WidgetCreate_InitCards(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
	void WidgetRefresh_UpdateCards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置卡片数据
	UFUNCTION()
	void SetCardData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _Data, int32 _Index);
	//----------------------------------（强化，转职，等）统一共用滑动界面【卡片区域】--------------------------------------

	/*----------------------------------------道具加载完成结束函数----------------------------------------*/
};
