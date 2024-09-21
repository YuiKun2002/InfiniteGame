// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "AttackLauncherComponent.generated.h"

/**
 * 攻击发射器组件
 * 拥有【随机攻击特性】20%发射子弹A  30%发射子弹B  50%发射子弹C
 */
UCLASS()
class FVM_API UAttackLauncherComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
	
};
