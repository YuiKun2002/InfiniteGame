// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/MapBaseType.h"
#include "ShootLineComponent.generated.h"

class AGameActorFlipbookBase;

//节点
USTRUCT(BlueprintType)
struct FTargetNode {
	GENERATED_USTRUCT_BODY()
public:
	//移动方向
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EShootDirection M_EShootDirection;
	//移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_Speed = 4.f;
	//高度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_Hight = 0.f;
	//当前高度
	UPROPERTY()
	float M_CurrentHight = 0.f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UShootLineComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	bool M_bMove = true;
	//增速度
	float M_Speed = 65.f;
public:
	//射击移动点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTargetNode M_MoveNode;
	//节点位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_NodePosition = 0;
	//目标点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_Target = 0;
	//拥有者
	UPROPERTY(EditInstanceOnly)
	class ASpineActor* M_Owner = nullptr;
public:
	// Sets default values for this component's properties
	UShootLineComponent();

	//结束射击
	UFUNCTION(BlueprintCallable)
	void Over();

	//移动
	UFUNCTION(BlueprintCallable)
	void Move(const float& _Speed, const float& _MoveHight, float& _CurrentMoveHight);

	//停止移动
	UFUNCTION(BlueprintCallable)
	void SetMoveEnabled(bool _bMove);

	//设置移动模式
	UFUNCTION(BlueprintCallable)
	void SetTargetNode(FTargetNode _FTargetNode);

	//获取移动方向
	UFUNCTION(BlueprintCallable)
	const EShootDirection& GetMoveDirection();
	UFUNCTION(BlueprintCallable)
	void SetMoveDirection(const EShootDirection& _Direction);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
