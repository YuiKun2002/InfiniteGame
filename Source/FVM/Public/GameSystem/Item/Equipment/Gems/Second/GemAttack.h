// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Equipment/Gems/GemSecondBase.h"
#include "GemAttack.generated.h"

//道具->宝石装备->被动技能宝石->攻击宝石类
USTRUCT(BlueprintType)
struct FGemAttackType : public FGemSecond {
	GENERATED_USTRUCT_BODY()
public:
	FGemAttackType() { this->M_GemTag = TEXT("Attack"); }
public:
	//额外增加的基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_UpAttackBase = 4;
	//每次升级加成伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_UpGradeAttack = 1;
	//每次升级几次加成一次
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_UpGradeCount = 2;
	//基础加成倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_UpAttackRate = 0.2f;
};


//装备-玩家宝石数据->攻击宝石数据
USTRUCT(BlueprintType)
struct FEquipment_GemAttack_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FGemAttackType M_FEquipment;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FEquipment;
		return true;
	}
};

UCLASS()
class FVM_API UGemAttack : public UGemSecondBase
{
	GENERATED_BODY()
public:
	//路径
	static FString DataTablePath;
};
