// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/ItemBaseStruct.h"


//全局静态流加载实例句柄
//static TSharedPtr<FStreamableHandle> UItemBaseStruct::M_Globle_StreamableHandle;
//全局静态物品基础对象
//static UItemBaseStruct* UItemBaseStruct::M_Globle_UItemBaseStruct_Ptr = nullptr;

bool UItemBaseStruct::GetTextureResource(int32 ID, FSoftObjectPath& SoftObjectPath)
{
	UItemBaseDataAssetCache* Cache = GetGameDataAssetCache<UItemBaseDataAssetCache>(GLOBALASSET_TEXTURE_RESOURCE);
	FItemResourceBase Data;
	Cache->GetTextureResource(ID, Data);
	FSoftObjectPath* TargetRes = Data.Resources.Find(ID);
	if (TargetRes)
	{
		SoftObjectPath = *TargetRes;

		return true;
	}
	else {
		SoftObjectPath.Reset();
	}

	return false;
}

void UItemBaseDataAssetCache::GetResource(int32 ID, TSet<int32>& Ranges, TArray<FItemResourceData>& Datas, FItemResourceBase& Data)
{
	//区域Index
	int32 Range = int32(ID / 200);
	//查询区域Index
	int32* Index = Ranges.Find(Range);
	if (Index)
	{
		Data = Datas[*Index].Resource;
		return;
	}
	else {
		//添加区域
		Ranges.Emplace(Range);
		//加载数据
		TArray<FItemResourceData> RangeDataDatas;
		TArray<FItemResourceData>& RangeData =
			GetDataTableSourceData(RangeDataDatas,
				this->SourceDatas,
				GLOBALASSET_TEXTURE_RESOURCE,
				FName(FString::FromInt(Range))
			);
		if (RangeDataDatas.Num())
		{
			//添加区域
			Datas.Emplace(RangeDataDatas[0]);
			//返回数据
			Data = RangeDataDatas[0].Resource;
		}
		else {
			//如果没有写，则添加空元素
			FItemResourceData Temp = {};
			Datas.Emplace(Temp);
			Data = Temp.Resource;
		}
	}
}

void UItemBaseDataAssetCache::GetTextureResource(int32 ID, FItemResourceBase& Data)
{
	this->GetResource(ID, this->TextureRanges, this->TextureDatas, Data);
}
