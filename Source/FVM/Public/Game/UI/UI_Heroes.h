// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/Item/ItemBaseStruct.h"
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


	

	
};
