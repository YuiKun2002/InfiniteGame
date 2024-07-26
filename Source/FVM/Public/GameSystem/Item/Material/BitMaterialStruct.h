// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "BitMaterialStruct.generated.h"

//材料->武器宝石开槽-钻头
USTRUCT(BlueprintType)
struct FBitMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
};

UCLASS()
class FVM_API UBitMaterialStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
