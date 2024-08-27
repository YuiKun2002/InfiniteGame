// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除
// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameAssetSubsystem.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"

//void IGameDataAssetObjectInterface::Unload_Implementation() {}
//void IGameDataAssetObjectInterface::Load_Implementation() {}

//void UGameDataAssetObject::Load_Implementation()
//{
//
//}
//
//void UGameDataAssetObject::Unload_Implementation()
//{
//
//}

UDataTable* UGameDataAssetObject::GetDataByName(FName Name)
{
	UDataTable** TargetTable = this->DataInstance.Find(Name);
	if (TargetTable && IsValid(*TargetTable))
	{
		return *TargetTable;
	}

	TSoftObjectPtr<UDataTable>* TargetSource = this->Datas.Find(Name);
	if (TargetSource)
	{
		UDataTable* NewData = TargetSource->LoadSynchronous();
		if (IsValid(NewData))
		{
			this->DataInstance.Emplace(Name, NewData);

			return NewData;
		}
	}

	return nullptr;
}

void UGameDataAssetObject::Unload()
{
	for (auto& Instance : this->DataInstance)
	{
		if (IsValid(Instance.Value))
		{
			Instance.Value = nullptr;
		}
	}

	this->DataInstance.Empty();
}

UGameDataAssetObject* UGameDataAsset::GetDataByName(FName Name)
{
	UGameDataAssetObject** TargetTable = this->DataInstance.Find(Name);
	if (TargetTable && IsValid(*TargetTable))
	{
		return *TargetTable;
	}

	TSoftObjectPtr<UGameDataAssetObject>* TargetSource = this->Datas.Find(Name);
	if (TargetSource)
	{
		UGameDataAssetObject* NewData = TargetSource->LoadSynchronous();
		if (IsValid(NewData))
		{
			this->DataInstance.Emplace(Name, NewData);
			return NewData;
		}
	}

	return nullptr;
}

UGameDataAssetCache* UGameDataAssetCache::MakeGameDataAssetCache(TSoftClassPtr<UGameDataAssetCache> BpGameDataAssetCache)
{
	UClass* TargetClass = BpGameDataAssetCache.LoadSynchronous();
	if (IsValid(TargetClass))
	{
		TSubclassOf<UGameDataAssetCache> SubClass(TargetClass);
		UGameDataAssetCache* CurrentCache = SubClass.GetDefaultObject();
		if (IsValid(CurrentCache))
		{
			return CurrentCache->GenGameDataAssetCache();
		}
	}

	return nullptr;
}

void UGameDataAssetCache::Unload_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("[UGameDataAssetCache]：调用了C++的默认资产缓存卸载函数"))
}

UGameDataSubsystem* UGameDataSubsystem::GetGameDataSubsystemStatic()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		if (IsValid(UFVMGameInstance::GetFVMGameInstance()->GetWorld()))
		{
			return UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetSubsystem<UGameDataSubsystem>();
		}
	}

	return nullptr;
}

UGameDataAsset* UGameDataSubsystem::GetAsset()
{
	return this->GameDataAsset;
}

void UGameDataSubsystem::AddGameDataAssetCache(FName Name, UGameDataAssetCache* CacheIns)
{
	UGameDataAssetCache** TargetIns = this->GameDataAssetCache.Find(Name);
	if (!TargetIns)
	{
		this->GameDataAssetCache.Emplace(Name, CacheIns);
	}
}

UGameDataAssetCache* UGameDataSubsystem::GetGameDataAssetCacheByDef(FName Name)
{
	UGameDataAssetCache** TargetIns = this->GameDataAssetCache.Find(Name);
	if (TargetIns && IsValid(*TargetIns))
	{
		return *TargetIns;
	}

	return nullptr;
}

UGameDataAssetCache* UGameDataSubsystem::GetGameDataAssetCacheByName(TSubclassOf<UAssetCategoryName> Name)
{
	return GetGameDataAssetCache<UGameDataAssetCache>(Name.GetDefaultObject()->GetCategoryName());
}

void UGameDataSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	//初始化游戏数据
	if (IsValid(UGameAssetSubsystem::GetGameAssetSubsystemStatic()))
	{
		this->GameDataAsset =
			Cast<UGameDataAsset>(
				UGameAssetSubsystem::GetGameAssetSubsystemStatic()->GetMainGameAsset()->
				GameData_DataTable.LoadSynchronous()
			);
	}

	if (IsValid(this->GameDataAsset))
	{
		//初始化UI子系统
		if (IsValid(UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic()))
		{
			UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic()->InitializeUserInterData(
				this->GameDataAsset
			);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("游戏数据子系统，数据已经加载，UWorld启动"))
}

void UGameDataSubsystem::Deinitialize()
{
	//卸载UI子系统数据
	if (IsValid(UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic()))
	{
		UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic()->UnloadAllResource();
	}

	if (IsValid(this->GameDataAsset))
	{
		for (auto& Data : this->GameDataAsset->DataInstance)
		{
			if (IsValid(Data.Value))
			{
				Data.Value->Unload();
			}
		}

		this->GameDataAsset->DataInstance.Empty();
	}


	for (auto& Data : this->GameDataAssetCache)
	{
		if (IsValid(Data.Value))
		{
			Data.Value->Unload();
			Data.Value = nullptr;
		}
	}
	this->GameDataAssetCache.Empty();

	UE_LOG(LogTemp, Warning, TEXT("游戏数据子系统，数据已经被卸载，UWorld卸载"))
}
