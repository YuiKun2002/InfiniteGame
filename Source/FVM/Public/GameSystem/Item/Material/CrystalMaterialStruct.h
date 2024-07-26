// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "CrystalMaterialStruct.generated.h"

//材料->强化水晶
USTRUCT(BlueprintType)
struct FCrystalMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//最小使用等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_UseGradeMin = 0;
	//最大使用等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_UseGradeMax = 10;
};

UCLASS()
class FVM_API UCrystalMaterialStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
