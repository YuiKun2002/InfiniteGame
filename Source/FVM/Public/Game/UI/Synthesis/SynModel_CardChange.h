// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/ItemStruct.h"
#include "Widgets/Views/STableViewBase.h"
#include "SynModel_CardChange.generated.h"

/**
 *卡片转职
 */

class UImage;
class UButton;
class UTextBlock;
class UUI_PlayerBagCardGrid;

enum class ESlateVisibility : uint8;

USTRUCT()
struct FCardChangeMaterialType {
	GENERATED_USTRUCT_BODY()
public:
	//转职材料名称
	UPROPERTY()
		FString ChangeMaterialName = TEXT("");
	//材料在背包中的索引
	UPROPERTY()
		int32 PlayerBagIndex = -1;
	//材料头像
	UPROPERTY()
		FString ChangeMaterialHeadPath = TEXT("");
	//对应的图片
	UPROPERTY()
		UImage* MaterialImg = nullptr;
	//是否显示【材料数量决定】
	UPROPERTY()
		ESlateVisibility Visible;
};


USTRUCT()
struct FMainCardChangeType {
	GENERATED_USTRUCT_BODY()
public:
	//卡片名称
	UPROPERTY()
		FString ChangeCardName = TEXT("");
	//卡片在背包中的索引
	UPROPERTY()
		int32 PlayerBagIndex = -1;
	//当前卡片等级
	UPROPERTY()
		int32 CardGrade = 0;
	//转职目标卡片
	UPROPERTY()
		FString TargetChangeCardName = TEXT("");
	//材料头像
	UPROPERTY()
		FString ChangeCardHeadPath = TEXT("");
	//卡片类型
	UPROPERTY()
		ECardType TargetType = ECardType::E_ATK;
	//对应卡片格子
	UPROPERTY()
		UUI_PlayerBagCardGrid* Grid = nullptr;
	//对应卡片的UI索引
	UPROPERTY()
		int32 UIGrideIndex = -1;
	//成功率
	UPROPERTY()
		int32 Rate = 20;
	//转职材料
	UPROPERTY()
		TArray<FCardChangeMaterialType> FCardChangeMaterials =
	{
		FCardChangeMaterialType(),
		FCardChangeMaterialType(),
		FCardChangeMaterialType()
	};
private:
	//保险金金额
	UPROPERTY()
		TArray<float> CardChangeJobInsurance =
	{
	5,10,30,50,
	70,90,110,
	190,270,500,
	1000,2000,5000,
	20000,50000,100000,
	120000 };
public:
	//更新显示
	void ShowStyle(class USynModel_CardChange* Class);
	//获取保险价格
	float GetCardChangeJobInsurancePrice(const int32& Grade);
};

UCLASS()
class FVM_API USynModel_CardChange : public UObject
{
	GENERATED_BODY()
public:
	//初始化数据
	void InitializeBySynthesis(class UUI_PlayerSynthesis* Class);
	//界面重置
	void WidgetReset();
	//加载所有数据
	void WidgetResetLoadData();
	//加载卡片
	void LoadCards();
	//加载材料
	void LoadMaterials();
public:
	//获取转职UI主卡按钮
	UFUNCTION()
		UButton* GetMainChangeCardButt();
	//获取转职按钮
	UFUNCTION()
		UButton* GetChangeCardButt();
	//设置转职概率
	UFUNCTION()
		void SetChangeCardRateText(const FString& Text);
	//获取合成屋
	UFUNCTION()
		UUI_PlayerSynthesis* GetPlayerSynthesis();
	//获取材料3显示界面
	UFUNCTION()
		UCanvasPanel* GetChangeMaterial3P();
	//获取转职卡片数据
	UFUNCTION()
		FMainCardChangeType& GetFMainCardChangeData();
	//取消转职卡片
	UFUNCTION()
		void CancelMainCard();
protected:
	//当卡片加载时
	UFUNCTION()
		void OnCardLoad(UUI_PlayerBagCardGrid* Grid, UItemDataTable* Data, int32 Index);
	//转职卡片
	UFUNCTION()
		void ChangeCard();
private:
	//合成屋
	UPROPERTY()
		UUI_PlayerSynthesis* PlayerSynthesis = nullptr;

	//转职材料显示图3
	UPROPERTY()
		UCanvasPanel* ChangeMaterial3P = nullptr;

	//转职卡片数据
	UPROPERTY()
		FMainCardChangeType MainCardChangeTypeData;

	//转职主卡按钮
	UPROPERTY()
		UButton* MainChangeCard_Butt = nullptr;
	//转职按钮
	UPROPERTY()
		UButton* ChangeCard_Butt = nullptr;
	//转职成功率
	UPROPERTY()
		UTextBlock* ChangeCardRate_Text = nullptr;
};
