// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "TicketStruct.generated.h"

//兑换券
USTRUCT(BlueprintType)
struct FTicket : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
};

UCLASS()
class FVM_API UTicketStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
