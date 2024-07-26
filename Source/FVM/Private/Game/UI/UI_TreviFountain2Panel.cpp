// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountain2Panel.h"

#include <Components/Image.h>

bool UUI_TreviFountain2Panel::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image_86")));
	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image")));
	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image_1")));
	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image_2")));
	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image_3")));
	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image_4")));
	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image_5")));
	this->M_Heads.Emplace(this->GetWidgetComponent<UImage>(this, TEXT("Image_6")));


	return true;
}


void UUI_TreviFountain2Panel::TPlayAnimation()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num() > 0)
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
}

void UUI_TreviFountain2Panel::SetItemHeads(TArray<FString>& _Heads)
{
	for (int32 i = 0; i < _Heads.Num(); i++)
	{
		UWidgetBase::SetImageBrush(this->M_Heads[i], _Heads[i], FVector(1.f), 1.f, true);
	}
}
