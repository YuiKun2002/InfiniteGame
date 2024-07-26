// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "PlayerDataSubsystem.generated.h"

/**
 * 角色存档子系统
 */

class USaveGame;
class UPlayerStructManager;


/**
 *角色数据接口
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerDataInterface : public UInterface
{
	GENERATED_BODY()
};

class FVM_API IPlayerDataInterface
{
	GENERATED_BODY()
public:

	/*

		Base

	*/

	//保存角色数据接口
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool SavePlayerData(const FString& SaveLogMsg);
	virtual bool SavePlayerData_Implementation(const FString& SaveLogMsg);

	//加载角色数据接口
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool LoadPlayerData(const FString& LoadLogMsg);
	virtual bool LoadPlayerData_Implementation(const FString& LoadLogMsg);

	//创建角色数据接口
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool CreateNewPlayerData();
	virtual bool CreateNewPlayerData_Implementation();

	//移除角色数据接口
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool RemovePlayerData();
	virtual bool RemovePlayerData_Implementation();

	//获取角色数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UPlayerStructManager* GetPlayerData();
	virtual UPlayerStructManager* GetPlayerData_Implementation();

public:


};


/*
本地角色数据对象句柄
*/
UCLASS(BlueprintType)
class FVM_API ULocalPlayerDataHandle : public UObject, public IPlayerDataInterface
{
	GENERATED_BODY()
public:
	//构造本地角色数据对象
	static ULocalPlayerDataHandle* MakeLocalPlayerDataHandle(
		FString PlayerName,
		int32 PlayerSex
	);
public:

	//保存本地存档
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool SavePlayerData(const FString& SaveLogMsg);
	virtual bool SavePlayerData_Implementation(const FString& SaveLogMsg) override;

	//加载本地存档
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool LoadPlayerData(const FString& LoadLogMsg);
	virtual bool LoadPlayerData_Implementation(const FString& LoadLogMsg) override;

	//创建新的角色数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool CreateNewPlayerData();
	virtual bool CreateNewPlayerData_Implementation() override;

	//移除角色数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool RemovePlayerData();
	virtual bool RemovePlayerData_Implementation() override;

	//获取角色数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UPlayerStructManager* GetPlayerData();
	virtual UPlayerStructManager* GetPlayerData_Implementation() override;

private:
	UPROPERTY()
		FString LocalPlayerName = TEXT("");
	UPROPERTY()
		int32 LocalPlayerSex = 0;
	UPROPERTY()
		UPlayerStructManager* LocalData = nullptr;
private:
	//检查本地文件是否存在
	bool LocalFileIsExist(FString FileName);
	//加载本地游戏存档
	USaveGame* LoadLocalSaveGame(const FString& FileName, const FString& LogMsg);
	//保存本地游戏存档
	void SaveLocalSaveGame(USaveGame* SaveGameObject, const FString& SlotName, const FString& LogMsg);
};


UCLASS()
class FVM_API UPlayerDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//获取角色存档子系统
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统")
		static UPlayerDataSubsystem* GetPlayerDataSubsystemStatic();

	//--------------------------------------------------------------------------------------角色
	//创建一个角色空实例对象
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统 | 角色")
		UPlayerStructManager* CreateEmptyPlayerInstance();
	//获取角色数据实例
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统 | 角色")
		UPlayerStructManager* GetPlayerDataInstance();
	//设置一个新的角色实例对象
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统 | 角色")
		void SetPlayerDataInstance(UPlayerStructManager* NewPlayerIns);
	//设置一个新的本地角色数据句柄对象
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统 | 角色")
		void SetLocalPlayerDataHandle(ULocalPlayerDataHandle* NewPlayerHandle);
	//获取一个新的本地角色数据句柄对象
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统 | 角色")
		ULocalPlayerDataHandle* GetLocalPlayerDataHandle();
	//--------------------------------------------------------------------------------------数据

	//保存角色数据
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统 | 角色数据")
		bool SavePlayerData(UObject* PlayerDataInterface, const FString& SaveLogMsg = TEXT(""));
	//加载角色数据
	UFUNCTION(BlueprintCallable, Category = "角色存档子系统 | 角色数据")
		bool LoadPlayerData(UObject* PlayerDataInterface, const FString& LoadLogMsg = TEXT(""));

	//--------------------------------------------------------------------------------------本地
	//创建一个本地玩家存档【并且自动设置角色数据实例和本地角色句柄】
	UFUNCTION()
		bool CreateNewLocalPlayerData(ULocalPlayerDataHandle* LocalPlayerHandle);
	//通过名称保存一个本地玩家存档
	UFUNCTION()
		bool SaveLocalPlayerData(ULocalPlayerDataHandle* LocalPlayerHandle, const FString& SaveLogMsg);
	//通过名称直接加载一个本地玩家存档
	UFUNCTION()
		ULocalPlayerDataHandle* LoadLocalPlayerData(FString LocalPlayerName, const FString& LoadLogMsg);
	//通过名称删除一个本地玩家存档	
	UFUNCTION()
		bool RemoveLocalPlayerData(ULocalPlayerDataHandle* LocalPlayerHandle);
	//--------------------------------------------------------------------------------------


	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
private:
	//当前角色数据实例
	UPROPERTY()
		class UPlayerStructManager* CurrentPlayerDataInstance = nullptr;
	//本地角色对象
	UPROPERTY()
		ULocalPlayerDataHandle* CurrentLocalPlayerDataHandle = nullptr;
};
