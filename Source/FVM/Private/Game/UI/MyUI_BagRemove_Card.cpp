// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/MyUI_BagRemove_Card.h"

#include <Components/Image.h>

bool UMyUI_BagRemove_Card::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_CardGradeImage = this->GetWidgetComponent<UImage>(this, "Grade");
	this->M_CardHead = this->GetWidgetComponent<UImage>(this, "Head");

	return true;
}

void UMyUI_BagRemove_Card::SetItemName(const FString& _ItemName)
{
	Super::SetItemName(_ItemName);

	this->M_CardName = _ItemName;
}

void UMyUI_BagRemove_Card::SetCard(const FString& _HeadPath, int32 _CardGrade)
{
	UWidgetBase::SetImageBrush(this->M_CardHead, _HeadPath);
	this->M_CardGradeImage->SetVisibility(ESlateVisibility::Hidden);
	if (_CardGrade > 0 && _CardGrade < 17)
	{
		FString _Grade = FString(TEXT("Texture2D'/Game/Resource/Texture/CardGrade/")) + FString::FromInt(_CardGrade) + "." + FString::FromInt(_CardGrade) + "'";
		UWidgetBase::SetImageBrush(this->M_CardGradeImage, _Grade);
	}
}
