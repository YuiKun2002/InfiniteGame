#include "Game/UI/HeroesData.h"

FSoftObjectPath UHeroItemDataAssetCache::GetResource(const FName& Name, const FName& RowName, const int32& Key)
{
	//如果当前缓存名称与新名称相同【数据表中的RowName】
	if (this->ResourceName.IsEqual(Name))
	{
		if (this->Datas.Num())
		{
			//查询
			FSoftObjectPath* TargetData = Datas.Find(Key);
			if (TargetData)
			{
				return *TargetData;
			}
			return FSoftObjectPath();
		}
	}

	//如果是不同的RowName则重新初始化
	this->Datas.Empty();
	//初始化【查询全局纹理等级资源】
	this->Data.InitDataTable(
		UGameDataSubsystem::GetGameDataSubsystemStatic()->GetAsset()->
		GetDataByName(GLOBALASSET_LEVELTEXTURE_RESOURCE)->GetDataByName(Name) //获取Row
	);
	//获取行
	FItemResourceData RowData = this->Data.GetDataByName(RowName);
	//释放
	this->Data.InitDataTable(nullptr);
	//填充数据
	Datas.Append(RowData.Resource.Resources);
	//查询
	FSoftObjectPath* TargetData = Datas.Find(Key);
	if (TargetData)
	{
		return *TargetData;
	}
	return FSoftObjectPath();
}
