// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "CardAtkStruct.generated.h"

//攻击类型防御卡
USTRUCT(BlueprintType)
struct FItemCardATK : public FItemCard {
	GENERATED_USTRUCT_BODY()
public:
	//卡片基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardATKBase = 0.f;
	//卡片攻击力加成比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardATKRate = 0.f;
	//是否有溅射伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_SputteringHurt = false;
	//溅射伤害比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SputteringATKRate = 0.f;
	//攻击速度(攻击延迟)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootSpeed = 2.f;
	//首次投射时间 {延迟}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FirstShootSpeed = 0.5f;
	//投射延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootDelay = 0.1f;
	//投射多少发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ProjectionItemCount = 1;
};

UCLASS()
class FVM_API UCardAtkStruct : public UObject
{
	GENERATED_BODY()

};
