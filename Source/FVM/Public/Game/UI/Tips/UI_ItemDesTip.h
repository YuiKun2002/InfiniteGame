// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_ItemDesTip.generated.h"

/**
 * 物品提示
 */

 // #include "Game/UI/Tips/UI_ItemDesTip.h"
 // WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemDesTip.BPUI_ItemDesTip_C'

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class FVM_API UUI_ItemDesTip : public UWidgetBase
{
	GENERATED_BODY()
private:
	//确认按钮
	UPROPERTY()
		UButton* M_Ok = nullptr;
	//确认按钮标题名称
	UPROPERTY()
		UTextBlock* M_Ok_TabName = nullptr;
	//取消按钮
	UPROPERTY()
		UButton* M_Cancel = nullptr;
	//取消按钮标题名称
	UPROPERTY()
		UTextBlock* M_Cancel_TabName = nullptr;
	//道具头像
	UPROPERTY()
		UImage* M_ItemHead = nullptr;
public:
	//标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_s_Title = TEXT("");
	//内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_s_Content = TEXT("");
public:
	//获取确认按钮代理
	FOnButtonClickedEvent& GetOkButtonDelegate();
	//获取取消按钮代理
	FOnButtonClickedEvent& GetCancelButtonDelegate();
public:
	//获取确认按钮
	UFUNCTION(BlueprintCallable)
		UButton* GetOkButtonComponent();
	//获取取消按钮
	UFUNCTION(BlueprintCallable)
		UButton* GetCancelButtonComponent();
	//设置确认按钮的文本
	UFUNCTION(BlueprintCallable)
		void SetOkButtonTitleText(FString _Content);
	//设置取消按钮的文本
	UFUNCTION(BlueprintCallable)
		void SetCancelButtonTitleText(FString _Content);
	//设置内容标题文本
	UFUNCTION(BlueprintCallable)
		void SetContentTitleText(FString _Content);
	//设置内容文本
	UFUNCTION(BlueprintCallable)
		void SetContentText(FString _Content);
	//设置道具图片
	UFUNCTION(BlueprintCallable)
		void SetItemHeadPath(FString _2DTexture_Path);
public:
	//初始化
	bool Initialize() override;
};
