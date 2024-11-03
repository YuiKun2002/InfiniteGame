// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Game/UI/HeroesData.h"
#include "Data/EquipmentDataSturct.h"
#include "GameSystem/DataTableAssetData.h"
#include "UI_Heroes_WeaponItem.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UUI_Heroes_WeaponSlot;

#define WEAPON_MAXLEVEL 30

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
	//武器等级图片
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemLV;
	//武器等级文本
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemLevel;
public:
	//初始化数据
	UFUNCTION(BlueprintCallable)
	void InitData(class UUI_Weapons* UI_Weapon, const FMainWeaponData& WeaponBaseData, bool bFrist = true);
	//初始化【装备位置】
	UFUNCTION(BlueprintCallable)
	void InitEquipPosition(bool bFrist);
	//装备武器
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(bool bFirst);
	//移除武器
	UFUNCTION(BlueprintCallable)
	void RemoveWeapon();
	//分解武器
	UFUNCTION(BlueprintCallable)
	void Decompose(UUI_Heroes_WeaponSlot* OtherWeapon);
	//强化武器
	UFUNCTION(BlueprintCallable)
	void Upgrade();
	//进化武器
	UFUNCTION(BlueprintCallable)
	void Evolve();
public:
	//获取武器数据
	UFUNCTION(BlueprintPure)
	void GetWeaponData(FMainWeaponData& WeaponBaseData);
	//获取武器等级
	UFUNCTION(BlueprintPure)
	int32 GetWeaponLevel();
	//获取武器星级
	UFUNCTION(BlueprintPure)
	int32 GetWeaponStars();
	//获取武器数据
	UFUNCTION(BlueprintPure)
	void GetWeaponDetailData(FMainWeaponData& MainWeaponData);
	//获取武器是否能升级
	UFUNCTION(BlueprintPure)
	bool GetUpgradeWeapon();
	//获取武器是否能进化
	UFUNCTION(BlueprintPure)
	bool GetEvolveWeapon();
	//获取武器UI界面
	UFUNCTION(BlueprintPure)
	class UUI_Weapons* GetUIWeapon();
private:
	UFUNCTION()
	void DecomposeWeapon();
	UFUNCTION()
	void UpdageWeapon(FMainWeaponData& Data);
	UFUNCTION()
	void EvolveWeapon(FMainWeaponData& Data);
private:
	UPROPERTY()
	class UUI_Weapons* UIWeapon = nullptr;
	UPROPERTY()
	FMainWeaponData WeaponData;
	UPROPERTY()
	bool bFristWeapon = true;
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
	void SetWeaponData(const FMainWeaponData& Data, class UUI_Weapons* UIWeapon);
	//初始化武器数据
	UFUNCTION()
	void InitWeaponData();
	//选中武器
	UFUNCTION(BlueprintCallable)
	void SelectWeapon();
	//当武器被选择时
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectWeaponData(class UUI_Weapons* UIWeapon, const FMainWeaponData& WeaponBaseData);
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Item;
private:
	//武器数据
	UPROPERTY()
	FMainWeaponData WeaponData;
	//英雄界面
	UPROPERTY()
	class UUI_Weapons* UI_Weapon = nullptr;

	/************************************************************************/
	/*                                基础显示                               */
	/************************************************************************/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemLevel;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemStars;
};
