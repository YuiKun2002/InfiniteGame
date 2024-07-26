// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountain_3Box.h"

#include <Components/Image.h>

bool UUI_TreviFountain_3Box::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Bg = this->GetWidgetComponent<UImage>(this, TEXT("Image_46"));
	this->M_Head = this->GetWidgetComponent<UImage>(this, TEXT("Image_144"));

	return true;
}

void UUI_TreviFountain_3Box::SetBgStyle(uint8 _value)
{
	switch (_value)
	{
	case 0:UWidgetBase::SetImageBrush(this->M_Bg, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain_4/156.156'"),FVector(1.f),1.f,false,FVector(1.f),true); break;
	case 1:UWidgetBase::SetImageBrush(this->M_Bg, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain_4/158.158'"),FVector(1.f), 1.f, false, FVector(1.f), true); break;
	case 2:UWidgetBase::SetImageBrush(this->M_Bg, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain_4/159.159'"),FVector(1.f), 1.f, false, FVector(1.f), true); break;
	}
}

void UUI_TreviFountain_3Box::SetHead(const FString& _path)
{
	UWidgetBase::SetImageBrush(this->M_Head, _path,FVector(1.f),1.f,true,FVector(1.1f));
}
