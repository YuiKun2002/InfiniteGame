// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "SpawnCardActor.generated.h"

/**
 * 生产类卡片
 */
UCLASS()
class FVM_API ASpawnCardActor : public ACardActor
{
	GENERATED_BODY()

public:
	//默认动画1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSubclassOf<UAssetCategoryName> Idle;
	//生产动画1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSubclassOf<UAssetCategoryName> Attack;
public:

	//获取生产卡片数据
	const FItemCardSpawn& GetSpawnCardData();

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	//生产卡片数据
	UPROPERTY()
		FItemCardSpawn ItemCardSpawnData;

};
