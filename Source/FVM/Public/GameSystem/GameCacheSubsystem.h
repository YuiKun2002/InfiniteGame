// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "VaRestSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameCacheSubsystem.generated.h"

class USaveGame;

/**
 * 游戏缓存对象
 */
UCLASS()
class FVM_API UGameCache : public USaveGame
{
	GENERATED_BODY()
public:
	//设置请求地址
	UFUNCTION(BlueprintCallable)
	void SetRequestURL(const FString& requestURL);
	//设置请求内容
	UFUNCTION(BlueprintCallable)
	void SetRequestContent(const FString& json);
public:
	//获取请求地址
	UFUNCTION(BlueprintPure)
	FString GetRequestURL() const;
	//获取请求的内容
	UFUNCTION(BlueprintPure)
	FString GetRequestContent() const;
public:
	//请求地址
	UPROPERTY()
	FString RequestURL;
	//请求数据
	UPROPERTY()
	FString RequestContent;
};

/**
 * 游戏缓存子系统
 */
UCLASS()
class FVM_API UGameCacheSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	bool Request(
		class UVaRestSubsystem* VaRestSub,
		class UPlayerStructManager* PlayerIns,
		const FString& URL,
		EVaRestRequestVerb Verb,
		EVaRestRequestContentType ContentType,
		TSubclassOf<class UAssetCategoryName> TagName,
		const FVaRestCallTagDelegate& Callback
	);

	UFUNCTION(BlueprintCallable)
	void RequestComplet(FName Tag, UVaRestRequestJSON* Request);
private:
	UPROPERTY()
	TMap<FName, UGameCache*> GameCache;
};
