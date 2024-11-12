// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/AttackLauncherComponent.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/Tools/DynamicProperty.h"
#include "CardAttackComponent.generated.h"

class AAttackCardActor;
class AMapMouseMesheManager;
class UMesheControllComponent;

/**
 * 卡片攻击组件【一般直线攻击卡片】小笼包，三线，双向等这种类型的基础卡片
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardAttackComponent : public UAttackLauncherComponent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void SpawnBullet(AFlyItemActor* NewBullet);
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
public:
	//资源加载
	virtual void LoadResource() override;
	//当动画播放完毕时触发
	UFUNCTION()
	void OnAnimationComplete(class UTrackEntry* Track);
	//当卡片管理器的动态属性发生变化时
	UFUNCTION()
	void OnCardManagerProChange(class UDynamicProperty* Property);
	//重新初始化Idle动画
	virtual void ReInitDefIdleAnimName(TSubclassOf<class UAssetCategoryName> IdleName) override;
	//更新
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	//攻击卡
	UPROPERTY(EditAnywhere)
	AAttackCardActor* AttackCardActor = nullptr;
private:
	//tick计时器
	float time = 0.f;
	//攻击倍率
	TSharedPtr<float> AttackRate;
	TSharedPtr<float> AttackRateAAAA;
	UPROPERTY()
	UDynamicProperty* DynamicProperty = nullptr;
};
