// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/PlayerStructManager.h"
#include "PlayerSkillBooksUpdate.generated.h"

/**
 * 角色技能书更新类
 */
UCLASS()
class FVM_API UPlayerSkillBooksUpdate : public UPlayerUpdateClass
{
	GENERATED_BODY()
public:
	virtual void Update_Implementation(class UPlayerStructManager* PlayerDataIns) override;
};
