// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpineActor.h"
#include "WaterSceneComponent.generated.h"

//水面动画
UCLASS()
class FVM_API AWaterActor : public ASpineActor
{
	GENERATED_BODY()
public:
	//当初始化时
	UFUNCTION(BlueprintImplementableEvent)
	void OnInit();
	//当结束时
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnd();
	//设置渲染层级
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetRenderLayer(const int32& Layer);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMove(const FVector& Location);
};

//
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UWaterSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWaterSceneComponent();

	UFUNCTION(BlueprintCallable)
	void SpawnWater();

	void Move(const float& Tick);

	void End();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AWaterActor* Water = nullptr;
	UPROPERTY()
	AActor* OwnerActor = nullptr;
};
