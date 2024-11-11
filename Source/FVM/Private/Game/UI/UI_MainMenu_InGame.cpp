// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_MainMenu_InGame.h"

#include "GameSystem/PlayerDataSubsystem.h"
#include "GameSystem/GameConfigSubsystem.h"

#include "Game/UI/UI_MainMenu_PlayerData.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include <PlatformFeatures.h>
#include <SaveGameSystem.h>

#include <Components/ScrollBox.h>
#include <Components/Image.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>


void UUI_MainMenu_InGame::InitPlayerData()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
	{
		return;
	}

	//加载对应的界面
	UClass* ObjInstance = LoadClass<UUI_MainMenu_PlayerData>(0,
		TEXT("WidgetBlueprint'/Game/Resource/BP/Game/InGame/UI_InGame_PlayerData.UI_InGame_PlayerData_C'"));
	UUI_MainMenu_PlayerData* Data = nullptr;

	//获取配置
	UGameConfigManager* Config = UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();

	//当角色列表
	TMap<FString, FPlayerLoginBaseData> PlayerNames;
	Config->GetLoginPlayerNameList(PlayerNames);

	//当前选中的索引
	int32 _SelectIndex = 0;
	int32 LocalIndex = 0;
	FString CurSelectName = TEXT("");
	FString DefSelectName = TEXT("");

	for (const auto& CurName : PlayerNames)
	{
		if (LocalIndex == 0)
		{
			DefSelectName = CurName.Key;
		}

		Data = CreateWidget<UUI_MainMenu_PlayerData>(this, ObjInstance);

		Data->M_PlayerName = FText::FromString(CurName.Key);

		//获取等级
		Data->SetImage(CurName.Value.PlayerGradePath);

		Data->M_UI_MainMenu_InGame = this;

		Data->SetIndex(LocalIndex);

		if (CurName.Value.PlayerSex == 0)
		{
			Data->SetTextPink();
		}

		this->M_PlayerSelectBgs.Emplace(Data->GetSelectBoxBg());

		this->M_UVerticalBox_PlayerDataList->AddChild(Data);

		//当前选择的角色名称
		if (Config->GetCurrentLoginPlayerName().Equals(CurName.Key))
		{
			_SelectIndex = LocalIndex;
			CurSelectName = CurName.Key;
		}

		LocalIndex++;
	}

	//选择
	UGameSystemFunction::WidgetSelect(
		this->GetPlayerSelectBgArrays(),
		ESlateVisibility::SelfHitTestInvisible,
		_SelectIndex
	);

	//加载角色存档
	if (!CurSelectName.Equals(TEXT("")))
	{
		ULocalPlayerDataHandle* Hd = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->LoadLocalPlayerData(
			CurSelectName, __FUNCTION__ + FString(TEXT("进入界面加载配置文件中选择的角色"))
		);
		if (IsValid(Hd))
		{
			//获取玩家实例
			UPlayerStructManager* CurrentPlayerIns = Cast<IPlayerDataInterface>(Hd)->Execute_GetPlayerData(Hd);
			if (IsValid(CurrentPlayerIns))
			{
				//设置登录名称
				Config->SetCurrentLoginPlayerName(CurrentPlayerIns->M_PlayerName);
				//设置角色实例
				UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetPlayerDataInstance(CurrentPlayerIns);
				//设置本地角色句柄
				UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetLocalPlayerDataHandle(Hd);
			}
			else {
				Config->SetCurrentLoginPlayerName(TEXT(""));
			}
		}
		else {
			Config->SetCurrentLoginPlayerName(TEXT(""));
		}
	}
	else {
		if (PlayerNames.Num())
		{
			//获取玩家实例
			ULocalPlayerDataHandle* Hd = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->LoadLocalPlayerData(
				DefSelectName, __FUNCTION__ + FString(TEXT("进入界面加载配置文件中第一个角色"))
			);
			if (IsValid(Hd))
			{
				//获取玩家实例
				UPlayerStructManager* CurrentPlayerIns = Cast<IPlayerDataInterface>(Hd)->Execute_GetPlayerData(Hd);
				if (IsValid(CurrentPlayerIns))
				{
					//设置登录名称
					Config->SetCurrentLoginPlayerName(CurrentPlayerIns->M_PlayerName);
					//设置角色实例
					UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetPlayerDataInstance(CurrentPlayerIns);
					//设置本地角色句柄
					UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetLocalPlayerDataHandle(Hd);
				}
				else {
					Config->SetCurrentLoginPlayerName(TEXT(""));
				}
			}
			else {
				Config->SetCurrentLoginPlayerName(TEXT(""));
			}
		}
		else {
			Config->SetCurrentLoginPlayerName(TEXT(""));
		}
	}

	//初始化玩家当前选择的存档名称
	this->UpdatePlayerTextShow();
}

bool UUI_MainMenu_InGame::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化
	this->M_UVerticalBox_PlayerDataList = this->GetWidgetComponent<UVerticalBox>(this, "PlayerDataList");
	this->M_UEditableTextBox_PlayerInput = this->GetWidgetComponent<UEditableTextBox>(this, "Input_Player_Name");

	//初始化角色数据
	//this->InitPlayerData(); 

	//this->M_CurrentDes = FPaths::ProjectSavedDir() + TEXT("SaveGame/");

	return true;
}

void UUI_MainMenu_InGame::AddList(UClass* _Obj, const FString& __PlayerName, uint8 _Sex, FString PlayerGradeImgStr)
{
	UUI_MainMenu_PlayerData* Data = CreateWidget<UUI_MainMenu_PlayerData>(this, _Obj);
	Data->M_PlayerName = FText::FromString(__PlayerName);
	Data->SetImage(PlayerGradeImgStr);
	Data->M_UI_MainMenu_InGame = this;
	Data->SetIndex(this->M_PlayerSelectBgs.Num());
	if (_Sex == 0)
	{
		Data->SetTextPink();
	}
	this->M_PlayerSelectBgs.Emplace(Data->GetSelectBoxBg());

	UGameSystemFunction::WidgetSelect(
		this->M_PlayerSelectBgs,
		ESlateVisibility::SelfHitTestInvisible,
		this->M_PlayerSelectBgs.Num() - 1
	);

	this->M_UVerticalBox_PlayerDataList->AddChild(Data);
}

bool UUI_MainMenu_InGame::CreateNewPlayerData()
{
	//角色名称
	const FString& PlayerName = this->M_UEditableTextBox_PlayerInput->GetText().ToString();

	ULocalPlayerDataHandle* NewHandle = ULocalPlayerDataHandle::MakeLocalPlayerDataHandle(
		PlayerName,
		this->M_PlayerSex
	);

	if (!UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->CreateNewLocalPlayerData(NewHandle))
	{
		return false;
	}

	//拿取角色数据
	UPlayerStructManager* CurrentPlayerIns = Cast<IPlayerDataInterface>(NewHandle)->Execute_GetPlayerData(NewHandle);

	if (!IsValid(CurrentPlayerIns))
	{
		return false;
	}

	//获取游戏数据
	if (!UFVMGameInstance::GetFVMGameInstance())
	{
		return false;
	}

	//加载对应的界面
	UClass* ObjInstance = LoadClass<UUI_MainMenu_PlayerData>(0,
		TEXT("WidgetBlueprint'/Game/Resource/BP/Game/InGame/UI_InGame_PlayerData.UI_InGame_PlayerData_C'"));

	//获取配置
	UGameConfigManager* Config = UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();

	//设置角色配置名称
	FPlayerLoginBaseData NewData;
	NewData.PlayerSex = CurrentPlayerIns->M_PlayerSex;
	NewData.PlayerGradePath = CurrentPlayerIns->GetGradeImagePath();

	//添加角色名称显示
	Config->AddPlayerLoginName(
		CurrentPlayerIns->M_PlayerName,
		NewData
	);

	//添加到列表
	this->AddList(
		ObjInstance,
		CurrentPlayerIns->M_PlayerName,
		CurrentPlayerIns->M_PlayerSex,
		CurrentPlayerIns->GetGradeImagePath()
	);

	//设置当前登录的角色名称
	Config->SetCurrentLoginPlayerName(CurrentPlayerIns->M_PlayerName);


	this->UpdatePlayerTextShow();

	return true;
}

void UUI_MainMenu_InGame::UpdatePlayerTextShow()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
	{
		return;
	}

	//获取配置
	UGameConfigManager* Config = UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();
	//初始化玩家当前选择的存档名称
	this->M_CurrentPlayerDataName = FText::FromString(Config->GetCurrentLoginPlayerName());
}

bool UUI_MainMenu_InGame::InspectPlayerData()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
	{
		return false;
	}

	//
	UPlayerStructManager* CurrentPlayerIns = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance();
	if (!IsValid(CurrentPlayerIns))
	{
		return false;
	}

	/*
	//进行版本匹配
	if (!CurrentPlayerIns->M_GameVersion.Equals(UFVMGameInstance::GetGameVersion()))
	{

		//设置游戏更新
		UFVMGameInstance::SetUpdateGame();

		//创建弹窗
		UUserWidget* Widget = CreateWidget<UUserWidget>(
			this,
			LoadClass<UUserWidget>(0,
				TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_UpdateGameVersion.BPUI_UpdateGameVersion_C'")
				));
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
	*/

	//发送补偿邮件
	//UGameSystemFunction::SendVersionCompensate();

	return true;
}

TArray<UWidget*>& UUI_MainMenu_InGame::GetPlayerSelectBgArrays()
{
	return this->M_PlayerSelectBgs;
}
