// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "SkyAlien.generated.h"

/**
 * 飞行单位的外星人
 */
UCLASS()
class FVM_API ASkyAlien : public ANormalCapsuleBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//死亡
	virtual void MouseDeathed() override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;	
};
