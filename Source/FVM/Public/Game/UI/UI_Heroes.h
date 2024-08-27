﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/Item/ItemBaseStruct.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_Heroes.generated.h"

#define HEROITEM_HEROITEM FName(TEXT("HeroItem"))
#define HEROITEM_ITEMLEVEL FName(TEXT("ItemLevel"))


//英雄物品缓存对象
UCLASS()
class FVM_API UHeroItemDataAssetCache : public UGameDataAssetCache
{
	GENERATED_BODY()
public:
	FSoftObjectPath GetResource(const FName& Name, const FName& RowName, const int32& Key);
private:
	//图片数据
	UPROPERTY()
	DataTableAssetData<FItemResourceData> Data;
	UPROPERTY()
	TMap<int32, FSoftObjectPath> Datas;
	//名称
	UPROPERTY()
	FName ResourceName;
};


/**
 * 英雄界面
 */
UCLASS()
class FVM_API UUI_Heroes : public UWidgetBase
{
	GENERATED_BODY()
public:
	/************************************************************************/
	/*                                角色界面                               */
	/************************************************************************/
	//初始化角色界面
	UFUNCTION(BlueprintCallable)
	void InitHeroes();

	/************************************************************************/
	/*                                武器界面                               */
	/************************************************************************/
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
	UFUNCTION(BlueprintCallable)
	void InitWeapons();
};
