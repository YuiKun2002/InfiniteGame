﻿ #pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/EquipmentDataSturct.h"
#include "UI_HeroItem.generated.h"

class UImage;
class UButton;

/**
 * 角色物品
 */
UCLASS()
class FVM_API UUI_HeroItem : public UWidgetBase
{
	GENERATED_BODY()

public:

	//设置角色数据
	UFUNCTION()
	void SetWeaponData(const FItemHeroBase& Data, class UUI_Heroes* UIHeroes);

	//初始化角色数据
	UFUNCTION()
	void InitWeaponData();

	//选择
	UFUNCTION(BlueprintCallable)
	void Select();

	//当被选择时
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelect(UUI_HeroItem* UIHeroItem, class UUI_Heroes* UIHeroes);

	//取消选择
	UFUNCTION(BlueprintCallable)
	void Cancel();

	//获取数据
	UFUNCTION(BlueprintPure)
	void GetHeroData(FItemHeroBase& Data);
	FItemHeroBase& GetHeroDataByRef();
private:

	//角色数据
	UPROPERTY()
	FItemHeroBase HeroData;

	//英雄UI
	UPROPERTY()
	class UUI_Heroes* UI_Heroes = nullptr;

public:

	//选择框
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* SelectBox;

	//按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Item;

};
