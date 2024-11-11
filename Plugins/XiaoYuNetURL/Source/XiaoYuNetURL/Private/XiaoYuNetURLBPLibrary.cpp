// Copyright Epic Games, Inc. All Rights Reserved.

#include "XiaoYuNetURLBPLibrary.h"
#include "XiaoYuNetURL.h"

UXiaoYuNetURLBPLibrary::UXiaoYuNetURLBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UXiaoYuNetURLBPLibrary::XGetBaseURL(FString& URL)
{
	URL = TEXT("http://192.168.1.8:8888/api/") + URL;
}

void UXiaoYuNetURLBPLibrary::XGetLoginURL(FString& URL, int32 State)
{
	switch (State)
	{
	case 2:
		UXiaoYuNetURLBPLibrary::XGetBaseURL(URL + TEXT("user/register"))
			; break;
	case 3:
		UXiaoYuNetURLBPLibrary::XGetBaseURL(URL + TEXT("user/logout"));
		break;
	case 4:
		UXiaoYuNetURLBPLibrary::XGetBaseURL(URL + TEXT("user/update"))
			; break;
	default:
		UXiaoYuNetURLBPLibrary::XGetBaseURL(URL + TEXT("user/login"));
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
	UXiaoYuNetURLBPLibrary::XGetBaseURL(URL + TEXT("item/queryAllItem"));
}

