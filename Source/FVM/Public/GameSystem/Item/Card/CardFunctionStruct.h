// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameStart/VS/MapBaseType.h"
#include "CardFunctionStruct.generated.h"

//功能类型防御卡
USTRUCT(BlueprintType)
struct FItemCardFunction : public FItemCard {
	GENERATED_USTRUCT_BODY();
public:
	//卡片的功能集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<TSoftClassPtr<class UCardFunctionBase>> Functions;
};

UCLASS()
class FVM_API UCardFunctionStruct : public UObject
{
	GENERATED_BODY()

};