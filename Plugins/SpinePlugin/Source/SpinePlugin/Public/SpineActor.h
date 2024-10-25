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

/*
UCLASS()
class SPINEPLUGIN_API USpineTrackEntryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//设置动画轨道
	UFUNCTION(BlueprintCallable)
	UTrackEntry* SetTrackEntry(int32 TrackIndex, UTrackEntry* TrackInstance);
public:
	//获取动画轨道
	UFUNCTION(BlueprintPure)
	UTrackEntry* GetTrackEntry(int32 TrackIndex);
	//获取动画轨道是否有效
	UFUNCTION(BlueprintPure)
	bool GetTrackIsValid(int32 TrackIndex) const;
private:
	//目前已经存在的轨道
	UPROPERTY()
	TMap<int32, UTrackEntry*> Tracks;
};*/

/*
Spine2D动画Actor
*/
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
	//获取当前轨道
	UFUNCTION(BlueprintPure)
	UTrackEntry* GetCurrentAnimationTrackEntry(int32 TrackIndex) const;
	//获取旋转
	UFUNCTION(BlueprintPure)
	FRotator GetRotation() const;
	//获取颜色
	UFUNCTION(BlueprintPure)
	FLinearColor GetSpineRenderColor() const;
	//获取渲染层级
	UFUNCTION(BlueprintPure)
	int32 GetSpineRenderLayer();
	virtual int32 GetRenderLayer();
public:
	//设置动画
	UFUNCTION(BlueprintCallable)
	UTrackEntry* SetAnimation(int32 TrackIndex, FString AnimationName, bool Loop = true);
	//设置动画
	UTrackEntry* SetAnimation(
		UTrackEntry*& CurTrack,
		int32 TrackIndex,
		const FName& AnimName
	);
	//添加动画
	UFUNCTION(BlueprintCallable)
	UTrackEntry* AddAnimation(int32 TrackIndex, FString AnimationName, bool Loop, float Delay);
	//设置空动画
	UFUNCTION(BlueprintCallable)
	UTrackEntry* SetEmptyAnimation(int32 TrackIndex, float MixDuration);
	//添加空动画
	UFUNCTION(BlueprintCallable)
	UTrackEntry* AddEmptyAnimation(int32 TrackIndex, float MixDuration, float Delay);
	//设置动画播放状态
	UFUNCTION(BlueprintCallable)
	void SetAnimationTimeScale(int32 TrackIndex, float Value);
	//清除动画播放
	/*
	@Param1 值为-1时全部清除，非-1时清除指定轨道
	*/
	UFUNCTION(BlueprintCallable)
	void SetAnimationClear(int32 TrackIndex = -1);
	//设置渲染的颜色
	UFUNCTION(BlueprintCallable)
	void SetSpineRenderColor(FLinearColor SpineColor);
	//设置渲染层级
	UFUNCTION(BlueprintCallable)
	void SetSpineRenderLayer(int32 RenderLayer);
	virtual	void SetRenderLayer(int32 RenderLayer);
	//初始化Spine显示
	UFUNCTION(BlueprintCallable)
	void InitSpineShow();
	//旋转Spine
	UFUNCTION(BlueprintCallable)
	void SetRotation(float Angle);

private:
	//渲染层
	UPROPERTY()
	int32 mRenderLayer = 0;
};
