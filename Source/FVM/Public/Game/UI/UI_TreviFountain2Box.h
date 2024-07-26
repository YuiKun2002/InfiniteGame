// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/TreviFountainStruct.h"
#include "UI_TreviFountain2Box.generated.h"

class UUI_TreviFountain2;
class UButton;
class UImage;

/**
 * 法老抽奖的开箱子的盒子
 */

UCLASS()
class FVM_API UUI_TreviFountain2Box : public UWidgetBase
{
	GENERATED_BODY()
public:
	//当前抽取到的物品头像路径
	UPROPERTY()
		FString M_CurrentItemHeadPath = TEXT("");
private:
	UPROPERTY()
		UUI_TreviFountain2* M_UUI_TreviFountain2 = nullptr;
private:
	UPROPERTY()
		bool M_bGet = false;
	//道具名字
	UPROPERTY()
		FString M_Name;
	//单抽的价格
	UPROPERTY()
		int32 M_GetPrice = 200;
private:
	//宝箱默认时候的图片
	UPROPERTY()
		FString M_BoxDefTexture2DPath;
	//宝箱打开时候的图片
	UPROPERTY()
		FString M_BoxOpenTexture2DPath;
	//宝箱图像的位移和缩放(默认大小)
	UPROPERTY()
		FVector DefScaleAndPosition;
	//宝箱图像的位移和缩放(缩放后的大小)
	UPROPERTY()
		FVector OffsetScaleAndPosition;
private:
	//保险抽奖
	UPROPERTY()
		UButton* M_UButton = nullptr;
	//获取的道具图标
	UPROPERTY()
		UImage* M_UImage = nullptr;
	//获取的道具极品道具图标
	UPROPERTY()
		UImage* M_UImageEpicTip = nullptr;
public:
	//界面初始化
	bool Initialize() override;
public:
	//初始化
	void SetUI_TreviFountain2(UUI_TreviFountain2* _UI_TreviFountain2);
	//设置抽奖完成
	void SetFinish();
	//设置宝箱样式
	void SetBoxStyle(const FString& _ItemHeadPath, bool _bOpened = false);
public:
	//抽取一次（返回头像路径）
	UFUNCTION()
		void Get();
	//获取物品名称
	UFUNCTION(BlueprintCallable)
		FString GetItemName();
public:
	//发送道具
	void SendItem(const FTreviFountainItemStruct& _Item);
public:
	//重置
	UFUNCTION(BlueprintCallable)
		void Reset();
	//初始化图片资源（默认宝箱的图形，打开宝箱的图形，默认宝箱图像的位移和缩放，打开宝箱后图像的位移和缩放）
	UFUNCTION(BlueprintCallable)
		void InitBoxImgSource(const FString& _DefPath, const FString& _OpenPath, FVector _DefScaleAndPosition, FVector _OffsetScaleAndPosition, int32 GetItemCoin = 200);
};
