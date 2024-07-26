// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "CardSpawnStruct.generated.h"

//生产类型防御卡
USTRUCT(BlueprintType)
struct FItemCardSpawn : public FItemCard {
	GENERATED_USTRUCT_BODY()
public:
	//每一个火苗初始量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_FlameNum = 25;
	//一次生产几个火苗
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_SpawnNum = 1;
	//生产速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnSpeed = 15.f;
	//首次生产延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FirstSpawnDelay = 0.f;
	//多次生产延迟间隔时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnDelay = 0.4f;
	//生产速度提升比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnSpeedRate = 0.034f;
	//是否开启周期生长
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_EnableWeekUp = false;
	//生长的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_WeekUpTime = 60.f;
	//生产火苗数量提升比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnFlameNumRate = 2.f;
};

UCLASS()
class FVM_API UCardSpawnStruct : public UObject
{
	GENERATED_BODY()
	
};
