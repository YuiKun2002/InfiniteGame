// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "VSAnimalBase.generated.h"

/**
 * 关卡里面的动物
 */

class UBoxComponent;
class AMouseActor;
class ABossBase;

UCLASS()
class FVM_API AVSAnimalBase : public AGameActorFlipbookBase
{
	GENERATED_BODY()

public:
	AVSAnimalBase();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//动画是否播放结束
	void AnimationPlayEnd();
public:

	//当动画播放完毕时
	UFUNCTION(BlueprintImplementableEvent)
		void OnAnimationPlayEnd();

	//当开始准备时
	UFUNCTION(BlueprintImplementableEvent)
		void OnReady();

	//当进入水池
	UFUNCTION(BlueprintImplementableEvent)
		void OnInWater();

	//当进入陆地
	UFUNCTION(BlueprintImplementableEvent)
		void OnGround();

	//与老鼠发生重叠时
	UFUNCTION(BlueprintImplementableEvent)
		void OnOverlapWidgtMouse(AMouseActor* MouseActor);
public:
	//当遇到老鼠时
	UFUNCTION()
		void OnMouseEnter(AMouseActor* MouseActor);
	//启动走路
	UFUNCTION(BlueprintCallable)
		void EnableWalk();
	//设置路线
	UFUNCTION()
		void SetRow(const int32& Row);
public:
	//碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent = nullptr;
public:
	UFUNCTION()
		void Begin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void End(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	//是否在走路
	UPROPERTY()
		bool bWalk = false;
	//是否在准备
	UPROPERTY()
		bool bReady = false;
	//行
	UPROPERTY()
		int32 CurRow = -1;
};
