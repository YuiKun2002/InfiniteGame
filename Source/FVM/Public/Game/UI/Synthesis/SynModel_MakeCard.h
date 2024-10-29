// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/ItemStruct.h"
#include "Game/UI/Synthesis/SynModel_Structures.h"
#include "SynModel_MakeCard.generated.h"

/**
 *
 */

class UUI_PlayerSynthesis;
class UScrollBox;
class UUniformGridPanel;
class UItemLoadManager;
class UItemDataTable;
class UWidget;
class UImage;


//格子【配方材料】
USTRUCT()
struct FMakeCardBlueprintMaterialGrid {
	GENERATED_USTRUCT_BODY()
public:
	void Init(
		class UUI_PlayerSynthesis* Class,
		const FString& BgName,
		const FString& HeadName,
		const FString& HeadPath
	);
	void SetImgHead(const FString& HeadPath, const FVector& Color) const;
	void Reset() const;
	void Hidden() const;
public:
	//背景
	UPROPERTY()
	UImage* Bg = nullptr;
	//头像
	UPROPERTY()
	UImage* Head = nullptr;
private:
	//背景图片地址
	FString CurHeadPath = TEXT("");
};


//合成配方材料配置
USTRUCT()
struct FMakeCardBlueprintMaterialData {
	GENERATED_USTRUCT_BODY()
public:
	//是否显示
	UPROPERTY()
	ESlateVisibility Visible;
	//配方材料在背包里的索引
	UPROPERTY()
	int32 PlayerBagIndex = -1;
	//配方材料名称
	UPROPERTY()
	FString BluepName;
	//配方材料的头像
	UPROPERTY()
	FString HeadPath;
};

//合成配方配置
USTRUCT()
struct FMakeCardBlueprintData {
	GENERATED_USTRUCT_BODY()
public:
	//配方完整数据包
	FCardBlueprint Bp;
	//配方在背包里的索引
	UPROPERTY()
	int32 PlayerBagIndex = -1;
	//配方名称
	UPROPERTY()
	FString BluepName;
	//合成的防御卡名称
	UPROPERTY()
	FString TargetCardName;
	//合成的防御卡的类型
	UPROPERTY()
	ECardType TargetCardType;
	//合成的防御卡的ID
	UPROPERTY()
	FString TargetCardID;
	//配方的头像
	UPROPERTY()
	FString HeadPath;
	//配方的UI格子 
	UPROPERTY()
	class UUI_PlayerBagMaterialGrid* Grid = nullptr;
	//配方的UI格子索引
	UPROPERTY()
	int32 UIGridIndex = -1;
private:
	//合成配方的材料
	UPROPERTY()
	TArray<FMakeCardBlueprintMaterialData> Materials = {
	FMakeCardBlueprintMaterialData(),
	FMakeCardBlueprintMaterialData(),
	FMakeCardBlueprintMaterialData()
	};
public:
	//显示外观，更新配方列表
	void ShowStyle(class USynModel_MakeCard* Class);
	//取消配方的选择
	void CancelBlueprint();
	//设置配方
	void SetBlueprintMaterials(const TArray<FString>& CurMaterials);
	//获取配方
	const TArray<FMakeCardBlueprintMaterialData>& GetBlueprintMaterials();
	//添加配方
	void AddNewBlueprint(const FCardBlueprint& BlueprintData, class UUI_PlayerBagMaterialGrid* BGrid, int32 BUIGridIndex, class USynModel_MakeCard* BClass);
};


//香料配置
USTRUCT()
struct FMakeCardSpicesData {
	GENERATED_USTRUCT_BODY()
public:
	//香料的UI格子 
	UPROPERTY()
	class UUI_PlayerBagMaterialGrid* Grid = nullptr;
	//香料索引
	UPROPERTY()
	int32 PlayerBagIndex = -1;
	//香料名称
	UPROPERTY()
	FString SpicesName;
	//香料可以制作的防御卡等级
	UPROPERTY()
	int32 MakeCardGrade = 0;
	//香料的基础数据
	UPROPERTY()
	FMaterialBase SpicesData;
public:
	//选择香料
	void SetSelectSpices(FMakeCardSpicesData NewData);
	//取消选择
	void CancelSelectSpices(class USynModel_MakeCard* Class);
	//使用香料
	void Use();
	//获取是否选择香料
	bool bSelectSpices();
	//获取制作卡片的等级
	int32 GetMakeCardGrade();
};

UCLASS(Blueprintable)
class FVM_API USynModel_MakeCard : public UObject
{
	GENERATED_BODY()
public:


	////加载卡片-卡片制作界面
	//UFUNCTION()
	//	void LoadCardsToMakeCard();


	//获取香料的按钮
	UFUNCTION()
	UButton* GetSpicesButt();
	//获取香料的数据
	UFUNCTION()
	FMakeCardSpicesData& GetSpicesData();
	//取消香料的选择
	UFUNCTION()
	void CancelSelectSpices();

	//获取当前制作的卡片名称
	UFUNCTION()
	FString GetCurrentCardName();

	//获取配方按钮
	UFUNCTION()
	UButton* GetBlueprintButt();
	//获取配方数据
	UFUNCTION()
	FMakeCardBlueprintData& GetBlueprintData();
	//添加新配方
	UFUNCTION(BlueprintCallable)
	void AddNewBlueprint(
		const FCardBlueprint& LBlueprintData,
		UUI_PlayerBagMaterialGrid* BGrid,
		int32 BUIGridIndex,
		USynModel_MakeCard* BClass
	);
	//取消配方的选择
	UFUNCTION(BlueprintCallable)
	void CancelSelectBlueprint();
	//卡片制作
	UFUNCTION()
	void MakeCard();
	//检查配方
	UFUNCTION(BlueprintCallable)
	bool CheckBlueprint();
	//检查圣水
	UFUNCTION(BlueprintCallable)
	void CheckSpices();
	//检查制作
	UFUNCTION(BlueprintCallable)
	void CheckMakeCard();
	//获取合成材料的UI格子组合
	UFUNCTION()
	const TArray<FMakeCardBlueprintMaterialGrid>& GetBlueprintMaterialGrid() const;
	//获取制作配方的按钮
	UFUNCTION()
	UButton* GetMakeCardButt();
	//获取合成屋
	UFUNCTION()
	UUI_PlayerSynthesis* GetPlayerSynthesis();
private:
	UPROPERTY()
	UUI_PlayerSynthesis* PlayerSynthesis = nullptr;
	//合成材料格子
	UPROPERTY()
	TArray<FMakeCardBlueprintMaterialGrid> BlueprintMaterialGrid;
	//香料数据
	UPROPERTY()
	FMakeCardSpicesData SpicesData;
	//配方数据
	UPROPERTY()
	FMakeCardBlueprintData BlueprintData;
	//配方按钮
	UPROPERTY()
	UButton* Blueprint_Butt = nullptr;
	//香料按钮
	UPROPERTY()
	UButton* Spices_Butt = nullptr;
	//制作按钮
	UPROPERTY()
	UButton* MakeCard_Butt = nullptr;
	//当前卡片制作成功的名称
	UPROPERTY()
	FString CardName;
public:
	void InitializeBySynthesis(UUI_PlayerSynthesis* Class);
	void WidgetReset();
	UFUNCTION(BlueprintCallable)
	void WidgetResetLoadData();
	/************************************************************************/
	/*                          香料显示界面                                 */
	/************************************************************************/
	//香料格子
	UPROPERTY()
	class UUI_PlayerBagMaterialGrid* SpiceUI = nullptr;
	//选择索引
	UPROPERTY()
	int32 SelectSpiceIndex = 0;
	//材料显示界面
	UPROPERTY()
	UScrollBox* ScrollBox_Spice = nullptr;
	//香料界面
	UPROPERTY()
	UUniformGridPanel* UniformGridPanel_Spice = nullptr;
	//香料加载器
	UPROPERTY()
	UItemLoadManager* ItemLoadManager_Spice = nullptr;
	//绑定函数的名称
	UPROPERTY()
	TArray<FMaterialsSerachTypeBind> M_BindFunctionName_Materials;
	//当前香料
	UPROPERTY()
	TArray<FMaterialBase> CurSpices;
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
		FMaterialBase _CardData,
		int32 _Index,
		const TArray<FMaterialsSerachTypeBind>& _BindFuncName
	);
	//加载香料-卡片制作界面
	UFUNCTION()
	void LoadSpicesToMakeCard(
		const TArray<FMaterialsSerachTypeBind>& _BindFuncName,
		const FMaterialsSerachKeyWordsIgnore& IgnoreKeyWords
	);
	//初始化香料的选择
	UFUNCTION()
	void InitSpicesSelect();
	//下一个香料的选择
	UFUNCTION()
	void NextSpiceSelect();

	/************************************************************************/
	/*                           配方、配方原材料显示                         */
	/************************************************************************/
	//显示配方等
	UPROPERTY()
	UUniformGridPanel* M_Materials_Box = nullptr;
	UPROPERTY()
	UScrollBox* M_UScrollBox_MakeCard_Material = nullptr;
	UPROPERTY()
	UItemLoadManager* M_UItemLoadManager_MakeCard_Materials = nullptr;
	//创建材界面
	UFUNCTION()
	UWidget* WidgetCreate_InitMakeCard_Material(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
	void WidgetRefresh_UpdateMakeCard_Material(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//当加载到配方时
	UFUNCTION()
	void OnLoadBlueprint(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index);
	//设置制作卡片界面-配方材料UI的数据
	UFUNCTION()
	void SetMakeCard_Material_PanelData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index);
	//加载材料-卡片制作界面
	UFUNCTION()
	void LoadMaterialsToMakeCard();
	//卡片显示界面
	//UPROPERTY()
	//UUniformGridPanel* M_Cards_Box = nullptr;
	//显示制作好的卡片
	//UPROPERTY()
	//UScrollBox* M_UScrollBox_MakeCard = nullptr;
	//UPROPERTY()
	//UItemLoadManager* M_UItemLoadManager_MakeCard = nullptr;
	//创建材界面
	//UFUNCTION()
	//UWidget* WidgetCreate_InitMakeCard(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	//UFUNCTION()
	//void WidgetRefresh_UpdateMakeCard(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置制作卡片界面UI的数据
	//UFUNCTION()
	//void SetMakeCardPanelData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _CardData, int32 _Index);
};
