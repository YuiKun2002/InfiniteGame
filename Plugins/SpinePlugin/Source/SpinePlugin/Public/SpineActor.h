// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpineActor.generated.h"

//#include "SpineSkeletonRendererComponent.h"
//#include "SpineSkeletonAnimationComponent.h"

class USceneComponent;
class UTrackEntry;
class USpineSkeletonRendererComponent;
class USpineSkeletonAnimationComponent;

UCLASS()
class SPINEPLUGIN_API ASpineActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpineActor();

protected:

	//Root组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp = nullptr;

	//位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PointComp = nullptr;

	//动画渲染组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpineSkeletonRendererComponent* AnimRenderComp = nullptr;

	//动画组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpineSkeletonAnimationComponent* AnimComp = nullptr;

public:

	/************************************************************************/
	/*							获取组件                                     */
	/************************************************************************/
	//获取场景位置组件
	UFUNCTION(BlueprintPure)
	USceneComponent* GetPointComponent();
	//获取渲染组件
	UFUNCTION(BlueprintPure)
	USpineSkeletonRendererComponent* GetRenderComponent();
	//获取动画组件
	UFUNCTION(BlueprintPure)
	USpineSkeletonAnimationComponent* GetAnimationComponent();


	//获取当前正在播放的动画名称
	UFUNCTION(BlueprintPure)
	FString GetCurrentAnimationName(int32 TrackIndex) const;

public:
	//播放动画
	UFUNCTION(BlueprintCallable)
	UTrackEntry* SetAnimation(int32 TrackIndex, FString AnimationName, bool Loop = true);
	UFUNCTION(BlueprintCallable)
	UTrackEntry* AddAnimation(int32 TrackIndex, FString AnimationName, bool Loop, float Delay);
	//设置动画播放状态
	UFUNCTION(BlueprintCallable)
	void SetAnimationTimeScale(int32 TrackIndex, float Value);
	//清除动画播放
	/*
	@Param1 值为-1时全部清除，非-1时清除指定轨道
	*/
	UFUNCTION(BlueprintCallable)
	void SetAnimationClear(int32 TrackIndex = -1);
public:
	//设置渲染的颜色
	UFUNCTION(BlueprintCallable)
	void SetRenderColor(FLinearColor SpineColor);

public:
	//设置渲染层级
	virtual	void SetRenderLayer(int32 RenderLayer);
};
