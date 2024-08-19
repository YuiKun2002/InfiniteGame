// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "CardBlueprintStruct.generated.h"

//材料->卡片配方
USTRUCT(BlueprintType)
struct FCardBlueprint : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//需要的材料道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> M_Materials = {};
	//合成的卡片类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardType M_SynthesisType = ECardType::E_ATK;
	//合成的卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_SynthesisCardName;
};

UCLASS()
class FVM_API UCardBlueprintStruct : public UObject
{
	GENERATED_BODY()
public:
	//数据表路径
	static FString DataTablePath;
};
