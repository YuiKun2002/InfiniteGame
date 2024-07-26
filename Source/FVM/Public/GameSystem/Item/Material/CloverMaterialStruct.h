// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "CloverMaterialStruct.generated.h"

//材料->四叶草
USTRUCT(BlueprintType)
struct FCloverMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//可以帮助概率提升x倍
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_UpGrateRate = 1.35f;
};

UCLASS()
class FVM_API UCloverMaterialStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
