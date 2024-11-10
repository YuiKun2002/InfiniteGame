﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "NormalAlienByWidget.generated.h"

/**
 *
 */
UCLASS()
class FVM_API ANormalAlienByWidget : public ANormalCapsuleByWidgetBase
{
	GENERATED_BODY()
public:
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
public:
	//初始化
	virtual void BeginPlay() override;
	virtual void MouseInit() override;
	//老鼠更新
	virtual void MouseTick(const float& DeltaTime) override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	UFUNCTION()
	void OnAnimationPlayEnd(class UTrackEntry* Track);
protected:
	void PlayAnim();
private:
	UPROPERTY()
	int32 State = -1;
	UPROPERTY()
	bool bAttackLine = true;
};
