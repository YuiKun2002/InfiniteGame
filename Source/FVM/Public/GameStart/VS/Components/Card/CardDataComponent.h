// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Data/CardData/CardDataStruct.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"

#include "CardDataComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardDataComponent();

public:

	//获取攻击类型的卡片数据
	UFUNCTION(BlueprintCallable)
		FItemCardATK GetCardATKData(const FString& _CardName);
	//获取生产类型的卡片数据
	UFUNCTION(BlueprintCallable)
		FItemCardSpawn GetCardSpawnData(const FString& _CardName);
	//获取防御类型的卡片数据
	UFUNCTION(BlueprintCallable)
		FItemCardDefence GetCardDefenceData(const FString& _CardName);
	//获取功能类型的卡片数据
	UFUNCTION(BlueprintCallable)
		FItemCardFunction GetCardFunction(const FString& _CardName);
	//获取范围类型的卡片数据
	UFUNCTION(BlueprintCallable)
		FItemCardRangeATK GetCardRangeATKData(const FString& _CardName);
	//获取卡片基本数据
	UFUNCTION(BlueprintCallable)
		static FItemCard GetCardBaseData(const FString& _CardName);

	template<class CardType = FItemCard>
	CardType GetCardData(const FString& _CardName, ECardType _ECardType)
	{
		//卡片数据
		TArray<CardType> CardDatas;

		//获取类型的卡片
		UCardBaseStruct::GetCardDatas<CardType>(_ECardType, CardDatas, 0);

		for (auto CardDataPP = CardDatas.CreateConstIterator(); CardDataPP; ++CardDataPP)
			if ((*CardDataPP).ItemName.ToString().Equals(_CardName))
				return (*CardDataPP);


		CardType CardData;
		return CardData;
	}
};
