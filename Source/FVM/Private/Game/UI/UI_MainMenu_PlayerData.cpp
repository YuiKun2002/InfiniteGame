// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_MainMenu_PlayerData.h"
#include "Game/UI/UI_MainMenu_InGame.h"
#include "GameSystem/FVMGameInstance.h"

#include "GameSystem/GameConfigSubsystem.h"
#include "GameSystem/PlayerDataSubsystem.h"

#include <Components/TextBlock.h>
#include <Components/Image.h>

#include "GameSystem/GameConfigManager.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include "Game/UI/Tips/UI_SelectTip.h"

void UUI_MainMenu_PlayerData::UpdateData()
{
	if (this->M_UI_MainMenu_InGame)
	{
		this->M_UI_MainMenu_InGame->UpdatePlayerTextShow();
	}

	//保存配置
	UGameConfigSubsystem::GetGameConfigSubsystemStatic()->SaveLocalGameConfigInstance();
}

bool UUI_MainMenu_PlayerData::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_TextName = this->GetWidgetComponent<UTextBlock>(this, "TextBlock_47");
	this->M_SelectBG = this->GetWidgetComponent<UImage>(this, "Image");
	this->M_GradeBG = this->GetWidgetComponent<UImage>(this, "Grade_img");

	return true;
}

void UUI_MainMenu_PlayerData::SetIndex(int32 _Index)
{
	this->M_Index = _Index;
}

int32 UUI_MainMenu_PlayerData::GetIndex()
{
	return this->M_Index;
}

void UUI_MainMenu_PlayerData::SetImage(const FString& _img)
{
	UWidgetBase::SetImageBrush(this->M_GradeBG, _img);
}

bool UUI_MainMenu_PlayerData::SelectPlayer()
{
	//获取当前已经加载的本地角色存档
	UPlayerStructManager* PlayerIns = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance();
	if (IsValid(PlayerIns))
	{
		//名称相同(当前选择选项卡名称和存档当前选择的名称)
		if (PlayerIns->M_PlayerName.Equals(this->M_PlayerName.ToString()))
		{
			return true;
		}
	}

	ULocalPlayerDataHandle* Hd = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->LoadLocalPlayerData(
		this->M_PlayerName.ToString(), __FUNCTION__ + FString(TEXT("选择角色存档"))
	);
	//判断是否加载成功
	if (IsValid(Hd))
	{
		PlayerIns = Cast<IPlayerDataInterface>(Hd)->Execute_GetPlayerData(Hd);

		if (IsValid(PlayerIns))
		{
			//设置登陆名称
			UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance()->SetCurrentLoginPlayerName(
				PlayerIns->M_PlayerName
			);

			UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetPlayerDataInstance(PlayerIns);

			//选中
			UGameSystemFunction::WidgetSelect(
				this->M_UI_MainMenu_InGame->GetPlayerSelectBgArrays(),
				ESlateVisibility::SelfHitTestInvisible,
				this->GetIndex()
			);

			//更新数据
			this->UpdateData();

			return true;
		}
	}

	return false;
}

void UUI_MainMenu_PlayerData::PlayerClickDelet()
{
	//创建提示
	UUI_SelectTip* Tip = UUI_SelectTip::CreateSelectTip(this);
	Tip->M_Title = FText::FromString(TEXT("角色存档操作"));
	Tip->M_Content = FText::FromString(TEXT("是否要删除这个存档?此操作不可逆!"));
	Tip->M_Button_Ok_Text = FText::FromString(TEXT("删除"));
	Tip->M_Button_Cancel_Text = FText::FromString(TEXT("取消"));
	FScriptDelegate Click;
	Click.BindUFunction(this, "DeleteAll");
	Tip->GetButtonClickBind().Add(Click);
	//AddDynamic(this, &UUI_MainMenu_PlayerData::DeletePlayerData);
}

bool UUI_MainMenu_PlayerData::DeletePlayerData()
{
	UPlayerStructManager* PlayerIns = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance();
	UGameConfigManager* GameConfig = UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();
	if (IsValid(PlayerIns))
	{
		if (this->M_PlayerName.ToString().Equals(PlayerIns->M_PlayerName))
		{
			//移除当前本地配置
			if (GameConfig->RemovePlayerLoginName(PlayerIns->M_PlayerName))
			{
				//删除本地角色存档
				UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->RemoveLocalPlayerData(
					UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetLocalPlayerDataHandle()
				);
				//设置本地角色配置
				UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetPlayerDataInstance(nullptr);
			}
		}
		else {
			//如果选择的本地存档名称不等于当前加载的本地存档名称
			if (GameConfig->CheckPlayerLoginNameIsValid(this->M_PlayerName.ToString()))
			{
				//移除当前本地配置
				GameConfig->RemovePlayerLoginName(this->M_PlayerName.ToString());
				//删除本地角色存档
				UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->RemoveLocalPlayerData(
					UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetLocalPlayerDataHandle()
				);
			}
			else {
				return false;
			}
		}
	}
	else {
		//如果选择的本地存档名称不等于当前加载的本地存档名称
		if (GameConfig->CheckPlayerLoginNameIsValid(this->M_PlayerName.ToString()))
		{
			//移除当前本地配置
			GameConfig->RemovePlayerLoginName(this->M_PlayerName.ToString());
			//删除本地角色存档
			UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->RemoveLocalPlayerData(
				UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetLocalPlayerDataHandle()
			);
		}
		else {
			return false;
		}
	}


	this->UpdateData();

	return true;
}

void UUI_MainMenu_PlayerData::DeleteAll()
{
	this->DeletePlayerData();
	this->RemoveFromParent();
}

void UUI_MainMenu_PlayerData::SetTextPink()
{
	//更新字体颜色：女:粉
	//if (UFVMGameInstance::GetFVMGameInstance() && UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex == 0)
	this->M_TextName->SetColorAndOpacity(FSlateColor(FLinearColor(1.6f, 0.2f, 0.4f, 1.f)));
}

UImage* UUI_MainMenu_PlayerData::GetSelectBoxBg()
{
	return this->M_SelectBG;
}
