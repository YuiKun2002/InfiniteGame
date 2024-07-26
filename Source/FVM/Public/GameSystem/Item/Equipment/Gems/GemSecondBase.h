// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Equipment/Gems/GemBaseStruct.h"
#include "GemSecondBase.generated.h"

//道具->宝石装备->被动技能宝石
USTRUCT(BlueprintType)
struct FGemSecond : public FWeaponGem {
	GENERATED_USTRUCT_BODY()
public:
	FGemSecond() { this->M_EWeaponGemType = EWeaponGemType::E_GemSecond; }
};

UCLASS()
class FVM_API UGemSecondBase : public UGemBaseStruct
{
	GENERATED_BODY()
	
};
