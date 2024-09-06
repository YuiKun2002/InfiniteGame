// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/Item/ItemBaseStruct.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_Weapons.generated.h"

class UTextBlock;
class UUI_Heroes_WeaponSlot;

/**
 * 武器界面
 */
UCLASS()
class FVM_API UUI_Weapons : public UWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FMainWeaponData> MainWeaponDatas;
	UPROPERTY()
	TArray<FSecondaryWeaponData> SecondaryWeaponDatas;
	UPROPERTY()
	TArray<FItemWeaponBase> Weapons;
	//武器加载器
	UPROPERTY()
	UItemLoadManager* ItemLoadManagerItem = nullptr;
	//武器滑动区
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* Scroll_Item = nullptr;
	//武器格子界面
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* Uniform_Item = nullptr;
	//创建格子界面
	UFUNCTION()
	UWidget* WidgetCreateInitWeapons(UItemDataTable* _Data, int32 _Index);
	//刷新格子界面
	UFUNCTION()
	void WidgetRefreshUpdateWeapons(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//初始化武器
	UFUNCTION(BlueprintCallable)
	void InitWeapons();

	//主武器槽
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUI_Heroes_WeaponSlot* MainWeaponSlot;
	//副武器槽
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUI_Heroes_WeaponSlot* SecondaryWeaponSlot;

	////装备武器
	//UFUNCTION()
	//void EquipWeapon(const FItemWeaponBase& Data);
	////装备武器
	//void EquipCurrentWeapon(
	//	const FItemWeaponBase& BaseData,
	//	UUI_Heroes_WeaponSlot* WeaponSlot,
	//	UTextBlock* Text1,
	//	const FText& CText1,
	//	UTextBlock* Text2,
	//	const FText& CText2,
	//	UTextBlock* Text3,
	//	const FText& CText3
	//);
	////主武器槽
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//UUI_Heroes_WeaponSlot* MainWeaponSlot;
	////取消主武器装备
	//UFUNCTION()
	//void CancelMainWeapon();
	////副武器槽
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//UUI_Heroes_WeaponSlot* SecondaryWeaponSlot;
	////取消副武器装备
	//UFUNCTION()
	//void CancelSecondaryWeapon();

	/*
	//攻击力
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_Attack;
	//暴击率
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_Burst;
	//金币花费
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WeaponCostCoin0;
	//生命值
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_Defence;
	//生命值回复速度
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_HealRate;
	//金币花费
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WeaponCostCoin1;*/
};
