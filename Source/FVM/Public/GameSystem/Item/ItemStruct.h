// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/*
	装备结构
*/
// #include "GameSystem/Item/Equipment/EquipmentBaseStruct.h"
//背包结构
#include "GameSystem/Item/Equipment/BagStruct.h"
//角色时装
#include "GameSystem/Item/Equipment/PlayerEquipmentStruct.h"
//宝石结构
#include "GameSystem/Item/Equipment/GemsStruct.h"
//角色武器
#include "GameSystem/Item/Equipment/PlayerWepaonStruct.h"
//礼盒
#include "GameSystem/Item/Equipment/GiftBoxStruct.h"

/*
    卡片结构
*/
//  #include "GameSystem/Item/Card/CardBaseStruct.h"

//攻击防御卡
#include "GameSystem/Item/Card/CardAtkStruct.h"
//范围攻击卡片
#include "GameSystem/Item/Card/CardRangeAtkStruct.h"
//生产卡片
#include "GameSystem/Item/Card/CardSpawnStruct.h"
//防御卡片
#include "GameSystem/Item/Card/CardDefenceStruct.h"
//功能卡片
#include "GameSystem/Item/Card/CardFunctionStruct.h"

/*
	材料结构
*/
//  #include "GameSystem/Item/Material/MaterialBaseStruct.h"

//卡片配方
#include "GameSystem/Item/Material/CardBlueprintStruct.h"
//卡片配方材料
#include "GameSystem/Item/Material/CardBlueprintMaterialStruct.h"
//券
#include "GameSystem/Item/Material/TicketStruct.h"
//卡片转职材料
#include "GameSystem/Item/Material/CardChangeJobMaterialStruct.h"
//香料
#include "GameSystem/Item/Material/SpicesMaterialStruct.h"
//四叶草
#include "GameSystem/Item/Material/CloverMaterialStruct.h"
//技能书
#include "GameSystem/Item/Material/CardSkillBookMaterialStruct.h"
//强化水晶
#include "GameSystem/Item/Material/CrystalMaterialStruct.h"
//拆卸改锥
#include "GameSystem/Item/Material/BitMaterialStruct.h"
//关卡钥匙
#include "GameSystem/Item/Material/LevelKeyStruct.h"

#include "ItemStruct.generated.h"


//辅助类型防御卡
USTRUCT(BlueprintType)
struct FItemCardAid : public FItemCard {
	GENERATED_USTRUCT_BODY();
public:

};



UCLASS()
class FVM_API UItemStruct : public UObject
{
	GENERATED_BODY()
};
