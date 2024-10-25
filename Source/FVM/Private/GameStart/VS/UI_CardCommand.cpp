// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI_CardCommand.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameSystem/FVMGameInstance.h"

#include "GameStart/Flipbook/GameActorFlipbookBase.h"

bool UUI_CardCommand::Initialize()
{
	if (!Super::Initialize())
		return false;
	return true;
}

void UUI_CardCommand::Init()
{
	if (this->M_ACreateCardBar)
	{
		this->M_MapMeshe = this->M_ACreateCardBar->M_UI_MapMeshe;
	}
}

void UUI_CardCommand::EradicateCard()
{
	if (this->M_MapMeshe)
	{
		M_MapMeshe->EradicateCard(true);

		AGameActorFlipbookBase* Eradicate_ = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(LoadClass<AGameActorFlipbookBase>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/BPEradicate.BPEradicate_C'")), this->M_MapMeshe->M_MapMesheTransform);
		Eradicate_->SetAnimationPlayEndDestroy();
		//播放BGM——S
		UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio");

		this->RemoveCurrentPanel();
	}
}

void UUI_CardCommand::CreateNewCard(UCardManagerComponent* _CardComp)
{
	if (this->M_MapMeshe)
	{
		this->M_MapMeshe->CreateNewCard(_CardComp);
		this->RemoveCurrentPanel();
	}
}

void UUI_CardCommand::RemoveCurrentPanel()
{
	if (this->M_ACreateCardBar)
	{
		this->M_ACreateCardBar->Destroy();
		AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->ShowCardBar();
	}
}
