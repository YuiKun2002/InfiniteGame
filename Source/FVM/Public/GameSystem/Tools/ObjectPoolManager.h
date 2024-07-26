// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolManager.generated.h"

// #include "GameSystem/Tools/ObjectPoolManager.h"


UINTERFACE(MinimalAPI)
class UObjectPoolInterface : public UInterface
{
	GENERATED_BODY()
};

//拥有对象池的功能，需要继承该接口
class FVM_API IObjectPoolInterface
{
	GENERATED_BODY()
public:
	//被对象池创建
	virtual void PoolCreate(class UObjectPoolManager* PoolManager) = 0;
	//被对象池初始化
	virtual void PoolInit(class UObjectPoolManager* PoolManager) = 0;
	//返回对象池【需要通过管理器返回】
	virtual bool ReturnPool() = 0;
};


//使用例子，这个例子作用于特效上了
UCLASS()
class FVM_API AObjectPoolClass : public AActor, public IObjectPoolInterface
{
	GENERATED_BODY()

public:
	//构造
	AObjectPoolClass();
	//当被池子创建
	virtual void PoolCreate(class UObjectPoolManager* PoolManager) override;
	//初始化池子
	virtual void PoolInit(class UObjectPoolManager* PoolManager) override;
	//返回池子
	UFUNCTION(BlueprintCallable)
		virtual bool ReturnPool() override;
private:
	//管理器
	UPROPERTY()
		class UObjectPoolManager* CurManager = nullptr;
};

/**
 * 对象池管理器
 */
UCLASS()
class FVM_API UObjectPoolManager : public UObject
{
	GENERATED_BODY()
public:
	//初始化管理器【必须继承接口（IObjectPoolInterface）】
	UFUNCTION(BlueprintCallable)
		static	UObjectPoolManager* MakePoolManager(
			class UWorld* World,
			TSoftClassPtr<AActor> ResPath,
			int32 InitCount
		);

	//获取
	UFUNCTION(BlueprintCallable)
		AActor* GetObjectActor();
	//返回资源
	UFUNCTION(BlueprintCallable)
		bool ReturnObjectActor(AActor* CurNewReturn);
	//是否初始化成功
	UFUNCTION(BlueprintCallable)
		bool InitComplete();
	//移除当前所有管理的资源
	UFUNCTION(BlueprintCallable)
		void ClearAllObjectActor();
	//设置对象池最大对象使用数量[0=无限制]
	UFUNCTION(BlueprintCallable)
		void SetObjectPoolMaxCount(int32 Count = 0);
private:
	//生成资源
	UFUNCTION()
		UClass* GenClass();
private:
	//能够使用的池子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> CanUsePool;
	//正在使用的池子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> CurUsePool;
	//资源
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSoftClassPtr<AActor> CurRes;
	//世界内容
	UPROPERTY()
		class UWorld* CurWorld = nullptr;
	//最大使用个数
	UPROPERTY()
		int32 UseCount = 0;
	UPROPERTY()
		int32 CurCount = 0;
};
