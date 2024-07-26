// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/PlayerStructManager.h"
#include "UI_PlayerBagEquipmentGrid.generated.h"

/**
 *
 */

 /*
 这个结构用于接收背包UI中穿过来的数据->如果当前物品是背包那么就会使用到这个数据
 */
 //角色背包结构
struct FPlayerBagData {
public:
	//名称
	FString VectorName;
	//界面索引
	uint8 M_PanelNumber;
	//第几个背包
	uint8 M_GridNumber;
};

class UButton;
class UImage;
class UTextBlock;
class UUI_PlayerBag;

UCLASS()
class FVM_API UUI_PlayerBagEquipmentGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	//道具按钮->调出界面
	UPROPERTY()
		UButton* M_UButton = nullptr;
	//道具等级->等级图片显示
	UPROPERTY()
		UImage* M_UItemGrade = nullptr;
private:
	//物品的ID号
	UPROPERTY()
		int32 M_ItemID = -1;
private:
	UPROPERTY()
		FString M_ItemName;
	//物品描述
	UPROPERTY()
		FString M_ItemDescribe;
public:
	//指向背包的数据
	FEquipmentBase* M_EuipmentData;
public:
	//背包UI
	UPROPERTY()
		UUI_PlayerBag* M_BagUI = nullptr;
public:
	//背包数据
	FPlayerBagData M_FPlayerBagData;
public:
	//道具显示数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_ItemCountText = nullptr;
public:
	virtual bool Initialize() override;
public:
	//设置文本显示的数量
	UFUNCTION()
		void SetItemCountText(const FString& _Text, bool _Show);
	//设置物品ID
	UFUNCTION()
		void SetItemID(int32 _ID);
	//设置名称
	UFUNCTION()
		void SetItemName(const FString& _Name);
	//设置描述
	UFUNCTION()
		void SetItemDescribe(const FString& _Index);
public:
	//获取物品ID->获取当前UI的位置ID
	UFUNCTION(BlueprintCallable)
		int32 GetItemID();
	//获取物品ID->获取当前物品数据的ID
	UFUNCTION(BlueprintCallable)
		int32 GetItemID_EquipData();
	//获取名称
	UFUNCTION(BlueprintCallable)
		FString GetItemName();
public:
	//重新加载背包装备栏
	UFUNCTION()
		void RelaodBagEquipment();
	//背包功能
	UFUNCTION()
		void AttachToBag();
	//创建一个新的礼盒显示界面
	UFUNCTION()
		void CreateNewGiftBox();
	//装备角色服装
	UFUNCTION()
		void UsePlayerSuit();
public:
	void PlayAnimation_1();
public:
	//文本显示
	UFUNCTION()
		void ShowText();
	//角色着装显示
	UFUNCTION()
		void ShowPlayerSuit();
	//显示角色武器，是否装备
	UFUNCTION()
		void ShowPlayerWeapon();
public:
	//设置物品等级
	UFUNCTION(BlueprintCallable)
		void SetItemGrade(const FString& _GradePath);
public:
	//装备武器装备
	UFUNCTION()
		void Equip_Weapon(FPlayerWepaon& _LFPlayerWepaon);
	//装备主武器
	UFUNCTION()
		void Equip_MainWeapon();
	//装备副武器
	UFUNCTION()
		void Equip_SecondWeapon();
	//装备超级武器
	UFUNCTION()
		void Equip_SuperWeapon();
public:
	//获取按钮
	UButton* GetButton();
};
