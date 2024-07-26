// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SynModel_GoldCardUp.generated.h"

/**
 *
 */

class UUI_PlayerSynthesis;
class UButton;
class UImage;


//卡片的选择
USTRUCT()
struct FGoldCardUpCardType
{
	GENERATED_USTRUCT_BODY()

public:
	//卡片名称
	UPROPERTY()
		FString CardName = FString();
	//卡片在背包中的索引
	UPROPERTY()
		int32 PlayerBagIndex = -1;
	//卡片等级
	UPROPERTY()
		int32 CardGrade = 0;
	//卡片头像路径
	UPROPERTY()
		FString CardHeadPath = FString();
	//UI格子显示对象
	UPROPERTY()
		class UUI_PlayerBagCardGrid* Grid = nullptr;
	//UI格子对应的索引
	UPROPERTY()
		int32 UIGridIndex = -1;
public:
	//初始化按钮
	void InitClass(UButton* ButtIns, class USynModel_GoldCardUp* CurClass);
	//显示按钮
	void ShowButton(const FString& HeadPath);
private:
	//卡片按钮
	UPROPERTY()
		UButton* CardButt = nullptr;
	//金卡进化界面
	UPROPERTY()
		class USynModel_GoldCardUp* Class = nullptr;
};

UCLASS()
class FVM_API USynModel_GoldCardUp : public UObject
{
	GENERATED_BODY()

public:

	//设置卡片数据
	bool AddCardData(const FGoldCardUpCardType& NewData);
	//更新显示
	void UpdateListData();
	//搜索材料
	void SearchMaterials();

	//获取第一张卡的按钮
	UButton* GetCardFristButt();
	//获取第二张卡的按钮
	UButton* GetCardSecondButt();
	//获取金卡进化的按钮
	UButton* GetCardUpButt();
	//获取金卡进化凭证的图像
	UImage* GetCardUpMaterialImg();
	//获取进化目标卡片
	UImage* GetTargetCardImg();


	//初始化
	void InitializeBySynthesis(class UUI_PlayerSynthesis* UI_PlayerSynthesis);
	//界面重置
	void WidgetReset();
	//数据加载
	void WidgetResetLoadData();
	//当卡片加载时
	UFUNCTION()
		void OnCardLoad(class UUI_PlayerBagCardGrid* Grid, class UItemDataTable* Data, int32 Index);
protected:
	//取消选择
	UFUNCTION()
		void CancelCard1();
	UFUNCTION()
		void CancelCard2();
	//卡片进化
	UFUNCTION()
		void CardUp();
private:
	UPROPERTY()
		UUI_PlayerSynthesis* PlayerSynthesis = nullptr;
	//第一张卡
	UPROPERTY()
		UButton* CardFrist_Butt = nullptr;
	//第二张卡
	UPROPERTY()
		UButton* CardSecond_Butt = nullptr;
	//对应的进化凭证
	UPROPERTY()
		UImage* CardUpMaterial = nullptr;
	//对应的进化的卡片
	UPROPERTY()
		UImage* TargetCard_Img = nullptr;
	//卡片进化按钮
	UPROPERTY()
		UButton* CardUp_Butt = nullptr;

	//检索成功
	UPROPERTY()
		bool bSearchOk = false;
	//进化凭证名称
	UPROPERTY()
		FString CardUpMaterialName = FString();
	//进化凭证的图片路径
	UPROPERTY()
		FString CardUpMaterialHeadPath = FString();
	//进化凭证在背包中的索引
	UPROPERTY()
		int32 CardUpMaterialPlayerBagIndex = -1;
	//对应进化卡片名称
	UPROPERTY()
		FString TargetCardName = FString();
	//对应进化卡片的图片路径
	UPROPERTY()
		FString TargetCardHeadPath = FString();

	//卡片数据
	UPROPERTY()
		TArray<FGoldCardUpCardType> CardsData =
	{
		FGoldCardUpCardType(),
		FGoldCardUpCardType()
	};
};
