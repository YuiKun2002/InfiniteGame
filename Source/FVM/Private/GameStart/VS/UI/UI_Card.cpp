// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_Card.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"

//地图实例
#include "GameStart/VS/GameMapInstance.h"
//卡片管理器
#include "GameStart/VS/Components/CardManagerComponent.h"

#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/CanvasPanelSlot.h>

//用于拖拽时，创建对应卡片的动画
#include "GameStart/Flipbook/GameActorFlipbookBase.h"

#include <kismet/GameplayStatics.h>


bool UUI_Card::Initialize()
{
	if (!Super::Initialize())
		return false;

	//this->M_Card_Butt = this->GetWidgetComponent<UButton>(this, TEXT("Bg"));
	this->M_Card_Select_Head = this->GetWidgetComponent<UImage>(this, TEXT("Card_Head"));
	this->M_UImage_Grade = this->GetWidgetComponent<UImage>(this, TEXT("Grade"));


	//this->M_Card_Select_Head->OnMouseButtonDownEvent.BindUFunction(this, TEXT(""));

	return true;
}

bool UUI_Card::GetCoolDownFinish()
{
	if (this->M_CardIndex == -1)
	{
		return false;
	}

	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		if (IsValid(AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent))
		{
			if (AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->GetCardCoolDownFinish(this->M_CardIndex))
			{
				if (AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->M_FlameNum >= this->M_NeedFlame)
				{
					return true;
				}
			}
		}
	}

	return false;
}

float UUI_Card::GetCoolDownFinishRate()
{
	if (this->M_CardIndex == -1)
	{
		return false;
	}

	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		if (IsValid(AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent))
		{
			return AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->M_CardCoolDownManager[
				this->M_CardIndex
			]->GetFinishRate();

		}
	}

	return false;
}

void UUI_Card::SetCardName(const FString& _Name)
{
	this->M_CardName = _Name;
}

void UUI_Card::SetCardGrade(const int32& _Grade)
{
	if (_Grade > 0 && _Grade < 17)
	{
		FSlateBrush Image;
		Image.SetResourceObject(LoadTexture2D(FString(TEXT("Texture2D'/Game/Resource/Texture/CardGrade/")) + FString::FromInt(_Grade) + FString(TEXT(".")) + FString::FromInt(_Grade) + FString(TEXT("'"))));
		Image.DrawAs = ESlateBrushDrawType::Image;
		this->M_UImage_Grade->SetBrush(Image);
		this->M_UImage_Grade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->M_UImage_Grade->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUI_Card::SetCoolDownFinish()
{
	AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->M_CardCoolDownManager[this->M_CardIndex]->SetCoolDownFinish();
}

void UUI_Card::SetCardSelect(bool _bSelect)
{
	this->bSelect = _bSelect;
}

void UUI_Card::SetMouseEnter(bool _bEnter)
{
	this->bMouseEnter = _bEnter;
}

void UUI_Card::PlaySelectCardAnimation(bool bForward)
{
	//Windows
	if (UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
	{
		//播放选择动画
		if (bForward)
			this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0], 0.f, 1, EUMGSequencePlayMode::Forward);
		else
			//鼠标移出 才可以播放逆序
			if (!this->bMouseEnter)
				this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0], 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
}

UPaperFlipbook* UUI_Card::CreateCard3DSceneFlipbook()
{
	return AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->GetCardIdleAnimByName(this->M_Card_ActorName);
}

UButton* UUI_Card::GetCardButtonA()
{
	return this->M_Card_Butt;
}

UImage* UUI_Card::GetCardHead()
{
	return this->M_Card_Select_Head;
}

FString UUI_Card::GetCardName()
{
	return this->M_CardName;
}

FString UUI_Card::GetCardActorName()
{
	return this->M_Card_ActorName;
}

bool UUI_Card::GetCardSelectState()
{
	return this->bSelect;
}

bool UUI_Card::GetMouseEnterState()
{
	return this->bMouseEnter;
}

void UUI_Card::SelectColor()
{
	//UWidgetBase::SetImageColor(this->GetCardButton()->WidgetStyle.Normal., FVector(0.2f, 0.2f, 0.2f), 1.f);

//	this->GetCardButton()->SetBackgroundColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.f));

	this->M_Card_Select_Head->SetColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 1.f));
}

void UUI_Card::CancelColor()
{
	//this->GetCardButton()->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));

	//鼠标移出 才可以取消选择的颜色
	if (!this->bMouseEnter)
		this->M_Card_Select_Head->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
}



