// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/UI_GamePlayerHead.h"
#include "Game/UI/UI_PlayerShow.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>

bool UUI_GamePlayerHead::Initialize()
{
	Super::Initialize();

	this->MainCharPoint = this->GetWidgetComponent<UCanvasPanel>(this, TEXT("PlayerPoint"));

	this->OtherCharHead = this->GetWidgetComponent<UImage>(this, TEXT("Image_119"));
	this->CharGradeHead = this->GetWidgetComponent<UImage>(this, TEXT("CurPlayerGrade"));
	this->CharName = this->GetWidgetComponent<UTextBlock>(this, TEXT("PlayerName"));

	return true;
}

void UUI_GamePlayerHead::Init(bool bMainPlayer, const FSoftObjectPath& OtherHead, int32 Grade, const FString& PlayerName)
{
	if (bMainPlayer)
	{
		this->OtherCharHead->SetVisibility(ESlateVisibility::Collapsed);
		this->MainCharPoint->SetVisibility(ESlateVisibility::Visible);

		this->OnMainPlayer();

		UUI_PlayerShow* PlayerInfor = CreateWidget<UUI_PlayerShow>(
			this,
			LoadClass<UUI_PlayerShow>(
				nullptr,
				TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerShow.BP_PlayerShow_C'")
				)
			);


		if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerSuit.M_bPlayerSuit
			&&
			UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerSuit.M_ShowPlayerSuit
			)
		{
			PlayerInfor->SetRenderScale(FVector2D(0.85f));
			PlayerInfor->SetRenderTranslation(FVector2D(0.f, -15.f));
		}

		this->MainCharPoint->AddChildToCanvas(PlayerInfor);
		this->CharGradeHead->SetBrushFromTexture(LoadObject<UTexture2D>(0, *UGameSystemFunction::GetPlayerGradeImagePath()));
		this->CharName->SetText(FText::FromString(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerName));
		return;
	}
	else {
		this->OtherCharHead->SetVisibility(ESlateVisibility::Visible);
		this->MainCharPoint->SetVisibility(ESlateVisibility::Collapsed);

		FString _Grade_Path =
			TEXT("Texture2D'/Game/Resource/Texture/PlayerGrade/") +
			FString::FromInt(Grade) +
			TEXT(".") +
			FString::FromInt(Grade) +
			TEXT("'");

		UWidgetBase::SetImageBrush(this->OtherCharHead, OtherHead.ToString());
		this->OtherCharHead->Brush.SetImageSize(FVector2D(142.f, 184.f));
		this->OtherCharHead->SetRenderScale(FVector2D(1.1f));
		this->CharGradeHead->SetBrushFromTexture(LoadObject<UTexture2D>(0, *_Grade_Path));
		this->CharName->SetText(FText::FromString(PlayerName));
	}
}
