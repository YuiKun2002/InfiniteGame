// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "FlipbookBaseComponent.generated.h"

/**
 *
 */

class UPaperFlipbook;
class AGameActorFlipbookBase;

DECLARE_DELEGATE(FAnimationEndDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicAnimationEndDelegate);
DECLARE_DELEGATE(FAnimationCompleteDelegate);

UCLASS(ShowCategories = (Mobility, ComponentReplication), ClassGroup = Paper2D, meta = (BlueprintSpawnableComponent))
class FVM_API UFlipbookBaseComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()
public:
	//动画结束代理
	FAnimationEndDelegate OnAnimationPlayEnd;
	//动画结束代理(动态)
	UPROPERTY(BlueprintAssignable)
		FDynamicAnimationEndDelegate OnDynamicAnimationEndDelegate;
	//动画播放完成
	FAnimationCompleteDelegate OnAnimationComplete;
public:
	//设置单次动画播放切换
	UFUNCTION(BlueprintCallable)
		void SetPlayAnimationOnce(UPaperFlipbook* _PlayAnim, UPaperFlipbook* _PlayEndAnim);
	//设置播放的动画
	UFUNCTION(BlueprintCallable)
		void SetPlayAnimation(UPaperFlipbook* _PlayAnim, bool _bInfluence_AnimOnce = false);
	//设置渲染层
	UFUNCTION(BlueprintCallable)
		void SetTranslucency(const int32& _layer);
	//获取渲染层
	UFUNCTION(BlueprintCallable)
		int32 GetTranslucency();
	//设置播放结束销毁
	void SetAnimationPlayEndDestroy();
	void CancelAnimationPlayEndDestroy();
public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//是否播放一次进行切换
	UPROPERTY()
		bool _bPlayOnce = false;
	//动画播放结束后销毁对象
	UPROPERTY()
		bool IsPlayEndOnDestroy = false;
	//渲染顺序
	UPROPERTY()
		int32 _Layer = 0;
private:
	// buffer ->动画切换缓冲
	UPROPERTY()
		UPaperFlipbook* M_UPaperFlipbook_PlayEnd = nullptr;
private:
	//动画结束
	bool PlayAnimationEnd(const float& DeltaTime);
};
