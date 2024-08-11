// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/GameCacheSubsystem.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/PlayerStructManager.h"

void UGameCache::SetRequestURL(const FString& requestURL)
{
	this->RequestURL = requestURL;
}

void UGameCache::SetRequestContent(const FString& json)
{
	this->RequestContent = json;
}

FString UGameCache::GetRequestURL() const
{
	return this->RequestURL;
}

FString UGameCache::GetRequestContent() const
{
	return this->RequestContent;
}

bool UGameCacheSubsystem::Request(
	UVaRestSubsystem* VaRestSub,
	UPlayerStructManager* PlayerIns,
	const FString& URL,
	EVaRestRequestVerb Verb,
	EVaRestRequestContentType ContentType,
	TSubclassOf<class UAssetCategoryName> TagName,
	const FVaRestCallTagDelegate& Callback
)
{
	if (IsValid(VaRestSub) && IsValid(PlayerIns))
	{
		UVaRestJsonObject* JsonObject = VaRestSub->ConstructVaRestJsonObject();
		VaRestSub->CallTagURL(
			URL,
			Verb,
			ContentType,
			TagName.GetDefaultObject()->GetCategoryName(),
			JsonObject,
			Callback
		);

		return true;
	}

	return false;
}

void UGameCacheSubsystem::RequestComplet(FName Tag, UVaRestRequestJSON* Request)
{

}
