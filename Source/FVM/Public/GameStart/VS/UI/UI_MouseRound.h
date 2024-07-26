// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MouseRound.generated.h"

class USlider;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UVerticalBox;
class UProgressBar;
class USizeBox;
class UUI_MouseBoss_Slider;

UCLASS()
class FVM_API UUI_MouseRound : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		int32 M_BossSlider_Count = 0;
public:
	//老鼠位置滑动条
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USlider* M_USlider = nullptr;
	//老鼠进度条
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProgressBar* M_UProgressBar = nullptr;
	//旗帜容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCanvasPanel* M_FlagBox = nullptr;
	//老鼠进攻回合文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_MouseRoundText = nullptr;
	//boss鼠列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UVerticalBox* M_Boss_List_VerticalBox = nullptr;
	//boss映射
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, UUI_MouseBoss_Slider*> M_BoosUIMap;
public:
	virtual bool Initialize() override;

	//初始化旗帜
	UFUNCTION(BlueprintCallable)
		void InitFlag(const int32& _RoundTotal);

	//更新老鼠进度条
	UFUNCTION(BlueprintCallable)
		void UpdateMouseRoundSlider(const float& _Curr, const float& _End);

	//更新老鼠回合文字
	UFUNCTION(BlueprintCallable)
		void UpdateMouseRoundText(const int32& _Round);

	//将boss的血量界面添加到列表中
	UFUNCTION(BlueprintCallable)
		void AddMouseBossSlider(const FString& BoosName, UUI_MouseBoss_Slider* _Widget_);

	//移除boss的血条
	UFUNCTION(BlueprintCallable)
		bool RemoveMouseBossSlider(const FString& BoosName);

	//获取boss的血条数量
	UFUNCTION(BlueprintCallable)
		int32 GetMouseBossSliderCount();
};
