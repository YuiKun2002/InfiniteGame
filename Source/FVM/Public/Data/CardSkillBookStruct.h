// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "CardSkillBookStruct.generated.h"


 //存储结构-技能书
USTRUCT(BlueprintType)
struct FSkillBookData_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FSkillBookData M_FSkillBookData;
};



UCLASS()
class FVM_API UCardSkillBookStruct : public UObject
{
	GENERATED_BODY()
};
