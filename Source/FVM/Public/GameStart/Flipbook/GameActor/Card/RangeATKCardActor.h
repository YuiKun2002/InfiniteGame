// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor/ATK/ATKCardActorBase.h"
#include "RangeATKCardActor.generated.h"

#define SpineAnimationState_RangeATKCard_Idle TEXT("Idle")
#define SpineAnimationState_RangeATKCard_Attack TEXT("Attack")

/**
 *	范围攻击类卡片
 */
UCLASS()
class FVM_API ARangeATKCardActor : public AATKCardActorBase
{
	GENERATED_BODY()
public:
	//获取防御卡片数据
	const FItemCardRangeATK& GetRangeATKCardData();

public:
	//默认动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_DefAnim;
	//攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_AttackAnim;
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	//防御卡片数据
	UPROPERTY()
		FItemCardRangeATK ItemCardRangeATK;
};
