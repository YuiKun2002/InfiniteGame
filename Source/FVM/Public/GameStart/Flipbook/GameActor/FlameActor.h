// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "FlameActor.generated.h"

/**
 * 火苗 用于在游戏中提供购买卡片的资源   一般通过资源管理器生成或者对应卡片生成
 */

class UCardSpawnComponent;

UCLASS()
class FVM_API AFlameActor : public AGameActorFlipbookBase
{
	GENERATED_BODY()
private:
	//基础资源值
	UPROPERTY()
		int32 M_Value = 25;
public:

	//设置基本属性
	UFUNCTION(BlueprintCallable)
	void SetFlameValue(const int32& _value);

	const int32& GetFlameValue() const;

	//开始播放动画
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
		void Run();
};
