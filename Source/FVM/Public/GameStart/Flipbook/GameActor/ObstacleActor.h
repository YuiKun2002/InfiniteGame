// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/MapBaseType.h"
#include "ObstacleActor.generated.h"

/**
 * 障碍物类型【只会带有路线】
 */
UCLASS()
class FVM_API AObstacleActor : public AObstacleBase
{
	GENERATED_BODY()

public:
	//设置线路
	UFUNCTION(BlueprintCallable)
		void SetLine(FLine Line);
	//获取线路
	UFUNCTION(BlueprintCallable)
		FLine GetLine();
	//初始化
	UFUNCTION(BlueprintImplementableEvent)
		void OnInit();
private:
	UPROPERTY()
		FLine CurLine;
};
