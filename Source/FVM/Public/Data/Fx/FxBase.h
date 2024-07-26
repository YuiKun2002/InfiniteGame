// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "FxBase.generated.h"

/**
 * 基础特效类
 */
UCLASS()
class FVM_API AFxBase : public AObjectPoolClass
{
	GENERATED_BODY()

public:
	//初始化池子
	virtual void PoolInit(class UObjectPoolManager* PoolManager) override;
	//初始化
	UFUNCTION(BlueprintImplementableEvent)
		void OnInit();
};
