// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceManagerComponent.generated.h"

class AFlameActor;
class UUI_GameOver;
class UObjectPoolManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UResourceManagerComponent : public UActorComponent
{

	friend class UUI_GameOver;

	GENERATED_BODY()
public:

	/*---------------------------------火苗---------------------------------*/
	//火苗默认数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_FlameNum = 500;
	/*---------------------------------火苗---------------------------------*/

	//是否是白天
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDayTime = true;

	//静态飞行物对象池
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, UObjectPoolManager*> StaticPool;

	//本局击杀的老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> KillMouses;

	//本局放置的卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> PlayCards;
public:
	// Sets default values for this component's properties
	UResourceManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/*---------------------------------火苗---------------------------------*/
	//开启火苗自动掉落
	void EnableAutoFlameSpawn(const float& _SpawnTime);
	//添加火苗
	UFUNCTION(BlueprintCallable)
		void AddFlameNum(const int32& _Value);
	//减少火苗
	UFUNCTION(BlueprintCallable)
		void SubtractFlameNum(const int32& _Value);
	//获取火苗
	UFUNCTION(BlueprintCallable)
		const int32& GetCurrentFlameNum();
	/*---------------------------------火苗---------------------------------*/

	/*---------------------------------天---------------------------------*/

	//获取是白天还是夜晚
	UFUNCTION(BlueprintCallable)
		bool GetDay();
	/*---------------------------------天---------------------------------*/

	/*---------------------------------对象池---------------------------------*/
	//添加静态对象的对象池
	UFUNCTION(BlueprintCallable)
		bool AddNewStaticPool(FString ID, UObjectPoolManager* NewPool, bool bReplace = false);
	//获取静态对象的对象池
	UFUNCTION(BlueprintCallable)
		UObjectPoolManager* GetStaticPool(FString ID);
	//添加并获取静态对象池
	UFUNCTION(BlueprintCallable)
		UObjectPoolManager* GetStaticPoolOrAdd(FString ID, UObjectPoolManager* NewPool, bool bReplace = false);
	/*---------------------------------对象池---------------------------------*/


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//游戏开始
	UFUNCTION(BlueprintCallable)
		void GameStart();

	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();

public:

	//获取资源管理组件
	UFUNCTION(BlueprintCallable)
		static UResourceManagerComponent* GetResourceManagerComponent();

	//添加新的资源
	UFUNCTION(BlueprintCallable)
		static void AddResource(FString ResourceName, int32 ResourceCount);

	//获取当前所有的资源
	UFUNCTION(BlueprintCallable)
		static TMap<FString, int32> GetResource();

	//生成一个Actor实例
	UFUNCTION(BlueprintCallable)
		static AActor* CreateNewActor(FSoftClassPath Path);

	//添加卡片放置
	UFUNCTION(BlueprintCallable)
		static void ResourceAddPlayCard(FString CardName);

	//添加死亡的老鼠
	UFUNCTION(BlueprintCallable)
		static void ResourceAddKillMouse(FString MouseName);

	//生成掉落物[指定]
	UFUNCTION(BlueprintCallable)
		static void SpawnItemByName(FString ResourceName, FVector Location);

	//生成回合掉落
	UFUNCTION(BlueprintCallable)
		static void SpawnItem(FVector Location);

	//生成金币专属掉落
	UFUNCTION(BlueprintCallable)
		static void SpawnItemCoin(FVector Location);

	//添加经验
	UFUNCTION(BlueprintCallable)
		static void ResourceAddEx(int32 Ex);

	//添加分数
	UFUNCTION(BlueprintCallable)
		static void ResourceAddScore(int32 Score);

	//添加卡片损坏
	UFUNCTION(BlueprintCallable)
		static void ResourceAddBadCard();

	//从池中获取火苗
	UFUNCTION(BlueprintCallable)
		static	AFlameActor* GetFlame();

	//归还火苗
	UFUNCTION(BlueprintCallable)
		static void AddFlame(AFlameActor* CurFlameActor);
private:
	//火苗缓存池
	UPROPERTY()
		TArray<AFlameActor*> FlameActorResourcePool;
private:
	//当前本局获得的资源
	UPROPERTY()
		TMap<FString, int32> CurResource;
	//当前的分数
	UPROPERTY()
		int32 CurScore = 0;
	//当前分数等级
	UPROPERTY()
		FString CurScoreLevel = TEXT("F");
	//当前的经验值
	UPROPERTY()
		int32 CurEx = 0;
	//当前时间
	UPROPERTY()
		int32 CurLevelTime = 200;
	//当前被损坏的卡片
	UPROPERTY()
		int32 CurBadCardCount = 0;
private:
	UPROPERTY()
		bool bEnable = false;
	UPROPERTY()
		float CurSpawnTime = 0;
	UPROPERTY()
		float CurTime = 0;
};
