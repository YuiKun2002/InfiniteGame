// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountianItemBox.h"
#include <Components/Image.h>
#include <Components/Button.h>

bool UUI_TreviFountianItemBox::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Head = this->GetWidgetComponent<UImage>(this, TEXT("Head"));
	this->M_ItemEpic_Image = this->GetWidgetComponent<UImage>(this, TEXT("Item_Epic"));
	this->M_Button = this->GetWidgetComponent<UButton>(this, TEXT("Button_28"));


	this->M_Button->OnClicked.AddDynamic(this, &UUI_TreviFountianItemBox::ShowInformation);

	return true;
}

void UUI_TreviFountianItemBox::SetTreviFountainItemData(FTreviFountainItemStruct _Data)
{
	this->Data = _Data;
}

void UUI_TreviFountianItemBox::LoadData()
{
	UWidgetBase::SetImageBrush(this->M_Head, this->Data.M_ItemHeadPath.ToString(), FVector(1.f), 1, true);
	if (this->Data.M_ItemQuality == ETreviFountainItemQualityType::TQT_Epic)
	{
		this->M_ItemEpic_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->M_ItemEpic_Image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_TreviFountianItemBox::ShowInformation()
{
	if (!UGameplayStatics::GetPlatformName().Equals("Windows"))
	{
		UWidgetBase::CreateTipWidget(FString(this->Data.M_ItemName));
		UWidgetBase::PlayOperateAudio();
	}
}

FString UUI_TreviFountianItemBox::ToString_Implementation()
{
	return this->Data.M_ItemName;
}

void UUI_TreviFountianItemBox::RemoveInformation()
{
}
