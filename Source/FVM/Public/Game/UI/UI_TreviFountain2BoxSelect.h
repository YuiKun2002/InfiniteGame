// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_TreviFountain2BoxSelect.generated.h"

/**
 * 法老抽奖 自选卡片
 */


class UImage;
class UButton;
class UTextBlock;
class USizeBox;
class UUI_TreviFountain2;

UCLASS()
class FVM_API UUI_TreviFountain2BoxSelect : public UWidgetBase
{
	GENERATED_BODY()
private:
	//奖池
	UPROPERTY()
		UUI_TreviFountain2* M_UUI_TreviFountain2 = nullptr;
	//头像
	UPROPERTY()
		UButton* M_Head = nullptr;
	//尺寸盒子
	UPROPERTY()
		USizeBox* M_USizeBox = nullptr;
	//选择
	UPROPERTY()
		UButton* M_Select = nullptr;
	//选择文字
	UPROPERTY()
		UTextBlock* M_SelectContent = nullptr;
private:
	//道具名称
	UPROPERTY()
		FString M_ItemName;
	//是否被选择 
	UPROPERTY()
		bool M_bSelect = false;
public:
	bool Initialize() override;
public:
	//获取按钮
	UButton* const GetButton();
	//设置父对象
	void SetUI_TreviFountain2(UUI_TreviFountain2* _UUI_TreviFountain2);
	//设置头像
	void SetHead(const FString& _HeadPath);
	//设置道具名称
	void SetItemName(const FString& _Name);
public:
	//获取道具名称
	UFUNCTION(BlueprintCallable)
		const FString& GetItemName();
public:
	//选择道具
	UFUNCTION()
		void Select_Item();
	//取消选择
	UFUNCTION()
		void Cancel_Item();
};
