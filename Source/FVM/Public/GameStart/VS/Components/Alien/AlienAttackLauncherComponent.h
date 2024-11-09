// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/AttackLauncherComponent.h"
#include "AlienAttackLauncherComponent.generated.h"

/**
 * 外星人攻击发射器组件
 */
UCLASS()
class FVM_API UAlienAttackLauncherComponent : public UAttackLauncherComponent
{
	GENERATED_BODY()
public:
	// Called when the game starts
	virtual void BeginPlay() override;
	//tick
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//生成子弹
	virtual void SpawnBullet(AFlyItemActor* NewBullet);
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	//资源加载
	virtual void LoadResource() override;
	//当动画播放完毕时触发【通过外星人动画结束调用】
	void OnAnimationComplete(class UTrackEntry* Track);
private:
	UPROPERTY()
	class AGunAlien* GunAlien = nullptr;
	UPROPERTY()
	class AGunAlienWidget* GunAlienWidegt = nullptr;
};
