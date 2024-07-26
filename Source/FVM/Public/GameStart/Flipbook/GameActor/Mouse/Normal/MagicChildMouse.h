// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "MagicChildMouse.generated.h"

/**
 *
 */

class UPaperFlipbook;
class UBoxComponent;
class USphereComponent;

UCLASS()
class FVM_API AMagicChildMouse : public ANormalBase
{
	GENERATED_BODY()

public:
	AMagicChildMouse();

	//当初始化时
	UFUNCTION(BlueprintImplementableEvent)
		void OnInitBlue();
	//飞行完毕
	UFUNCTION(BlueprintCallable)
		void OnFlyEnd();
	//禁止攻击
	UFUNCTION(BlueprintCallable)
		void DisEnableATK();

	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//移动
	virtual void MoveingUpdate(float DeltaTime) override;
	//攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//老鼠死亡
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimPlayEnd();

public:
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComp = nullptr;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType CardCollisionType = ECardCollisionType::E_CardActor2;

	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 行走")
		TSoftObjectPtr<UPaperFlipbook> Walk;
	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 攻击")
		TSoftObjectPtr<UPaperFlipbook> Attack;
	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 死亡")
		TSoftObjectPtr<UPaperFlipbook> Death;

private:
	bool bFlyMove = false;
	bool bAtk = false;
};
