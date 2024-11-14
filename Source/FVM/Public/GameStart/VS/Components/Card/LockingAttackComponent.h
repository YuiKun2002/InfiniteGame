// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/ItemBaseStruct.h"
#include "GameStart/VS/Components/Card/AttackLauncherComponent.h"
#include "LockingAttackComponent.generated.h"

/**
 * 锁定攻击
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API ULockingAttackComponent : public UAttackLauncherComponent
{
	GENERATED_BODY()
public:
	//游戏开始
	virtual void BeginPlay() override;
	//游戏更新
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//加载数据
	virtual void LoadResource() override;
	//生成子弹
	virtual void SpawnBullet(AFlyItemActor* NewBullet);
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	//动画播放完成
	UFUNCTION()
	void AnimationPlayEnd(class UTrackEntry* Track);
	UFUNCTION()
	void OnPropertyChange(class UDynamicProperty* Property);
protected:
	void PlayIdleAnim();
	void PlayAttackAnim();

	//检测有效外星人
	bool CheckLineAlien(int32 Row);
	//检测外星人
	bool CheckAlien(class UMouseLineManager* AlienManagerComp);
private:
	//攻击卡片
	UPROPERTY()
	class AAttackCardActor* CardActor = nullptr;
	//外星人管理器
	UPROPERTY()
	class UMouseManagerComponent* AlienManager = nullptr;
	//检测类型
	UPROPERTY()
	TSet<ELineTraceType> CheckType;
	//当前外星人
	UPROPERTY()
	class AMouseActor* CurrentAlien = nullptr;
	//计时器
	UPROPERTY()
	float Time = 0.f;
	//攻击力提升率
	TSharedPtr<float> CardATKRate;
};
