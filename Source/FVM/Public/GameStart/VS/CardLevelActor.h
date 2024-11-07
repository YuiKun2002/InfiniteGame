﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "CardLevelActor.generated.h"

/**
 *卡片等级对象
 */
UCLASS()
class FVM_API ACardLevelActor : public ASpineActor
{
	GENERATED_BODY()
public:
	//设置卡片位置
	UFUNCTION(Blueprintcallable)
	void SetCardLevelLocation(
		const FVector& Location,
		const int32& CardLevel,
		const int32& RenderLayer,
		const FVector& Offset
	);
};
