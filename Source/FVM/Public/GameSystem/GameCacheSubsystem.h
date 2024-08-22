// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "Tickable.h"
#include "CoreMinimal.h"
#include "VaRestSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Subsystems/EngineSubsystem.h"
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
	//设置Tag
	UFUNCTION(BlueprintCallable)
	void SetRequestTag(FName curTag);
	//设置请求地址
	UFUNCTION(BlueprintCallable)
	void SetRequestURL(const FString& requestURL);
	//设置请求内容
	UFUNCTION(BlueprintCallable)
	void SetRequestContent(const FString& json);
	//设置请求Json对象
	UFUNCTION(BlueprintCallable)
	void SetRequestJsonObject(class UVaRestJsonObject* VRJsonObject);
	//保存
	UFUNCTION(BlueprintCallable)
	void SaveGameCache();
public:
	//获取请求地址
	UFUNCTION(BlueprintPure)
	FString GetRequestURL() const;
	//获取请求的内容
	UFUNCTION(BlueprintPure)
	FString GetRequestContent() const;
	//获取请求的Json对象
	UFUNCTION(BlueprintPure)
	UVaRestJsonObject* GetRequestJsonObject();
	//获取结果
	UFUNCTION(BlueprintPure)
	bool GetResult() const;
public:
	//标记
	UPROPERTY()
	FName Tag;
	//请求地址
	UPROPERTY()
	FString RequestURL;
	//请求数据
	UPROPERTY()
	FString RequestContent;
	//请求的Json对象
	UPROPERTY()
	class UVaRestJsonObject* JsonObject = nullptr;
};

//蓝图异步请求类
UCLASS(BlueprintType, Blueprintable)
class FVM_API UGameCacheAsyncRequestFunction : public UObject {
	GENERATED_BODY()
public:
	//请求地址
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString URL;
	//标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> Tag;
	//请求Verb
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVaRestRequestVerb Verb;
	//请求类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVaRestRequestContentType ContentType;
	//发送的Json
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Json;
	//请求子系统
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UVaRestSubsystem* VaRestSub = nullptr;
	//请求缓存子系统
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UGameCacheSubsystem* GameCacheSubsystem = nullptr;
	//是否保存
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSaveCache = true;
public:
	//初始化
	UFUNCTION(BlueprintImplementableEvent)
	void Init();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCacheRequestDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameRequestDelegate, UGameCache*, gameCache);
//缓存请求节点
UCLASS(/*meta = (HideThen = true)*/)
class FVM_API UGameCacheAsyncRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	//成功
	UPROPERTY(BlueprintAssignable)
	FGameCacheRequestDelegate RequestComplet;

	//失败
	UPROPERTY(BlueprintAssignable)
	FGameCacheRequestDelegate RequestFailed;

	UPROPERTY()
	FVaRestCallTagDelegate CallBcak;

	UFUNCTION()
	void AsyncRequestComplet(FName Tag, UVaRestRequestJSON* Request);

	virtual void AsyncRequestCompletFunc(FName Tag, UVaRestRequestJSON* Request);


	//【保存到本地文件缓存】
	UFUNCTION(BlueprintCallable, Category = "GameCacheRequestAction")
	static UGameCacheAsyncRequest* GameCacheAsyncRequest(
		FString URL,
		TSubclassOf<UGameCacheAsyncRequestFunction> RequestTask
	);
protected:
	// 继承自父类的虚函数
	virtual void Activate() override;
protected:
	UPROPERTY()
	UGameCacheAsyncRequestFunction* Task = nullptr;
};


//异步请求节点【不会保存到本地文件】
UCLASS(/*meta = (HideThen = true)*/)
class FVM_API UGameAsyncRequest : public UGameCacheAsyncRequest
{
	GENERATED_BODY()
public:
	//成功
	UPROPERTY(BlueprintAssignable)
	FGameRequestDelegate RequestCacheComplet;
public:
	//【不会保存到本地文件】
	UFUNCTION(BlueprintCallable, Category = "GameCacheRequestAction")
	static UGameAsyncRequest* GameAsyncRequest(
		FString URL,
		TSubclassOf<UGameCacheAsyncRequestFunction> RequestTask,
		bool bSaveCache = true
	);

	virtual void AsyncRequestCompletFunc(FName Tag, UVaRestRequestJSON* Request) override;
};


//Json异步请求节点【不会保存到本地文件】
UCLASS(/*meta = (HideThen = true)*/)
class FVM_API UGameAsyncJsonRequest : public UGameAsyncRequest
{
	GENERATED_BODY()
public:
	//【不会保存到本地文件】
	UFUNCTION(BlueprintCallable, Category = "GameCacheRequestAction")
	static UGameAsyncJsonRequest* GameAsyncJsonRequest(
		FString URL,
		FString Json,
		TSubclassOf<UGameCacheAsyncRequestFunction> RequestTask,
		bool bSaveCache = true
	);

	virtual void AsyncRequestCompletFunc(FName Tag, UVaRestRequestJSON* Request) override;
};

/**
 * 游戏缓存子系统
 */
UCLASS()
class FVM_API UGameCacheSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	bool Request(
		class UVaRestSubsystem* VaRestSub,
		const FString& URL,
		const FString& Json,
		EVaRestRequestVerb Verb,
		EVaRestRequestContentType ContentType,
		FName TagName,
		const FVaRestCallTagDelegate& Callback
	);

	UFUNCTION(BlueprintCallable)
	UGameCache* RequestComplet(FName Tag, UVaRestRequestJSON* Request, bool bSave);

	//标签
	UFUNCTION(BlueprintPure)
	UGameCache* GetGameCache(TSubclassOf<class UAssetCategoryName> Tag);

	UGameCache* GetGameCache_Im(FName Tag);

private:
	UPROPERTY()
	TMap<FName, UGameCache*> GameCache;
};
