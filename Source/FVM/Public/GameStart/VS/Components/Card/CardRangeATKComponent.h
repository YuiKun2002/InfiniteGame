// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/Card/RangeATKCardActor.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "CardRangeATKComponent.generated.h"

class UPaperFlipbook;
class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardRangeATKComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
public:
	UCardRangeATKComponent();

	virtual void BeginPlay() override;
public:
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	//初始化攻击条件时间等
	virtual void InitCondition() override;
public:
	//资源加载
	virtual void LoadResource() override;
	//范围检测
	UFUNCTION(BlueprintCallable)
	bool CheckMouse(FHitResult& OutResult);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	//卡片拥有者
	UPROPERTY()
	ARangeATKCardActor* RangeATKCardActor = nullptr;
	//当前存在的老鼠
	UPROPERTY()
	TArray<AMouseActor*> M_Mouses;
	//当前存活时间
	UPROPERTY()
	float M_CurrentTime = 0.f;
	//老鼠检测延迟
	UPROPERTY()
	float CheckMouseDelay = 0.5f;
};
