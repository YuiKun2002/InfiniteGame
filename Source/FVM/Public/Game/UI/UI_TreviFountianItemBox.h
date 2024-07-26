// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/TreviFountainStruct.h"
#include "UI_TreviFountianItemBox.generated.h"


class UImage;
class UButton;

UCLASS()
class FVM_API UUI_TreviFountianItemBox : public UWidgetBase
{
	GENERATED_BODY()
private:
	FTreviFountainItemStruct Data;
private:
	//头像
	UPROPERTY()
		UImage* M_Head = nullptr;
	//史诗道具（动画显示）
	UPROPERTY()
		UImage* M_ItemEpic_Image = nullptr;
	//按钮（点击之后显示名称）
	UPROPERTY()
		UButton* M_Button = nullptr;
public:
	//初始化
	bool Initialize() override;
	//设置数据
	void SetTreviFountainItemData(FTreviFountainItemStruct _Data);
public:
	//加载数据
	UFUNCTION(BlueprintCallable)
		void LoadData();
	//显示信息介绍界面
	UFUNCTION(BlueprintCallable)
		void ShowInformation();
	//获取物品名称
	UFUNCTION(BlueprintCallable)
	   FString GetItemName();
	//移除信息介绍界面       
	UFUNCTION(BlueprintCallable)
		void RemoveInformation();
};
