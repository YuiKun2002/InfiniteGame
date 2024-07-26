// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerInfor.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

//角色形象
#include "Game/UI/UI_PlayerShow.h"

bool UUI_PlayerInfor::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_PlayerGrade = this->GetWidgetComponent<UImage>(this, "PlayerGrade");

	this->M_Player_Progress_Main = this->GetWidgetComponent<UImage>(this, "Slider_Main");
	this->M_Player_Progress_Bg = this->GetWidgetComponent<UImage>(this, "Slider_BG");
	this->M_Player_Progress_Text = this->GetWidgetComponent<UTextBlock>(this, "Slider_Text");

	this->M_Player_VIP = this->GetWidgetComponent<UButton>(this, "VIP");


	return true;
}



void UUI_PlayerInfor::UpdatePlayerHead()
{

}

void UUI_PlayerInfor::InitDataBase()
{
	this->PlayerTips = { TEXT("新手冒险家"),TEXT("初级冒险家"),TEXT("中级冒险家"),TEXT("巨匠冒险家"),TEXT("超神秘冒险家"),TEXT("传说冒险家"),TEXT("传说冒险家") };
}

void UUI_PlayerInfor::UpdatePlayerInfor()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();
	this->M_PlayerName = Player->M_PlayerName;
	
	
	int32 Index = Player->M_FPlayerEx.M_PlayerGrade / 10;

	this->M_PlayerTip = this->PlayerTips[Index];
	
	this->M_PlayerLog = TEXT("未来可自定义");

	//更新VIP显示
	UWidgetBase::SetButtonStyle(this->M_Player_VIP, FString("Texture2D'/Game/Resource/Texture/VIP/0/" + FString::FormatAsNumber(Player->M_FPlayerVIP.M_VIP + 1) + "." + FString::FormatAsNumber(Player->M_FPlayerVIP.M_VIP + 1) + "'"));
}

void UUI_PlayerInfor::UpdatePlayerGrade()
{
	UWidgetBase::SetImageBrush(this->M_PlayerGrade, UGameSystemFunction::GetPlayerGradeImagePath());
}

void UUI_PlayerInfor::UpdatePlayerEx()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	// X
	float _Value = UKismetMathLibrary::Lerp(5.f, 325.f, Player->M_FPlayerEx.M_CurrentEx / Player->M_FPlayerEx.M_CurrentExTop);

	//Position X
	float _PosiValue = UKismetMathLibrary::Lerp(-160.f, 0.f, Player->M_FPlayerEx.M_CurrentEx / Player->M_FPlayerEx.M_CurrentExTop);


	if (UWidgetBase::WidgetToCanvasSlot(this->M_Player_Progress_Main))
	{
		UWidgetBase::WidgetToCanvasSlot(this->M_Player_Progress_Main)->SetSize(FVector2D(_Value, 41.25f));
		UWidgetBase::WidgetToCanvasSlot(this->M_Player_Progress_Main)->SetPosition(FVector2D(_PosiValue, 0.f));

		FText M_Content = FText::FromString(FString(FString::FromInt((Player->M_FPlayerEx.M_CurrentEx / Player->M_FPlayerEx.M_CurrentExTop) * 100.f) + "%") + TEXT("  (") + FString::FromInt(Player->M_FPlayerEx.M_CurrentEx) + TEXT(" / ") + FString::FromInt(Player->M_FPlayerEx.M_CurrentExTop) + TEXT(")"));

		M_Player_Progress_Text->SetText(M_Content);
	}
}
