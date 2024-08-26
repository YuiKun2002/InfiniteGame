// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_Heroes.generated.h"

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
	UFUNCTION(BlueprintCallable)
	void InitWeapons();

	//武器加载器
	UPROPERTY()
	UItemLoadManager* ItemLoadManagerItem = nullptr;
	//武器滑动区
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UScrollBox* Scroll_Item = nullptr;
	//武器格子界面
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UUniformGridPanel* Uniform_Item = nullptr;
	//创建格子界面
	UFUNCTION()
	UWidget* WidgetCreateInitWeapons(UItemDataTable* _Data, int32 _Index);
	//刷新格子界面
	UFUNCTION()
	void WidgetRefreshUpdateWeapons(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
};
