// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/EquipmentDataSturct.h"
#include "UI_Heroes_WeaponItem.generated.h"

class UImage;
class UButton;
class UTextBlock;

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
	void SetWeaponData(const FItemWeaponBase& Data);
	//初始化武器数据
	UFUNCTION()
	void InitWeaponData();
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Item;
private:
	//武器数据
	UPROPERTY()
	FItemWeaponBase WeaponData;
};
