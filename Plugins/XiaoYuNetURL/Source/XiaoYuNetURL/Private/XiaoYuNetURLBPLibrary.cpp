// Copyright Epic Games, Inc. All Rights Reserved.

#include "XiaoYuNetURLBPLibrary.h"
#include "XiaoYuNetURL.h"

UXiaoYuNetURLBPLibrary::UXiaoYuNetURLBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UXiaoYuNetURLBPLibrary::XGetBaseURL(FString& URL)
{
	URL = TEXT("http://192.168.1.8:8888/api/");
}

void UXiaoYuNetURLBPLibrary::XGetLoginURL(FString& URL, int32 State)
{
	FString Temp;
	UXiaoYuNetURLBPLibrary::XGetBaseURL(Temp);
	switch (State)
	{
	case 2:
		URL = Temp + TEXT("user/register");
		break;
	case 3:
		URL = Temp + TEXT("user/logout");
		break;
	case 4:
		URL = Temp + TEXT("user/update");
		; break;
	default:
		URL = Temp + TEXT("user/login");
		break;
	}
}

FString UXiaoYuNetURLBPLibrary::XGetPlayerCoinURL(const FString& UserID)
{
	FString Str;
	UXiaoYuNetURLBPLibrary::XGetBaseURL(Str);

	Str += TEXT("money/getMoney?userId=") + UserID;

	return Str;
}

FString UXiaoYuNetURLBPLibrary::XGetPlayerBagURL(const FString& UserID)
{
	FString Str;
	UXiaoYuNetURLBPLibrary::XGetBaseURL(Str);

	Str += TEXT("package/getPackageItems?userId=") + UserID;

	return Str;
}

void UXiaoYuNetURLBPLibrary::XGetShopURL(FString& URL)
{
	FString Temp;
	UXiaoYuNetURLBPLibrary::XGetBaseURL(Temp);
	URL = Temp + TEXT("item/queryAllItem");
}

