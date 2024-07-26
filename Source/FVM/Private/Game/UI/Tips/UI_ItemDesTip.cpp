// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Tips/UI_ItemDesTip.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>

// WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemDesTip.BPUI_ItemDesTip_C'

FOnButtonClickedEvent& UUI_ItemDesTip::GetOkButtonDelegate()
{
	return this->M_Ok->OnClicked;
}

FOnButtonClickedEvent& UUI_ItemDesTip::GetCancelButtonDelegate()
{
	return this->M_Cancel->OnClicked;
}

UButton* UUI_ItemDesTip::GetOkButtonComponent()
{
	return this->M_Ok;
}

UButton* UUI_ItemDesTip::GetCancelButtonComponent()
{
	return this->M_Cancel;
}

void UUI_ItemDesTip::SetOkButtonTitleText(FString _Content)
{
	this->M_Ok_TabName->SetText(FText::FromString(_Content));
}

void UUI_ItemDesTip::SetCancelButtonTitleText(FString _Content)
{
	this->M_Cancel_TabName->SetText(FText::FromString(_Content));
}

void UUI_ItemDesTip::SetContentTitleText(FString _Content)
{
	this->M_s_Title = _Content;
}

void UUI_ItemDesTip::SetContentText(FString _Content)
{
	this->M_s_Content = _Content;
}

void UUI_ItemDesTip::SetItemHeadPath(FString _2DTexture_Path)
{
	UWidgetBase::SetImageBrush(this->M_ItemHead,_2DTexture_Path,FVector(1.f),1.f,true,FVector(1.f,1.f,0.f));
}

bool UUI_ItemDesTip::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Ok = this->GetWidgetComponent<UButton>(this, TEXT("Ok"));
	this->M_Cancel = this->GetWidgetComponent<UButton>(this, TEXT("Cancel"));

	this->M_Ok_TabName = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_35"));
	this->M_Cancel_TabName = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock"));

	this->M_ItemHead = this->GetWidgetComponent<UImage>(this, TEXT("Image_130"));

	return true;
}