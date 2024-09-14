// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/GameDataSubsystem.h"
#include "HeroesData.generated.h"

#define HEROITEM_HEROITEM FName(TEXT("HeroItem"))
#define HEROITEM_ITEMLEVEL FName(TEXT("ItemLevel"))

//英雄物品缓存对象
UCLASS()
class FVM_API UHeroItemDataAssetCache : public UGameDataAssetCache
{
	GENERATED_BODY()
public:
	FSoftObjectPath GetResource(const FName& Name, const FName& RowName, const int32& Key);
private:
	//图片数据
	DataTableAssetData<FItemResourceData> Data;
	UPROPERTY()
	TMap<int32, FSoftObjectPath> Datas;
	//名称
	UPROPERTY()
	FName ResourceName;
};