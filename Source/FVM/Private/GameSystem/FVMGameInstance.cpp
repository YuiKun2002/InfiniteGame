// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/FVMGameInstance.h" 

#include "GameSystem/GameConfigSubsystem.h"
#include "GameSystem/PlayerDataSubsystem.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"
#include "Data/GameLogSubsystem.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "Game/UI/UI_MainFramework.h"

//地图数据-老鼠回合数据表
#include "Data/MapData/MapDataStruct.h"
//技能书数据表
#include "Data/CardSkillBookStruct.h"

#include "GameSystem/GlobalDatas.h"

#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "GameFramework/GameUserSettings.h"

//UI 层 函数库
#include <Blueprint/WidgetLayoutLibrary.h>

//标题提示UI->争对Windows平台
#include "Game/UI/Tips/UI_ItemTitleTip.h"

#include "GameSystem/GameUserInterfaceSubsystem.h"


//是否显示游戏测试文字
bool UFVMGameInstance::M_bShowDebugText = false;

UUI_ItemTitleTip* const UFVMGameInstance::GetUI_ItemTitleTip()
{
	if (UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
	{
		return this->M_Windows_ShowUI_ItemTitleTip;
	}

	return nullptr;
}

UFVMGameInstance::UFVMGameInstance()
{

}

void UFVMGameInstance::Init()
{
	Super::Init();

	//预加载数据
	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Log, TEXT("加载静态UFVMGameInstance"));
		UE_LOG(LogTemp, Log, TEXT("加载数据表!"));
	}

	UGlobalDatas::InitCardsDatasLoader();


	//初始化->争对Windows平台UI
	if (UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
	{
		this->M_Windows_ShowUI_ItemTitleTip = CreateWidget<UUI_ItemTitleTip>(
			this->GetWorld(),
			LoadClass<UUI_ItemTitleTip>(0,
				TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemTitleTip.BPUI_ItemTitleTip_C'")));
	}
}

void UFVMGameInstance::Shutdown()
{
	Super::Shutdown();

	//卡片数据卸载
	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Log, TEXT("卸载数据表!"));
	}

	UGlobalDatas::UnLoadCardsDatasLoader();

	//判断地图UI是否存在
	if (this->M_CurrentMap)
	{
		this->M_CurrentMap->RemoveFromParent();
	}

	//玩家的主要UI界面显示
	if (this->M_MainUIFrame)
	{
		this->M_MainUIFrame->RemoveFromParent();
	}
}

FString UFVMGameInstance::GetGameVersion()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion;
	}

	return TEXT("");
}

UUI_MainFramework* UFVMGameInstance::GetMainFramework()
{
	return this->M_MainUIFrame;
}

void UFVMGameInstance::SetUpdateGame()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate = true;
	}
}

void UFVMGameInstance::SetMainFramework(UUI_MainFramework* _UUI_MainFramework)
{
	if (!this->M_MainUIFrame)
		this->M_MainUIFrame = _UUI_MainFramework;

	this->M_MainUIFrame->AddToViewport();
}

void UFVMGameInstance::SetGameDebug(bool _Value)
{
	UFVMGameInstance::M_bShowDebugText = _Value;
}

bool UFVMGameInstance::GetDebug()
{
	return UFVMGameInstance::M_bShowDebugText;
}

void UFVMGameInstance::SetGameVersion(FString _Version)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_ConfigVersion = _Version;
	}
}

void UFVMGameInstance::SetWorldMapWidget(UWidgetBase* _CurrentMap)
{
	if (_CurrentMap && this->M_CurrentMap)
		this->M_CurrentMap->RemoveFromParent();

	this->M_CurrentMap = _CurrentMap;

	if (this->M_CurrentMap)
		this->M_CurrentMap->AddToViewport();
}

bool UFVMGameInstance::GetUpdateGame()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate;
	}

	return false;
}

UWidgetBase* UFVMGameInstance::GetWorldMap()
{
	return this->M_CurrentMap;
}

UGameMapStructManager* UFVMGameInstance::LoadGameMapStructManager(const FString& _FileName)
{
	UDataTable* GameMapData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/MapData/GameMapData.GameMapData'"));
	TArray<FGameMapData*> M_GameMapData;
	GameMapData->GetAllRows("GameMapList", M_GameMapData);

	for (auto Datas : M_GameMapData)
	{
		//从数据表寻找指定的地图名称
		if ((*Datas).M_FLevelConfig.LevelName.Equals(_FileName))
		{
			//创建新的地图结构并且将数据表的数据赋予给新创建的地图结构
			this->M_UGameMapStructManager = Cast<UGameMapStructManager>(
				UGameplayStatics::CreateSaveGameObject(
					LoadClass<UGameMapStructManager>(0, TEXT("Class'/Script/FVM.GameMapStructManager'"))
				));
			this->M_UGameMapStructManager->GameMapStruct = (*Datas).M_FGameMapStruct;
			this->M_UGameMapStructManager->LevelConfig = (*Datas).M_FLevelConfig;


			//创建新的老鼠结构并且把数据表的数据赋予给新的老鼠结构
			this->M_MouseStructManager = Cast<UMouseStructManager>(
				UGameplayStatics::CreateSaveGameObject(
					LoadClass<UMouseStructManager>(0, TEXT("Class'/Script/FVM.MouseStructManager'"))
				));
			//设置老鼠配置
			this->M_MouseStructManager->SetMouseConfig((*Datas).M_FMouseConfig);
			//设置行数量
			this->M_MouseStructManager->SetRowCount(this->M_UGameMapStructManager->GameMapStruct.M_Meshe.Num());

			//返回数据
			return this->M_UGameMapStructManager;
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Log, TEXT("地图配置管理器加载失败!"));
	}

	return nullptr;
}

UGameConfigManager* UFVMGameInstance::GetGameConfigManager()
{
	if (UGameConfigSubsystem::GetGameConfigSubsystemStatic())
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();
	}

	return nullptr;
}

UGameMapStructManager* UFVMGameInstance::GetGameMapStructManager()
{
	return this->M_UGameMapStructManager;
}

UPlayerStructManager* UFVMGameInstance::GetPlayerStructManager()
{
	if (IsValid(UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()))
	{
		return UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance();
	}

	return nullptr;
}

void UFVMGameInstance::CreateBaseTipWidget(const FString& _ShowTitle, bool _bShow)
{
	UUI_ItemTitleTip* const UITitle = UFVMGameInstance::GetFVMGameInstance()->GetUI_ItemTitleTip();
	if (IsValid(UITitle))
	{
		if (_bShow)
		{
			//设置标题
			UITitle->SetShow(true, _ShowTitle);
			//获取鼠标位置
			FVector2D LPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(UFVMGameInstance::GetFVMGameInstance()->GetWorld());
			//设置缩放
			LPosition *= UWidgetLayoutLibrary::GetViewportScale(UFVMGameInstance::GetFVMGameInstance()->GetWorld());
			//设置最终位置
			UITitle->SetPositionInViewport(LPosition, true);
		}
		else {
			UITitle->SetShow(false, TEXT(""));
		}
	}
}

bool UFVMGameInstance::GetGameConfig_GameEFEnabled()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameEF.M_bEnable;
	}

	return false;
}

void UFVMGameInstance::UpdateBGMVolume()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->UpdateGameBGMVolume();
	}
}

void UFVMGameInstance::SetBGMScale(float Value)
{
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->SetGameBGMVolume(Value);
	}
}

UMouseStructManager* UFVMGameInstance::GetMouseStructManager()
{
	return this->M_MouseStructManager;
}

UPlayerStructManager* UFVMGameInstance::GetPlayerStructManager_Static()
{
	if (IsValid(UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()))
	{
		return UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance();
	}
	return nullptr;
}

UGameConfigManager* UFVMGameInstance::GetGameConfigManager_Static()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance();
	}

	return nullptr;
}

UFVMGameInstance* UFVMGameInstance::GetFVMGameInstance()
{
	if (IsValid(GEngine))
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		if (context)
		{
			return Cast<UFVMGameInstance>(context->OwningGameInstance);
		}
	}

	return nullptr;
}

void UFVMGameInstance::CreateMainFramePanel()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		if (UFVMGameInstance::GetFVMGameInstance()->GetMainFramework())
		{
			UFVMGameInstance::GetFVMGameInstance()->GetMainFramework()->RemoveFromParent();

			UFVMGameInstance::GetFVMGameInstance()->GetMainFramework()->AddToViewport();
		}
		else {
			UFVMGameInstance::GetFVMGameInstance()->SetMainFramework(
				CreateWidget<UUI_MainFramework>(
					UFVMGameInstance::GetFVMGameInstance()->GetWorld(),
					LoadClass<UUI_MainFramework>(0,
						TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/BPUI_MainFrame.BPUI_MainFrame_C'")
						)));
		}
	}
}


void UFVMGameInstance::PlayBGM_Static(const FSoftObjectPath& _MusicName, bool _Loop)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->PlayGameBGM(_MusicName, _Loop);
	}
}

FSoftObjectPath UFVMGameInstance::GetPlayBGMName()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetCurrentPlayGameBgmPath();
	}

	return FSoftObjectPath();
}

void UFVMGameInstance::PlayBGM_S_Static(const FString& _MusicName, const FString& _MusicPath)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameConfigSubsystem::GetGameConfigSubsystemStatic()->PlayGameSoundEffects(_MusicName, _MusicPath);
	}
}

float UFVMGameInstance::GetBGMS_Volume()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameSoundEffectsVolume();
	}

	return 0.f;
}