// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "CardBlueprintMaterialStruct.generated.h"

//材料->卡片配方材料
USTRUCT(BlueprintType)
struct FCardBlueprintMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
};


UCLASS()
class FVM_API UCardBlueprintMaterialStruct : public UObject
{
	GENERATED_BODY()
public:
	//数据表路径
	static FString DataTablePath;
};
