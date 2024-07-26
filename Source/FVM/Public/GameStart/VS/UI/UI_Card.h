// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_Card.generated.h"

class ACardCoolDownManager;
class UCardManagerComponent;
class UImage;
class UPaperFlipbook;


UCLASS()
class FVM_API UUI_Card : public UWidgetBase
{
	GENERATED_BODY()
private:
	//是否被选中
	UPROPERTY()
		bool bSelect = false;
	//鼠标是否移入
	UPROPERTY()
		bool bMouseEnter = false;
public:
	//卡片索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardIndex = -1;
	//卡片名称(该名称用于文件资源名称)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Card_ActorName;
	//卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardName;
	//需要的火苗数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_NeedFlame = 250;
	//对应卡片资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftClassPath CardActorSource;
	//是否允许被复制
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEnableCopy = true;
	//卡片按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_Card_Butt = nullptr;
	//卡片选择头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_Card_Select_Head = nullptr;
	//卡片等级图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_UImage_Grade = nullptr;
public:
	virtual bool Initialize() override;
public:
	//获取冷却时间
	UFUNCTION(BlueprintCallable)
		bool GetCoolDownFinish();
	//获取完成比率
	UFUNCTION(BlueprintCallable)
		float GetCoolDownFinishRate();
	//获取背景
	UFUNCTION(BlueprintCallable)
		UButton* GetCardButtonA();
	//获取卡片头像
	UFUNCTION(BlueprintCallable)
		UImage* GetCardHead();
	//获取卡片名称
	UFUNCTION(BlueprintCallable)
		FString GetCardName();
	//获取卡片资源名称
	UFUNCTION(BlueprintCallable)
		FString GetCardActorName();
	//获取卡片是否被选择
	UFUNCTION(BlueprintCallable)
		bool GetCardSelectState();
	//获取鼠标移入移出状态
	UFUNCTION(BlueprintCallable)
		bool GetMouseEnterState();
public:
	//设置卡片名称
	UFUNCTION(BlueprintCallable)
		void SetCardName(const FString& _Name);
	//设置卡片等级
	UFUNCTION(BlueprintCallable)
		void SetCardGrade(const int32& _Grade);
	//设置冷却完成
	UFUNCTION(BlueprintCallable)
		void SetCoolDownFinish();
	//设置选择的状态
	UFUNCTION(BlueprintCallable)
		void SetCardSelect(bool _bSelect);
	//设置鼠标移入移出的状态
	UFUNCTION(BlueprintCallable)
		void SetMouseEnter(bool _bEnter);
public:
	//播放卡片选择动画-bForward正常播放  否则 逆播放
	UFUNCTION(BlueprintCallable)
		void PlaySelectCardAnimation(bool bForward = true);
public:
	//创建卡片动画(Flipbook 3D场景  用于被检测到拖拽时创建)
	UFUNCTION(BlueprintCallable)
		UPaperFlipbook* CreateCard3DSceneFlipbook();
public:
	//选中时的颜色
	UFUNCTION(BlueprintCallable)
		void SelectColor();
public:
	//取消时的颜色
	UFUNCTION(BlueprintCallable)
		void CancelColor();

};
