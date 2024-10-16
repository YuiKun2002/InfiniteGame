// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "SubmersibleAlien.generated.h"

/**
 * 潜水外星人
 */
UCLASS()
class FVM_API ASubmersibleAlien : public ANormalCapsuleBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
};
