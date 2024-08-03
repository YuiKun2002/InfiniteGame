// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpineActor.generated.h"

class USceneComponent;
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

	//获取渲染组件
	UFUNCTION(BlueprintPure)
	USpineSkeletonRendererComponent* GetRenderComponent();

	//获取动画组件
	UFUNCTION(BlueprintPure)
	USpineSkeletonAnimationComponent* GetAnimationComponent();
};
