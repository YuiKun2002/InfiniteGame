#include "Game/UI/HeroesData.h"

FSoftObjectPath UHeroItemDataAssetCache::GetResource(const FName& Name, const FName& RowName, const int32& Key)
{
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
	//清理数据
	this->Datas.Empty();
	//初始化
	this->Data.InitDataTable(
		UGameDataSubsystem::GetGameDataSubsystemStatic()->GetAsset()->
		GetDataByName(GLOBALASSET_LEVELTEXTURE_RESOURCE)->GetDataByName(Name)
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
