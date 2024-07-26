// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_EradicateCard.h"
#include <Components/Button.h>
#include <Components/Image.h>

bool UUI_EradicateCard::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_EradicateImage = this->GetWidgetComponent<UImage>(this, TEXT("Head"));
	this->M_EradicateHead = this->GetWidgetComponent<UImage>(this, TEXT("Bg"));

	return true;
}

void UUI_EradicateCard::Select()
{
	UWidgetBase::SetImageColor(this->M_EradicateImage, FVector(0.3f));
}

void UUI_EradicateCard::Cancel()
{
	UWidgetBase::SetImageColor(this->M_EradicateImage, FVector(1.f));
}

UImage* UUI_EradicateCard::GetBG()
{
	return this->M_EradicateHead;
}
