// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/PlayerStructManager.h"
#include "PlayerEquipmentUpdate.generated.h"

/**
 * 角色装备更新类
 */
UCLASS()
class FVM_API UPlayerEquipmentUpdate : public UPlayerUpdateClass
{
	GENERATED_BODY()
public:
	virtual void Update_Implementation(class UPlayerStructManager* PlayerDataIns) override;
};
