// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/EquipmentDataSturct.h"
#include "GameSystem/DataTableAssetData.h"
#include "UI_Heroes_WeaponItem.generated.h"

class UImage;
class UButton;
class UTextBlock;

//武器槽
UCLASS()
class FVM_API UUI_Heroes_WeaponSlot : public UWidgetBase
{
	GENERATED_BODY()
public:
	//武器名称
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WeaponName;
	//武器等级
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* WepaonLevel;
	//武器界面
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtWeaponHead;
};

/**
 * 角色界面-武器道具
 */
UCLASS()
class FVM_API UUI_Heroes_WeaponItem : public UWidgetBase
{
	GENERATED_BODY()
public:
	//设置武器数据
	UFUNCTION()
	void SetWeaponData(const FItemWeaponBase& Data, class UUI_Heroes* UIHero);
	//初始化武器数据
	UFUNCTION()
	void InitWeaponData();
	//选中武器
	UFUNCTION(BlueprintCallable)
	void SelectWeapon();
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Item;
private:
	//武器数据
	UPROPERTY()
	FItemWeaponBase WeaponData;
	//英雄界面
	UPROPERTY()
	class UUI_Heroes* UI_Hero = nullptr;
	/************************************************************************/
	/*                                基础显示                               */
	/************************************************************************/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemLevel;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemStars;
};
