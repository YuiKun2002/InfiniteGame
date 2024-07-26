// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"

//包含抽奖结构
#include "Data/TreviFountainStruct.h"

#include "UI_TreviFountain_3.generated.h"

/**
 *  新年抽奖活动
 */

class UUniformGridPanel;
class UCanvasPanel;
class UTextBlock;
class UButton;
class UImage;
class URichTextBlock;

UCLASS()
class FVM_API UUI_TreviFountain_3 : public UWidgetBase
{
	GENERATED_BODY()
private:
	//奖品内容展示区域
	UPROPERTY()
		TArray<UCanvasPanel*> M_ItemSlotShowPanels;
	//抽奖获取奖励的界面
	UPROPERTY()
		TArray<UCanvasPanel*> M_GetItemPanels;
private:
	//奖池
	FTreviFountainRandomCategoyStruct M_FTreviFountainRandomCategoyStruct;
	//奖励
	TArray<FTreviFountainItemStruct> M_FTreviFountainItemStructs;
	//奖励数据结构缓冲区
	TArray<FTreviFountainStructData> M_FTreviFountainStructData;
private:
	//单抽奖励图片
	UPROPERTY()
		UImage* M_Get_1_Head = nullptr;
	//10连奖励图片
	UPROPERTY()
		TArray<UImage*> M_Get_2_Heads;
	//抽奖奖励名称文本
	UPROPERTY()
		URichTextBlock* M_URichTextBlock = nullptr;
	//文本内容
	UPROPERTY()
		FString M_TextContent;
public:
	//界面初始化
	bool Initialize() override;
public:
	//初始化内容
	UFUNCTION(BlueprintCallable)
		void InitContent();
public:
	//单抽
	UFUNCTION(BlueprintCallable)
		void Get_1();
	//10连
	UFUNCTION(BlueprintCallable)
		void Get_t10();
	//文本添加（_Reset = false 每调用一次,文本追加一次）
	UFUNCTION(BlueprintCallable)
		FString AddText(const FString& _Content, ETreviFountainItemQualityType _Quality, bool _Reset = false);
};
