// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

class UDataTable;

//数据表获取
template<class DataTableType>
class DataTableAssetData
{
public:
	DataTableAssetData(UDataTable* Data);
	DataTableAssetData() {};
public:
	//初始化数据表
	void InitDataTable(UDataTable* Data);
	//获取数据表
	const TMap<FName, DataTableType>& GetDatas();
	//获取值
	DataTableType GetDataByName(FName Name);
	void GetDataByName(FName Name, DataTableType& OutData);
	//获取Name数组
	void GetNames(TArray<FName>& Names);
	//获取值
	void GetValues(TArray<DataTableType>& Values);
public:
	//新增
	bool AddTemp(FName Name, const DataTableType& Data);
	//查询
	bool Find(FName Name);
public:
	//获取数量
	int32 Num() const;
private:
	void InitData(UDataTable* DataTable, TMap<FName, DataTableType>& OutDatas);
private:
	UPROPERTY()
	TMap<FName, DataTableType> SourceDatas;
};

template<class DataTableType>
bool DataTableAssetData<DataTableType>::Find(FName Name)
{
	return (SourceDatas.Find(Name) != nullptr);
}

template<class DataTableType>
bool DataTableAssetData<DataTableType>::AddTemp(FName Name, const DataTableType& Data)
{
	if (this->Find(Name))
	{
		return false;
	}

	this->SourceDatas.Emplace(Name, Data);
}


template<class DataTableType>
DataTableAssetData<DataTableType>::DataTableAssetData(
	UDataTable* Data
)
{
	if (IsValid(Data))
	{
		this->InitDataTable(Data);
	}
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::InitDataTable(UDataTable* Data)
{
	if (IsValid(Data))
	{
		if (!this->SourceDatas.Num())
		{
			this->InitData(Data, this->SourceDatas);
		}
	}
	else {
		SourceDatas.Empty();
	}
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::InitData(UDataTable* DataTable, TMap<FName, DataTableType>& OutDatas)
{
	OutDatas.Reserve(OutDatas.Num() + DataTable->GetRowMap().Num());
	for (
		TMap<FName, uint8*>::TConstIterator RowMapIter(DataTable->GetRowMap().CreateConstIterator());
		RowMapIter;
		++RowMapIter
		)
	{
		OutDatas.Add(
			RowMapIter.Key(),
			*(reinterpret_cast<DataTableType*>(
				RowMapIter.Value()
				)));
	}
}

template<class DataTableType>
const TMap<FName, DataTableType>& DataTableAssetData<DataTableType>::GetDatas()
{
	return this->SourceDatas;
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::GetDataByName(FName Name, DataTableType& OutData)
{
	OutData = this->GetDataByName(Name);
}

template<class DataTableType>
DataTableType DataTableAssetData<DataTableType>::GetDataByName(FName Name)
{
	DataTableType* Target = this->SourceDatas.Find(Name);
	if (Target)
	{
		return *Target;
	}

	UE_LOG(LogTemp, Error, TEXT("[%s]%s：未找到Row？"), __FUNCTION__, *Name.ToString());

	return DataTableType();
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::GetValues(TArray<DataTableType>& Values)
{
	this->SourceDatas.GenerateValueArray(Values);
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::GetNames(TArray<FName>& Names)
{
	this->SourceDatas.GenerateKeyArray(Names);
}

template<class DataTableType>
int32 DataTableAssetData<DataTableType>::Num() const
{
	return this->SourceDatas.Num();
}

