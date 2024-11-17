// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "CarAlienWidget.generated.h"

/**
 * 车类型的外星人界面
 */
UCLASS()
class FVM_API ACarAlienWidget : public ANormalCapsuleByWidgetBase
{
	GENERATED_BODY()
public:
	public:
	//检测卡片类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType CheckCardType = ECardCollisionType::E_CardActor2;
	//左检测点位
	UPROPERTY(VisibaleAnywehre, BlueprintReadWrite)
	USceneComponent* CheckWidLeftLocationComp = nullptr;
	//右检测点位
	UPROPERTY(VisibaleAnywehre, BlueprintReadWrite)
	USceneComponent* CheckWidRightLocationComp = nullptr;
public:
	ACarAlienWidget();

	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//受到攻击
	virtual void BeAttakedBegin() override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
private:
	void CheckCard();
	void PlayMoveAnim();
private:
	UPROPERTY()
	int32 State = -1;
	UPROPERTY()
	float Time = 0.f;
};