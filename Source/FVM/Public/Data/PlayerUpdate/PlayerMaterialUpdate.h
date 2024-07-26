// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/PlayerStructManager.h"
#include "PlayerMaterialUpdate.generated.h"

/**
 *
 */
UCLASS()
class FVM_API UPlayerMaterialUpdate : public UPlayerUpdateClass
{
	GENERATED_BODY()
public:
	virtual void Update_Implementation(class UPlayerStructManager* PlayerDataIns) override;
};
