// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

//宝石基础类
#include "GameSystem/Item/Equipment/Gems/GemBaseStruct.h"

//宝石主动类(激光宝石类)
#include "GameSystem/Item/Equipment/Gems/First/GemLaser.h"


//宝石被动动类(攻击宝石类)
#include "GameSystem/Item/Equipment/Gems/Second/GemAttack.h"

#include "GemsStruct.generated.h"


UCLASS()
class FVM_API UGemsStruct : public UObject
{
	GENERATED_BODY()
public:
};
