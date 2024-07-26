// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CruveMoveComponent.generated.h"


//曲线移动组件
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCruveMoveComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCruveMoveComponent();

public:
	//初始化曲线数据
	UFUNCTION(BlueprintCallable)
		void Cruve_Init(FVector CurLocation, FVector TargetLocation, float CruveHight);
	//更新位置
	UFUNCTION(BlueprintCallable)
		void Curve_UpdateLocation(AActor* TargetActor, float CruveLocationTime, float CruveHightTime);
private:
	//当前坐标
	UPROPERTY()
		FVector M_CurrentLocation = FVector(0.f);
	//目前坐标
	UPROPERTY()
		FVector Target = FVector(0.f);
	//曲线最高高度
	UPROPERTY()
		float MaxHight = 5.f;
};
