// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "ItemDataTable.generated.h"

/**
 *
 */

 //存储结构-索引数据表索引存储
USTRUCT(BlueprintType)
struct FItemData_Index : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品索引
	UPROPERTY(EditDefaultsOnly)
		int32 M_Index;
};

UCLASS()
class FVM_API UItemDataTable : public UObject
{
	GENERATED_BODY()
private:
	FTableRowBase* M_Value = nullptr;
public:
	//设置数据
	void SetValue(FTableRowBase* _Value);
	//获取数据
	FTableRowBase* GetValue();
};
