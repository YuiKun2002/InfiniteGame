// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameFramework/Actor.h"
#include "UI_VSCountDown.generated.h"

/**
 *  游戏倒计时
 */

class UTextBlock;


UCLASS()
class FVM_API AVSCountDown : public AActor
{
	GENERATED_BODY()
public:
	AVSCountDown();
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY()
		class UUI_VSCountDown* UI = nullptr;
};

UCLASS()
class FVM_API UUI_VSCountDown : public UWidgetBase
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

public:
	//开启计时
	void Enable(const float& _time);

	//关闭计时
	void DisEnable();


	//初始化
	UFUNCTION(BlueprintCallable)
		void Init();

public:

	void Update(const float& _Tick);

	void UpdateText();

private:

	//时间倒计时文本
	UPROPERTY()
		UTextBlock* TimeText = nullptr;

public:

	//是否开启计时
	UPROPERTY()
		bool bEnable = false;

	//时间
	UPROPERTY()
		int32 Times = 100;

	//当前时间
	UPROPERTY()
		float CurTime = 0.f;
};
