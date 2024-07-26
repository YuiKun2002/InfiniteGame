// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Equipment/Gems/GemBaseStruct.h"
#include "GemFirstBase.generated.h"


//道具->宝石装备->主动技能宝石
USTRUCT(BlueprintType)
struct FGemFirst : public FWeaponGem {
	GENERATED_USTRUCT_BODY()
public:
	FGemFirst() { this->M_EWeaponGemType = EWeaponGemType::E_GemFirst; }
};

UCLASS()
class FVM_API UGemFirstBase : public UGemBaseStruct
{
	GENERATED_BODY()
	
};
