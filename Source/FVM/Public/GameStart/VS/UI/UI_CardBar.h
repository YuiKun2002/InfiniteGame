// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_CardBar.generated.h"

class UHorizontalBox;
class UVerticalBox;
class UImage;

class UTextBlock;
class UScrollBox;
class UCanvasPanel;
class UCanvasPanelSlot;
class UUI_EradicateCard;
class UUI_GamePlayerHead;

UCLASS()
class FVM_API UUI_CardBar : public UWidgetBase
{
	GENERATED_BODY()

public:
	//初始化
	virtual bool Initialize() override;
	//初始化
	void Init();
	//添加卡片
	void AddCard(UWidget* _Wid);
	//添加角色信息
	void AddPlayerInfor(UUI_GamePlayerHead* PlayerInfor);
private:
	//铲卡道具2
	UPROPERTY()
		UUI_EradicateCard* M_TCard_Item_2 = nullptr;
private:
	//水平线
	UPROPERTY()
		UImage* M_Bg_1 = nullptr;
	//垂直线
	UPROPERTY()
		UImage* M_Bg_2 = nullptr;
	//拐角图片
	UPROPERTY()
		UImage* M_Bg_3 = nullptr;
	//第二列卡槽
	UPROPERTY()
		UCanvasPanel* M_UCanvasPanel_CardSlot = nullptr;
private:
	//水平卡
	UPROPERTY()
		UHorizontalBox* M_UHorizontalBox_Cards = nullptr;
	//垂直卡
	UPROPERTY()
		UVerticalBox* M_UVerticalBox_Cards = nullptr;
private:
	//显示角色的位置
	UPROPERTY()
		UScrollBox* M_PlayerScrollBox = nullptr;
};
