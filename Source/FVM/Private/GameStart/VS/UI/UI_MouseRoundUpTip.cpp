// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MouseRoundUpTip.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"

#include "GameSystem/GameMapStructManager.h"
#include "GameStart/VS/Components/VSManagerComponent.h"

#include "GameStart/VS/PlayerView.h"

#include <Components/TextBlock.h>
#include <Components/Image.h>

bool UUI_MouseRoundUpTip::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Time_Text = this->GetWidgetComponent<UTextBlock>(this, "Time_Text");
	this->M_Key_Name = this->GetWidgetComponent<UTextBlock>(this, "Key_Name");
	this->M_Key_Image = this->GetWidgetComponent<UImage>(this, "Key_Image");



	return true;
}

void UUI_MouseRoundUpTip::SetKeyImage(const FString& _ImageName)
{
	this->M_Key_Image->SetBrushFromTexture(LoadObject<UTexture2D>(0, *_ImageName));
}

void UUI_MouseRoundUpTip::SetKeyName(const FString& _KeyName)
{
	this->M_Key_Name->SetText(FText::FromString(_KeyName));
}

void UUI_MouseRoundUpTip::GameWin()
{
	AGameMapInstance::GetVSManagerComponent_Static()->GameWin();
}

void UUI_MouseRoundUpTip::GameContinue()
{
	//播放BGM
	AGameMapInstance::GetGameMapInstance()->PlaySound(AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.LevelUpMusic);

	if (this->bKey)
	{
		int32 A = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(this->Key);
		if (A != -1)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[A].M_Count -= 1;
			if (UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[A].M_Count <= 0)
			{
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[A].bWaitRemove = true;
			}
		}
	}
	else {
		int32 A = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(this->KeyB_A);
		int32 B = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(this->KeyB_B);
		if (A != -1 && B != -1)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[A].M_Count -= 1;
			if (UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[A].M_Count <= 0)
			{
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[A].bWaitRemove = true;
			}
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[B].M_Count -= 1;
			if (UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[B].M_Count <= 0)
			{
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[B].bWaitRemove = true;
			}
		}
	}

	//钥匙删除
	UFVMGameInstance::GetPlayerStructManager_Static()->Remove_Item(
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material
	);
	//保存游戏
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("使用了关卡钥匙或者徽章")));

	//设置Key的使用
	AGameMapInstance::GetVSManagerComponent_Static()->M_bUseKey = true;
	//继续游戏
	AGameMapInstance::GetVSManagerComponent_Static()->SetGameContinue();

	//进入下一个回合
	AGameMapInstance::GetMouseManagerComponent_Static()->ForceNextRound();
	AGameMapInstance::GetMouseManagerComponent_Static()->ShowMouseUI(
		AGameMapInstance::GetMouseManagerComponent_Static()->M_UMouseStructManager->GetRoundTotal()
	);
}

void UUI_MouseRoundUpTip::SetData(bool IsKey, FString Key1, FString Key2)
{
	this->bKey = IsKey;
	if (IsKey)
	{
		this->Key = Key1;
	}
	else {
		this->KeyB_A = Key1;
		this->KeyB_B = Key2;
	}
}
