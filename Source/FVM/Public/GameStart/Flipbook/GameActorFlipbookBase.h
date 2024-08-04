// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Paper2D/Classes/PaperFlipbookComponent.h>
#include "GameStart/Flipbook/Components/FlipbookBaseComponent.h"
#include "GameFramework/Actor.h"
#include "GameActorFlipbookBase.generated.h"


class UPaperSprite; 
class UFlipbookBaseComponent;
class UPaperFlipbookComponent;
class USceneComponent;

UCLASS()
class FVM_API AGameActorFlipbookBase : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AGameActorFlipbookBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//获取翻书对象MyActor
	UFUNCTION(BlueprintCallable)
		UFlipbookBaseComponent* GetMyActor();
	//获取当前的Scene组件
	UFUNCTION(BlueprintCallable)
		USceneComponent* GetMyPosition();
	//获取当前的统一坐标
	UFUNCTION(BlueprintCallable)
		USceneComponent* GetGameLocation();
	UFUNCTION(BlueprintCallable)
		float GetFlipbookPitchRotation();
	UFUNCTION(BlueprintCallable)
		UFlipbookBaseComponent* GetRenderComponent() const;
public:
	UFUNCTION(BlueprintCallable)
		void SetAnimationPlayEndDestroy();
	//判断该Actor是否等待被销毁
	UFUNCTION(BlueprintCallable)
		bool IsPendingKill(AActor* _Actor);
	//设置单次动画播放切换(注意直接使用SetFlipBook会受到单次动画切换的影响)
	UFUNCTION(BlueprintCallable)
		void SetPlayAnimationOnce(UPaperFlipbook* _PlayAnim, UPaperFlipbook* _PlayEndAnim);
	//设置播放的动画(设置之后会影响(单次动画播放切换->失效))
	UFUNCTION(BlueprintCallable)
		void SetPlayAnimation(UPaperFlipbook* _PlayAnim, bool _bInfluence_AnimOnce = false);
	//设置播放速度
	UFUNCTION(BlueprintCallable)
		void SetAnimationPlayRate(float _Value);
	//设置翻书的颜色
	UFUNCTION(BlueprintCallable)
		void SetFlipbookColor(FVector _Color = FVector(1.f), float _Alpha = 1.f);
	//设置翻书的旋转(会调用Location相关的设置修正)
	UFUNCTION(BlueprintCallable)
		void SetFlipbookRotation(FRotator _rotation);
	//设置翻书旋转(Y-Pitch) 平面旋转
	UFUNCTION(BlueprintCallable)
		void SetFlipbookPitchRotation(float _angle);
	//设置游戏Actor翻书对象旋转
	UFUNCTION(BlueprintCallable)
		void SetGameActorFlipbookRotation(FRotator _rotation);

	//设置渲染顺序
	UFUNCTION(BlueprintCallable)
		virtual	void SetRenderLayer(int32 _Layer);
	//获取渲染顺序
	UFUNCTION(BlueprintCallable)
		int32 GetTranslucentSortPriority() const;
public:
	//初始化旋转(直接校准)注意：初始化后X与Y的坐标会进行交换
	UFUNCTION(BlueprintCallable)
		void InitRotation();
	//播放完成
	UFUNCTION()
		void FuncDelegatePlayComplete();

	//播放结束
	UFUNCTION()
	void FuncDelegatePlayEnd();

	//动画播放完成
	UFUNCTION(BlueprintImplementableEvent)
		void OnAnimationPlayEndBlueprint();
public:
	//初始化完毕之后会自动加上这个偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AFlipbookLocationOffset")
		FVector M_OffsetPosition;
	//自定义Actor的拥有者
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AFlipbookCustomActorOwner")
		AActor* M_CustomActorOwner = nullptr;
public:
	//渲染的翻书对象
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_Actor = nullptr;
	//位置[root]
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* M_Position = nullptr;
	//所有子对象的锚点
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* M_Location = nullptr;
	//渲染顺序
	UPROPERTY()
		float CurActorTransLayer = 0;
};
