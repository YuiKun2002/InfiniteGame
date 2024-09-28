// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_CardBar.h"

#include "GameStart/VS/UI/UI_EradicateCard.h"
#include "GameStart/VS/UI/UI_GamePlayerHead.h"

#include <Components/HorizontalBox.h>
#include <Components/VerticalBox.h>
#include <Components/UniformGridPanel.h>
#include <Components/Image.h>
#include <Components/Textblock.h>

#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/ScrollBox.h>
#include "Game/UI/UI_PlayerShow.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include<UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <kismet/KismetMathLibrary.h>


bool UUI_CardBar::Initialize()
{
	return Super::Initialize();
}

void UUI_CardBar::Init()
{
}

void UUI_CardBar::AddCard(UWidget* _Wid)
{
	this->CardPanel->AddChildToUniformGrid(_Wid,
		this->CardPanel->GetChildrenCount() / 2
		, this->CardPanel->GetChildrenCount() % 2);
}

void UUI_CardBar::AddPlayerInfor(UUI_GamePlayerHead* PlayerInfor)
{
}
