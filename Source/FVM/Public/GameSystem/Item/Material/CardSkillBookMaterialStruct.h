// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "CardSkillBookMaterialStruct.generated.h"

//材料-功能道具-技能书
USTRUCT(BlueprintType)
struct FCardSkillBookMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//经验-增加值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardSkillBookMaterial_ExAdd = 10;
	//经验书等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardSkillBookMaterial_ExLevel = 1;
};

UCLASS()
class FVM_API UCardSkillBookMaterialStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
