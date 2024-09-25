// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "DefenceCardActor.generated.h"

/**
 *防御类的防御卡
 */
UCLASS()
class FVM_API ADefenceCardActor : public ACardActor
{
	GENERATED_BODY()
public:
	//获取防御卡片数据
	const FItemCardDefence& GetDefenceCardData();

public:
	//默认动画1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSubclassOf<UAssetCategoryName> CardActor_DefAnim1;
	//半血动画2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSubclassOf<UAssetCategoryName> CardActor_DefAnim2;
	//残血动画3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSubclassOf<UAssetCategoryName> CardActor_DefAnim3;
	//死亡动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSubclassOf<UAssetCategoryName> CardActor_DeathAnim;
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	//防御卡片数据
	UPROPERTY()
		FItemCardDefence ItemCardDefence;
};
