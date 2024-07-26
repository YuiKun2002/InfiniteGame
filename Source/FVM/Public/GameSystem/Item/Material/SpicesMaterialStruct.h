// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "SpicesMaterialStruct.generated.h"

//材料->香料
USTRUCT(BlueprintType)
struct FSpicesMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//可以制作的卡片等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_MakeCardGrade = 1;
};

UCLASS()
class FVM_API USpicesMaterialStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
