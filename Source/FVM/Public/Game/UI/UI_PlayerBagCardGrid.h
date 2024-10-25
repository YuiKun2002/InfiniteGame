// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include <Components/Button.h>
#include "GameSystem/Item/ItemStruct.h"
#include "UI_PlayerBagCardGrid.generated.h"

class UUI_PlayerSynthesis;
class UUI_ExchangeAndSplit;
class UUI_PlayerBag;
class UButton;
class UImage;

UCLASS()
class FVM_API UUI_PlayerBagCardGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	//卡片数据（不允许地址被修改，使用前必须判断地址有效性）
	FItemCard CopyData;
	//卡片索引
	int32 M_CardIndex = -1;
	//UI索引
	int32 M_UI_Index = -1;
public:
	//合成屋的UI
	UPROPERTY()
	UUI_PlayerSynthesis* M_UI_PlayerSynthesis = nullptr;
	//背包UI
	UPROPERTY()
	UUI_PlayerBag* M_PlayerUIBag = nullptr;
	//其他UI
	UPROPERTY()
	UWidget* M_UI_Other = nullptr;
public:
	//卡片价格文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_CardPriceText;
	//卡片图片路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_CardTexturePath;
public:
	//当卡片选择移除时，调用
	UFUNCTION(BlueprintImplementableEvent)
	void OnRemoveCurrentSelectCard();
private:
	//卡片按钮
	UPROPERTY()
	UButton* M_CardButton = nullptr;
	//卡片等级图片
	UPROPERTY()
	UImage* M_CardGradeImage = nullptr;
public:
	virtual bool Initialize() override;

	//获取按钮点击事件
	FOnButtonClickedEvent& GetButtonClickEvent();
	//设置卡片数据
	void SetFItemCardData(FItemCard  _FItemCardData);
	//设置合成屋的UI
	void SetUI_PlayerSynthesis(UUI_PlayerSynthesis* const _PlayerSynthesis);
	//设置其他UI
	void SetUI_Other(UWidget* const _Widget);
	//设置索引
	void SetCardIndex(const int32& _Value);
	void SetUIIndex(const int32& _Value);
public:
	//获取索引
	int32 GetCardIndex();
	int32 GetUIIndex();
	//获取卡片数据
	FItemCard* const GetFItemCardData();
public:
	virtual FString ToString_Implementation() override;
public:
	//显示卡片的细节
	UFUNCTION()
	void ShowCardDetails();

	/*----------------------------------------卡片选择----------------------------------------*/

	//选择当前卡片(游戏准备-选卡)
	UFUNCTION()
	void SelectCurrentCard();
	//移除当前选择的卡片
	UFUNCTION()
	void RemoveCurrentSelectCard();
	/*----------------------------------------卡片选择----------------------------------------*/

	/*----------------------------------------卡片强化----------------------------------------*/
	//添加合成屋的强化用卡卡槽
	UFUNCTION()
	void AddUpGradeFunc();
	/*----------------------------------------卡片强化----------------------------------------*/
	/*----------------------------------------卡片转职----------------------------------------*/
	UFUNCTION()
	void AddSynthesisChangeJobs();
	/*----------------------------------------卡片转职----------------------------------------*/
	/*----------------------------------------金卡进化----------------------------------------*/
	UFUNCTION()
	void AddSynthesisGoldCardUp();
	/*----------------------------------------金卡进化----------------------------------------*/

	/*----------------------------------------UI道具兑换和分解----------------------------------------*/
	//将卡片添加到分解槽
	UFUNCTION()
	void AddExchangeSplit_SplitSlot();
	/*----------------------------------------UI道具兑换和分解----------------------------------------*/




public:
	//获取卡片
	UFUNCTION(BlueprintCallable)
	UButton* GetButton();

	//更新按钮的纹理
	UFUNCTION(BlueprintCallable)
	void UpdateButtonTexture(const FString& _Price);

	//获取卡片纹理
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetTexture();

	//播放动画
	UFUNCTION(BlueprintCallable)
	void PlayBeginAnim();
};
