// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "DaLiShenMissile.generated.h"

/**
 * 大力神 发射的导弹
 */
UCLASS()
class FVM_API ADaLiShenMissile : public ASpineActor
{
	GENERATED_BODY()
private:
	//最终目标点
	UPROPERTY()
		FVector M_Target;
	//当前坐标
	UPROPERTY()
		FVector M_Current;
private:
	float M_CurrentTime = 5.f;
	bool M_Result = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION(BlueprintCallable)
		void SetTarget(const FVector& _Value);
public:
	//初始化水平坐标
	UFUNCTION(BlueprintCallable)
		void InitLocation();
	//更新坐标
	UFUNCTION(BlueprintCallable)
		void UpdatePosition(float value);
	//获取结果
	UFUNCTION(BlueprintCallable)
		bool GetResult();
};
