// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include"GameSystem/Item/Equipment/EquipmentBaseStruct.h"
#include "BagStruct.generated.h"


//道具->装备背包
USTRUCT(BlueprintType)
struct FBag : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	//背包容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ContentCount = 0;
	//表示背包在序列中的别名->这个是用于判定玩家装备背包的标准【将道具添加到背包之后要判断当前名称是否存在，确保唯一性】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_VectorName = "";
};

//装备-背包数据
USTRUCT(BlueprintType)
struct FEquipment_Bag_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FBag M_FEquipment;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FEquipment;
		return true;
	}
};

UCLASS()
class FVM_API UBagStruct : public UObject
{
	GENERATED_BODY()
public:
	static FString DataTablePath;
};
