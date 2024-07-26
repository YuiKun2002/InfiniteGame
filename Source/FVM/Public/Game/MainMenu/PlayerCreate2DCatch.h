// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCreate2DCatch.generated.h"

class UPaperFlipbook;
class UFlipbookBaseComponent;

UCLASS()
class FVM_API APlayerCreate2DCatch : public AActor
{
	GENERATED_BODY()

public:
	//翻书对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_UFlipbookBaseComponent = nullptr;
	//坐标组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	   USceneComponent* M_LocationComp = nullptr;

public:
	// Sets default values for this actor's properties
	APlayerCreate2DCatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
