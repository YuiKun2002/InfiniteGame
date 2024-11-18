﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameLevelSubsystem.generated.h"

/**
 * 游戏关卡子系统
 */
UCLASS()
class FVM_API UGameLevelSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	//获取游戏关卡子系统
	UFUNCTION(BlueprintPure)
	static UGameLevelSubsystem* GetGameLevelSubsystemStatic();
	//获取地图实例
	UFUNCTION(BlueprintPure)
	class AGameMapInstance* GetGameMapInstance();
public:
	UFUNCTION(BlueprintCallable)
	void SetGameMapInstance(class AGameMapInstance* NewGameMapInstance);
private:
	UPROPERTY()
	class AGameMapInstance* GameMapInstance = nullptr;
};
