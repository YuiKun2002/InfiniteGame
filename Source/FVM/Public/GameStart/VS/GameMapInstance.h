// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/MapBase.h"
#include "GameMapInstance.generated.h"

class UGameMapFogManager;
class APaperSpriteActor;
class UPaperSpriteComponent;
class UGameMapStructManager;
class UMesheControllComponent;
class UVSManagerComponent;
class UCardManagerComponent;
class UMouseManagerComponent;
class UResourceManagerComponent;
class UAudioComponent;
class UWidgetBase;
class AGamePlayer;
class AMapMeshe;
class UUI_MapMeshe;


//当放置角色的时候委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayPlayerDelegate);
//生成老鼠时触发点委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawnMouseDelegate, FString, MouseName, bool, bBoss);
//生成Boss时
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnMouseBossDelegate);
//当更新回合时
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateRoundDelegate, int32, Round);
//当最后一波时
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateRoundLastDelegate);


UCLASS()
class FVM_API AGameMapInstance : public AMapBase
{
	GENERATED_BODY()
private:
	//是否放置了玩家
	UPROPERTY()
		bool M_bIsPlayPlayer = false;
	//提示玩家放置角色的提示界面界面
	UPROPERTY()
		UWidgetBase* M_UI_PlayPlayerTipsWidget = nullptr;
	//玩家实体对象
	UPROPERTY()
		TMap<FString, AGamePlayer*> M_PlayerInstance;
public:
	//地图背景
	UPROPERTY(EditAnywhere, Category = "Actor | Bg")
		APaperSpriteActor* M_AMapBackground = nullptr;

	//静态地图实例
	static AGameMapInstance* M_AGameMapInstance;
public:
	//当放置角色时触发
	UPROPERTY(BlueprintAssignable)
		FOnPlayPlayerDelegate OnPlayPlayerDelegate;
	//当生成老鼠时
	UPROPERTY(BlueprintAssignable)
		FOnSpawnMouseDelegate OnSpawnMouseDelegate;
	//生成Boss时
	UPROPERTY(BlueprintAssignable)
		FOnSpawnMouseBossDelegate OnSpawnMouseBossDelegate;
	//当更新回合时
	UPROPERTY(BlueprintAssignable)
		FOnUpdateRoundDelegate OnUpdateRoundDelegate;
	//当最后一波时
	UPROPERTY(BlueprintAssignable)
		FOnUpdateRoundLastDelegate OnUpdateRoundLastDelegate;
public:
	//地图的配置结构
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Struct")
		UGameMapStructManager* M_UGameMapStructManager = nullptr;
	//格子控制组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UMesheControllComponent* M_MesheControllComponent = nullptr;
	//游戏开始VS管理器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UVSManagerComponent* M_VSManagerComponent = nullptr;
	//卡片管理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UCardManagerComponent* M_CardManagerComponent = nullptr;
	//老鼠管理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UMouseManagerComponent* M_MouseManagerComponent = nullptr;
	//资源管理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UResourceManagerComponent* M_ResourceManagerComponent = nullptr;
	//迷雾管理器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Fog")
		class UGameMapFogManager* FogManager = nullptr;
private:
	//是否在游戏(将区别与开发与实际游玩)
	UPROPERTY()
		bool IsInGame = false;
	//暂停
	UPROPERTY()
		bool IsInPause = false;
public:
	//获取卡片管理组件
	UFUNCTION(BlueprintCallable)
		static UCardManagerComponent* GetCardManagerComponent_Static();
	//获取VS管理组件
	UFUNCTION(BlueprintCallable)
		static UVSManagerComponent* GetVSManagerComponent_Static();
	//获取老鼠管理组件
	UFUNCTION(BlueprintCallable)
		static UMouseManagerComponent* GetMouseManagerComponent_Static();
	//获取资源管理组件
	UFUNCTION(BlueprintCallable)
		static UResourceManagerComponent* GetResourceManagerComponent_Static();
	//获取迷雾管理器
	UFUNCTION(BlueprintCallable)
		static UGameMapFogManager* GetGameMapFogManager_Static();
public:
	//当前正在播放的音频名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Condition")
		FString M_CurrentPlaySoundName = FString("");
public:
	// Sets default values for this actor's properties
	AGameMapInstance();

	//获取游戏地图实例
	UFUNCTION(BlueprintCallable)
		static AGameMapInstance* GetGameMapInstance();

	//创建一个2D的Actor到世界
	UFUNCTION(BlueprintCallable)
		void CreatePaperSpriteActorToWorld(APaperSpriteActor* _Actor, UPaperSprite* _PSprite, const FTransform& _ActorTranform);

	//获取网格控制组件
	UFUNCTION(BlueprintCallable)
		UMesheControllComponent* GetMesheControllComponent();

	//初始化地图配置管理
	UFUNCTION(BlueprintCallable)
		void InitGameMapStructManager(UGameMapStructManager* _UGameMapStructManager);

	//获取当前地图所使用的地图配置管理器
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* GetGameMapStructManager();

	//获取迷雾管理器
	UFUNCTION(BlueprintCallable)
		class UGameMapFogManager* GetGameMapFogManager();

	//获取角色对象
	UFUNCTION(BlueprintCallable)
		AGamePlayer* GetPlayer(FString PlayerName);
	//获取主角
	UFUNCTION(BlueprintCallable)
		AGamePlayer* GetMainPlayer();

	//播放背景音乐
	UFUNCTION(BlueprintCallable)
		void PlaySound(const FSoftObjectPath& _MusicName);
public:
	//获取游戏是否结束
	UFUNCTION(BlueprintCallable)
		bool GetGameOver();
	//游戏是否暂停
	UFUNCTION(BlueprintCallable)
		bool GetGamePause();
	//获取玩家是否已经放置了角色
	UFUNCTION(BlueprintCallable)
		bool GetIsPlayPlayer();
public:
	//设置游戏结束
	UFUNCTION(BlueprintCallable)
		void SetGameOver(bool _Value);
	//设置游戏暂停或者继续
	UFUNCTION(BlueprintCallable)
		void SetGamePause(bool _Value);
	//设置游戏现在开始(特指玩家放置了角色)
	UFUNCTION(BlueprintCallable)
		void SetGameStartNow();
	//设置角色
	UFUNCTION(BlueprintCallable)
		void SetPlayer(FString PlayerName, AGamePlayer* NewPlayerIns);
public:
	//生成角色
	UFUNCTION(BlueprintCallable)
		void SpawnPlayerToMeshe(AMapMeshe* _MapMeshe, UUI_MapMeshe* _UI_MapMeshe);
	//初始化静态地图的实例对象
	UFUNCTION(BlueprintCallable)
		bool InitStaticGameMapInstance();
public:
	//游戏开始
	UFUNCTION(BlueprintCallable)
		void GameStart();
	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
	//退出并结束游戏
	UFUNCTION(BlueprintCallable)
		void QuitGame();
	//游戏结束
	UFUNCTION(BlueprintCallable)
		static bool GetVSGameOver();
public:
	//初始化地图的背景
	void InitGameMapBackground();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

};

bool FVM_VS_GamePause();
bool FVM_VS_GameOver();
