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
class UUniformGridPanel;
class UCanvasPanelSlot;
class UUI_EradicateCard;
class UUI_GamePlayerHead;

UCLASS()
class FVM_API UUI_CardBar : public UWidgetBase
{
	GENERATED_BODY()
public:
	//卡片格子界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* CardPanel;
public:
	//初始化
	virtual bool Initialize() override;
	//初始化
	void Init();
	//添加卡片
	void AddCard(UWidget* _Wid);
	//添加角色信息
	void AddPlayerInfor(UUI_GamePlayerHead* PlayerInfor);
};
