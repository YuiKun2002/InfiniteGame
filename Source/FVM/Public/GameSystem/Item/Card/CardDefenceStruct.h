// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "GameSystem/Item/Card/CardRangeAtkStruct.h"
#include "CardDefenceStruct.generated.h"

//防御类型防御卡
USTRUCT(BlueprintType)
struct FItemCardDefence : public FItemCard {
	GENERATED_USTRUCT_BODY();
public:
	//防御力加成比例(基于生命值加成)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_DefenceUpRate = 0.034f;
	//是否开启反伤(ATK * ATKRate * HP * 0.1f / 2.f)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableReturnATK = false;
	//反伤
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK = 0.f;
	//反伤加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK_UpRate = 0.f;
	//开启死亡爆炸功能
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableDeathBomb = false;
	//半径(卡片攻击范围)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATKRadius = 45.f;
	//伤害(剩余血量百分比)[默认返还剩余血量的2%作为伤害]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_LifeToATK_Rate = 0.2f;
	//死亡造成的伤害携带的额外的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItem_Buff M_Buffs;
	//检测类型(决定当前卡片可以攻击的老鼠类型)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMouseCollisionType M_ELineTraceType;
	//开启死亡爆炸动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableDeathBombAnim = false;
	//死亡爆炸翻书动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_DeathBombAnimFlipbookPath;
	//死亡爆炸动画的偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_AnimOffset;
	//开启死亡爆炸音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCardRangeATK_Audio M_FCardRangeATK_Audio;
};

UCLASS()
class FVM_API UCardDefenceStruct : public UObject
{
	GENERATED_BODY()

};
