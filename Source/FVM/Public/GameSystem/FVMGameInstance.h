// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FVMGameInstance.generated.h"

// #include "GameSystem/FVMGameInstance.h"

//用于加载存档和地图的各种配置
class UPlayerStructManager;
class UGameMapStructManager;
class UGameConfigManager;
class UMouseStructManager;

UCLASS()
class FVM_API UFVMGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//设置Debug(true则显示测试文字)
	UPROPERTY()
	bool M_bShowDebugText = false;
	//游戏是否更新
	UPROPERTY(EditAnywhere)
	bool M_bGameUpdate = false;
public:
	//设置更新
	UFUNCTION()
	static void SetUpdateGame();
	//设置游戏debug测试
	UFUNCTION(BlueprintCallable)
	static void SetGameDebug(bool _Value);
	//设置游戏版本
	UFUNCTION(BlueprintCallable)
	static void SetGameVersion(FString _Version);
public:
	//获取游戏配置
	UFUNCTION(BlueprintCallable)
	UGameConfigManager* GetGameConfigManager();
	//获取游戏地图配置管理
	UFUNCTION(BlueprintCallable)
	UGameMapStructManager* GetGameMapStructManager();
	//获取角色结构管理
	UFUNCTION(BlueprintCallable)
	UPlayerStructManager* GetPlayerStructManager();
	//获取老鼠结构管理
	UFUNCTION(BlueprintCallable)
	UMouseStructManager* GetMouseStructManager();

	//获取游戏debug测试
	UFUNCTION(BlueprintCallable)
	static bool GetDebug();
	//获取角色存档
	UFUNCTION(BlueprintCallable)
	static UPlayerStructManager* GetPlayerStructManager_Static();
	//获取配置文件
	UFUNCTION(BlueprintCallable)
	static UGameConfigManager* GetGameConfigManager_Static();
	//获取游戏版本
	UFUNCTION(BlueprintCallable)
	static FString GetGameVersion();
	//获取更新
	UFUNCTION(BlueprintCallable)
	static bool GetUpdateGame();
public:
	//加载一个游戏地图管理器（可以选择是否从数据表中加载数据,会先判断本地是否存在,如果选择true,会从数据表检索，如果成功则会自动创建）
	UFUNCTION(BlueprintCallable)
	UGameMapStructManager* LoadGameMapStructManager(const FString& _FileName);

	//--------------------------------------------------[角色配置]-------------------------------------------------------
	//获取游戏特效是否开启
	UFUNCTION(BlueprintCallable)
	static bool GetGameConfig_GameEFEnabled();
	//--------------------------------------------------[角色配置]-------------------------------------------------------
public:
	//获取FVM2游戏实例
	UFUNCTION(BlueprintCallable)
	static UFVMGameInstance* GetFVMGameInstance();

	//更新背景音乐的音量
	UFUNCTION(BlueprintCallable)
	void UpdateBGMVolume();
	//设置背景音乐缩放
	UFUNCTION(BlueprintCallable)
	static void SetBGMScale(float Value);
	//播放背景音乐
	UFUNCTION(BlueprintCallable)
	static void PlayBGM_Static(const FSoftObjectPath& _MusicName, bool _Loop = true);
	//获取当前播放的背景音乐
	UFUNCTION(BlueprintCallable)
	static FSoftObjectPath GetPlayBGMName();
	//播放音效
	UFUNCTION(BlueprintCallable)
	static void PlayBGM_S_Static(
		const FString& _MusicName,
		const FString& _MusicPath = FString(TEXT("GameBG"))
	);
	//获取音效音量
	UFUNCTION(BlueprintCallable)
	static float GetBGMS_Volume();
public:
	//删除玩家背包中有待删除的道具
	template<class ItemType>
	static void ClearWaitingItems(TArray<ItemType>& _Arrays)
	{
		TArray<ItemType> _Buff;

		for (ItemType& _Items : _Arrays)
		{
			//将未标记的物品添加到缓冲区
			if (!_Items.bWaitRemove)
				_Buff.Emplace(_Items);
		}
		//清空
		_Arrays.Empty();
		//重新复制新值
		_Arrays = _Buff;
	}
private:
	//游戏地图数据
	UPROPERTY()
	UGameMapStructManager* M_UGameMapStructManager = nullptr;
	//老鼠结构数据
	UPROPERTY()
	UMouseStructManager* M_MouseStructManager = nullptr;
};
