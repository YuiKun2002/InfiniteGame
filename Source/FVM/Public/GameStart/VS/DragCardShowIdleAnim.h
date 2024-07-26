// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragCardShowIdleAnim.generated.h"

class USceneCaptureComponent2D;
class UPaperFlipbookComponent;
class UPaperFlipbook;

UCLASS()
class FVM_API ADragCardShowIdleAnim : public AActor
{
	GENERATED_BODY()
public:
	//2D场景捕捉
	UPROPERTY(EditAnywhere)
		USceneCaptureComponent2D* M_Capture2D = nullptr;
	//动画显示组件
	UPROPERTY(EditAnywhere)
		UPaperFlipbookComponent* M_AnimComponent = nullptr;
public:
	// Sets default values for this actor's properties
	ADragCardShowIdleAnim();

public:
	//设置播放动画
	UFUNCTION(BlueprintCallable)
		void SetAnim(UPaperFlipbook* _anim);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
