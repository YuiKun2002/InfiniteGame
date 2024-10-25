// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "TSubmersibleAlien.generated.h"

/**
 * 潜水艇外星人
 */
UCLASS()
class FVM_API ATSubmersibleAlien : public ANormalCapsuleBase
{
	GENERATED_BODY()
public:
	//攻击卡片的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType CheckCardType = ECardCollisionType::E_CardActor2;
public:
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//更新
	virtual void MouseTick(const float& DeltaTime) override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime);
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//进入网格
	virtual void InMapMeshe(ELineType CurLineType);
	//死亡
	virtual void MouseDeathed() override;
	//动画播放完毕
	UFUNCTION()
	virtual void AnimationPlayEnd(class UTrackEntry* Track);
private:
	void PlayMove();
	void PlayAttack();
private:
	UPROPERTY()
	int32 State = -1;
	UPROPERTY()
	bool bEnbaleAttackLine = true;
	UPROPERTY()
	bool bEnanbleWaterMode = false;
};
