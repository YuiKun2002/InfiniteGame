// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/GameCacheSubsystem.h"
#include "GameSystem/GameDataSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UGameCache::SetRequestTag(FName curTag)
{
	this->Tag = curTag;
}

void UGameCache::SetRequestURL(const FString& requestURL)
{
	UE_LOG(LogTemp, Warning, TEXT("RequestURL[%s]"), *requestURL);
	this->RequestURL = requestURL;
}

void UGameCache::SetRequestContent(const FString& json)
{
	UE_LOG(LogTemp, Warning, TEXT("RequestContent[%s]"), *json);
	this->RequestContent = json;
}

void UGameCache::SetRequestJsonObject(class UVaRestJsonObject* VRJsonObject)
{
	this->JsonObject = VRJsonObject;
}

void UGameCache::SaveGameCache()
{
	UGameplayStatics::SaveGameToSlot(
		this,
		FString("GameCache_") + Tag.ToString(),
		0);
}

FString UGameCache::GetRequestURL() const
{
	return this->RequestURL;
}

FString UGameCache::GetRequestContent() const
{
	return this->RequestContent;
}

UVaRestJsonObject* UGameCache::GetRequestJsonObject()
{
	return this->JsonObject;
}

bool UGameCache::GetResult() const
{
	if (this->Tag.IsEqual(TEXT("None")))
	{
		return false;
	}
	return true;
}

bool UGameCacheSubsystem::Request(
	UVaRestSubsystem* VaRestSub,
	const FString& URL,
	const FString& Json,
	EVaRestRequestVerb Verb,
	EVaRestRequestContentType ContentType,
	FName TagName,
	const FVaRestCallTagDelegate& Callback
)
{
	if (IsValid(VaRestSub))
	{
		UVaRestJsonObject* JsonObject = VaRestSub->ConstructVaRestJsonObject();

		if (!Json.IsEmpty())
		{
			JsonObject->DecodeJson(Json);
		}

		VaRestSub->CallTagURL(
			URL,
			Verb,
			ContentType,
			TagName,
			JsonObject,
			this->PlayerAccountToken,
			Callback
		);

		return true;
	}

	return false;
}

UGameCache* UGameCacheSubsystem::RequestComplet(FName Tag, UVaRestRequestJSON* Request, bool bSave)
{
	UGameCache* NewCache = NewObject<UGameCache>();
	NewCache->SetRequestTag(Tag);
	NewCache->SetRequestURL(Request->GetURL());
	NewCache->SetRequestContent(Request->GetResponseObject()->EncodeJsonToSingleString());
	NewCache->SetRequestJsonObject(Request->GetResponseObject());
	if (bSave)
	{
		this->GameCache.Emplace(Tag, NewCache);
	}

	return NewCache;
}

UGameCache* UGameCacheSubsystem::GetGameCache(TSubclassOf<class UAssetCategoryName> Tag)
{
	return this->GetGameCache_Im(Tag.GetDefaultObject()->GetCategoryName());
}

UGameCache* UGameCacheSubsystem::GetGameCache_Im(FName Tag)
{
	UGameCache** CurCache = this->GameCache.Find(Tag);
	if (CurCache && IsValid(*CurCache))
	{
		return (*CurCache);
	}
	return NewObject<UGameCache>();
}

void UGameCacheSubsystem::SetToken(const FString& Token)
{
	this->PlayerAccountToken = Token;
}

void UGameCacheAsyncRequest::AsyncRequestComplet(FName Tag, UVaRestRequestJSON* Request)
{
	this->AsyncRequestCompletFunc(Tag, Request);
}

void UGameCacheAsyncRequest::AsyncRequestCompletFunc(FName Tag, UVaRestRequestJSON* Request)
{
	if (IsValid(Request))
	{
		switch (Request->GetResponseCode())
		{
		case 0:
		{
			UE_LOG(LogTemp, Error, TEXT("code：0 服务器连接错误！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 200:
		{
			UE_LOG(LogTemp, Warning, TEXT("code：200 连接成功！"));
			UGameCache* CacheObject = this->Task->GameCacheSubsystem->RequestComplet(Tag, Request, true);
			int32 Response = CacheObject->GetRequestJsonObject()->GetIntegerField(TEXT("code"));
			if (Response == 0)
			{
				this->RequestComplet.Broadcast();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Response code：%d %s"),
					Response,
					*CacheObject->GetRequestJsonObject()->GetStringField(TEXT("message"))
				);
				this->RequestFailed.Broadcast();
			}
		}
		break;
		case 201:
		{
			UE_LOG(LogTemp, Error, TEXT("code：201 服务器创建！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 401:
		{
			UE_LOG(LogTemp, Error, TEXT("code：401 服务器未经授权！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 403:
		{
			UE_LOG(LogTemp, Error, TEXT("code：403 服务器被禁止！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 404:
		{
			UE_LOG(LogTemp, Error, TEXT("code：404 服务器未找到！"));
			this->RequestFailed.Broadcast();
		}
		break;
		default:
		{
			UE_LOG(LogTemp, Error, TEXT("code：%d 服务器未知错误！"), Request->GetResponseCode());
			this->RequestFailed.Broadcast();
		}
		break;
		}
	}

	this->Task->RemoveFromRoot();
	this->RemoveFromRoot();
}

UGameCacheAsyncRequest* UGameCacheAsyncRequest::GameCacheAsyncRequest(FString URL, TSubclassOf<UGameCacheAsyncRequestFunction> RequestTask)
{
	UGameCacheAsyncRequest* Re = NewObject<UGameCacheAsyncRequest>();
	Re->Task = RequestTask.GetDefaultObject();
	Re->Task->URL = URL;
	Re->Task->Json.Empty();
	Re->Task->bSaveCache = true;
	Re->Task->Init();
	Re->Task->AddToRoot();
	Re->AddToRoot();
	return Re;
}

void UGameCacheAsyncRequest::Activate()
{
	this->CallBcak.BindUFunction(this, TEXT("AsyncRequestComplet"));

	this->Task->GameCacheSubsystem->Request(
		this->Task->VaRestSub,
		this->Task->URL,
		this->Task->Json,
		this->Task->Verb,
		this->Task->ContentType,
		this->Task->Tag.GetDefaultObject()->GetCategoryName(),
		CallBcak
	);
}

UGameAsyncRequest* UGameAsyncRequest::GameAsyncRequest(FString URL, TSubclassOf<UGameCacheAsyncRequestFunction> RequestTask, bool bSaveCache)
{
	UGameAsyncRequest* Re = NewObject<UGameAsyncRequest>();
	Re->Task = RequestTask.GetDefaultObject();
	Re->Task->URL = URL;
	Re->Task->Json.Empty();
	Re->Task->bSaveCache = bSaveCache;
	Re->Task->Init();
	Re->Task->AddToRoot();
	Re->AddToRoot();
	return Re;
}

void UGameAsyncRequest::AsyncRequestCompletFunc(FName Tag, UVaRestRequestJSON* Request)
{
	if (IsValid(Request))
	{
		switch (Request->GetResponseCode())
		{
		case 0:
		{
			UE_LOG(LogTemp, Error, TEXT("code：0 服务器连接错误！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 200:
		{
			UE_LOG(LogTemp, Warning, TEXT("code：200 连接成功！"));
			//获取缓存
			UGameCache* CacheObject = this->Task->GameCacheSubsystem->RequestComplet(Tag, Request, this->Task->bSaveCache);
			if (IsValid(CacheObject))
			{
				int32 Response = CacheObject->GetRequestJsonObject()->GetIntegerField(TEXT("code"));
				if (Response == 0)
				{
					this->RequestComplet.Broadcast();
					this->RequestCacheComplet.Broadcast(CacheObject);
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("Response code：%d %s"),
						Response,
						*CacheObject->GetRequestJsonObject()->GetStringField(TEXT("message"))
					);
					this->RequestFailed.Broadcast();
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("code：200 临时缓存添加失败！"));
				this->RequestFailed.Broadcast();
			}
		}
		break;
		case 201:
		{
			UE_LOG(LogTemp, Error, TEXT("code：201 服务器创建！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 401:
		{
			UE_LOG(LogTemp, Error, TEXT("code：401 服务器未经授权！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 403:
		{
			UE_LOG(LogTemp, Error, TEXT("code：403 服务器被禁止！"));
			this->RequestFailed.Broadcast();
		}
		break;
		case 404:
		{
			UE_LOG(LogTemp, Error, TEXT("code：404 服务器未找到！"));
			this->RequestFailed.Broadcast();
		}
		break;
		default:
		{
			UE_LOG(LogTemp, Error, TEXT("code：%d 服务器未知错误！"), Request->GetResponseCode());
			this->RequestFailed.Broadcast();
		}
		break;
		}
	}

	this->Task->RemoveFromRoot();
	this->RemoveFromRoot();
}

UGameAsyncJsonRequest* UGameAsyncJsonRequest::GameAsyncJsonRequest(
	FString URL,
	FString Json,
	TSubclassOf<UGameCacheAsyncRequestFunction> RequestTask,
	bool bSaveCache
)
{
	UGameAsyncJsonRequest* Re = NewObject<UGameAsyncJsonRequest>();
	Re->Task = RequestTask.GetDefaultObject();
	Re->Task->URL = URL;
	Re->Task->Json = Json;
	Re->Task->bSaveCache = bSaveCache;
	Re->Task->Init();
	Re->Task->AddToRoot();
	Re->AddToRoot();
	return Re;
}

void UGameAsyncJsonRequest::AsyncRequestCompletFunc(FName Tag, UVaRestRequestJSON* Request)
{
	Super::AsyncRequestCompletFunc(Tag, Request);
}
