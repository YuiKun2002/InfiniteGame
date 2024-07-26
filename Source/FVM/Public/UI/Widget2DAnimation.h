// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Widget2DAnimation.generated.h"

class UImage;
class UPaperFlipbook;

/**
 * 图形界面动画
 */
UCLASS(BlueprintType)
class FVM_API UWidget2DAnimation : public UObject
{
	GENERATED_BODY()
public:
	//创建一个2D动画对象
	UFUNCTION(BlueprintCallable)
		static UWidget2DAnimation* MakeWidget2DAnimation(TSoftObjectPtr<UPaperFlipbook> FlipResource, UImage* ShowImage);
	//更新动画
	UFUNCTION(BlueprintCallable)
		void Update(const float& time);
private:
	//资产
	UPROPERTY()
		TSoftObjectPtr<UPaperFlipbook> NewFlipbook;
	//播放的动画
	UPROPERTY()
		UPaperFlipbook* FlipAnim = nullptr;
	//显示的图片
	UPROPERTY()
		UImage* ShowView = nullptr;
	//当前时间
	UPROPERTY()
		float TimeNow = 0.f;
};
