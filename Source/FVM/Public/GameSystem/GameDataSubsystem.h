// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "UObject/Interface.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameSystem/DataTableAssetData.h"
#include "GameDataSubsystem.generated.h"

/**
 * 全局资源名称
 *
 *  如果需要修改，请修改【蓝图实现BP_xxxx_CateName】【C++的UI宏名称】
 */
#define GLOBALASSET_UI FName(TEXT("GlobalAsset_UI"))
#define GLOBALASSET_PLAYER FName(TEXT("GlobalAsset_Player"))
#define GLOBALASSET_CARD FName(TEXT("GlobalAsset_Card"))
#define GLOBALASSET_MATERIAL FName(TEXT("GlobalAsset_Material"))
#define GLOBALASSET_EQUIP FName(TEXT("GlobalAsset_Equip"))

 /*
	 UI的分类名称

	 如果需要修改，请修改【数据表DT_GameUserInterface】【蓝图实现BP_xxxx_CateName】【C++的UI宏名称】
 */
#define UI_GLOBALUINAME FName(TEXT("Global"))
#define UI_WORLDMAPUINAME FName(TEXT("WorldMap"))


 //可视化名字类型分类[蓝图实现，全局的宏]
UCLASS(BlueprintType, Blueprintable)
class FVM_API UAssetCategoryName : public UObject {
	GENERATED_BODY()
public:
	//获取分类名称
	UFUNCTION(BlueprintImplementableEvent)
	FName GetCategoryName();
};

//数据资产对象
UCLASS(BlueprintType, Blueprintable)
class FVM_API UGameDataAssetObject : public UDataAsset
{
	GENERATED_BODY()
public:
	//全部数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "数据集合")
	TMap<FName, TSoftObjectPtr<UDataTable>> Datas;
public:
	UFUNCTION(BlueprintPure, Category = "获取数据表")
	UDataTable* GetDataByName(FName Name);
	//卸载
	UFUNCTION()
	void Unload();
private:
	//已经完成加载的数据表格
	UPROPERTY()
	TMap<FName, UDataTable*> DataInstance;
};

//游戏数据资产
UCLASS(BlueprintType, Blueprintable)
class FVM_API UGameDataAsset : public UDataAsset
{
	GENERATED_BODY()

	friend class UGameDataSubsystem;

public:
	//全部数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "数据集合")
	TMap<FName, TSoftObjectPtr<UGameDataAssetObject>> Datas;
public:
	UFUNCTION(BlueprintPure, Category = "获取数据资产对象")
	UGameDataAssetObject* GetDataByName(FName Name);
private:
	//已经完成加载的数据资产对象
	UPROPERTY()
	TMap<FName, UGameDataAssetObject*> DataInstance;
};

//缓存对象->持续到关卡被卸载
UCLASS(BlueprintType, Blueprintable)
class FVM_API UGameDataAssetCache : public UObject
{
	GENERATED_BODY()
public:
	//通过软类生成一个资产缓存
	UFUNCTION(BlueprintPure)
	static UGameDataAssetCache* MakeGameDataAssetCache(
		TSoftClassPtr<UGameDataAssetCache> BpGameDataAssetCache
	);
public:
	//蓝图生成缓存
	UFUNCTION(BlueprintImplementableEvent)
	UGameDataAssetCache* GenGameDataAssetCache();

	//卸载数据
	UFUNCTION(BlueprintNativeEvent)
	void Unload();
	virtual void Unload_Implementation();
};

/**
 * 游戏数据子系统
 */
UCLASS()
class FVM_API UGameDataSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	friend class UGameDataAsset;

public:
	UFUNCTION(BlueprintPure, Category = "游戏数据子系统")
	static UGameDataSubsystem* GetGameDataSubsystemStatic();

	UFUNCTION(BlueprintPure, Category = "游戏数据子系统 | 获取资产")
	UGameDataAsset* GetAsset();

	//添加缓存对象
	UFUNCTION(BlueprintCallable, Category = "游戏数据子系统 | 缓存对象")
	void AddGameDataAssetCache(FName Name, UGameDataAssetCache* CacheIns);
	//获取缓存对象
	UFUNCTION(BlueprintPure, Category = "游戏数据子系统 | 缓存对象")
	UGameDataAssetCache* GetGameDataAssetCache(FName Name);
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
private:
	//游戏数据
	UPROPERTY()
	UGameDataAsset* GameDataAsset = nullptr;
	//资产缓存
	UPROPERTY()
	TMap<FName, UGameDataAssetCache*> GameDataAssetCache;
};


//获取游戏数据资产缓存【模板】
template<class CacheClassType>
CacheClassType* GetGameDataAssetCache(FName Name)
{
	if (!IsValid(UGameDataSubsystem::GetGameDataSubsystemStatic()))
	{
		return nullptr;
	}

	UGameDataAssetCache* CurCache = UGameDataSubsystem::GetGameDataSubsystemStatic()->GetGameDataAssetCache(Name);
	if (IsValid(CurCache))
	{
		CacheClassType* CurCacheType = Cast<CacheClassType>(CurCache);
		if (IsValid(CurCacheType))
		{
			return CurCacheType;
		}
	}

	CacheClassType* CurCacheType = NewObject<CacheClassType>();
	UGameDataSubsystem::GetGameDataSubsystemStatic()->AddGameDataAssetCache(Name, CurCacheType);

	return CurCacheType;
}


/*
//数据资产对象接口
UINTERFACE(MinimalAPI, Blueprintable)
class UGameDataAssetObjectInterface : public UInterface
{
	GENERATED_BODY()
};
class FVM_API IGameDataAssetObjectInterface
{
	GENERATED_BODY()
public:
	//加载资产
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Load();
	virtual void Load_Implementation();

	//卸载资产
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Unload();
	virtual void Unload_Implementation();
};
*/