// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/GameMapStructManager.h"
#include "GameStart/VS/GameMapFogManager.h"

#include "Data/GameLogSubsystem.h"

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"

#include "GameStart/VS/PlayerCameraPosition.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/PlayerView.h"

#include "GameStart/Flipbook/GameActor/GamePlayer.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperSpriteActor.h>
#include <Paper2D/Classes/PaperSpriteComponent.h>

#include "GameSystem/FVMGameInstance.h"

#include "UI/WidgetBase.h"

#include <Kismet/GameplayStatics.h>
#include <Sound/SoundBase.h>

//初始化静态变量
AGameMapInstance* AGameMapInstance::M_AGameMapInstance = nullptr;

UCardManagerComponent* AGameMapInstance::GetCardManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent;
}

UVSManagerComponent* AGameMapInstance::GetVSManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent;
}

UMouseManagerComponent* AGameMapInstance::GetMouseManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
}

UResourceManagerComponent* AGameMapInstance::GetResourceManagerComponent_Static()
{
	return AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent;
}

UGameMapFogManager* AGameMapInstance::GetGameMapFogManager_Static()
{
	return AGameMapInstance::GetGameMapInstance()->FogManager;
}

// Sets default values
AGameMapInstance::AGameMapInstance()
{
	PrimaryActorTick.bCanEverTick = true;
}


AGameMapInstance* AGameMapInstance::GetGameMapInstance()
{
	return AGameMapInstance::M_AGameMapInstance;
}

void AGameMapInstance::CreatePaperSpriteActorToWorld(APaperSpriteActor* _Actor, UPaperSprite* _PSprite, const FTransform& _ActorTranform)
{
	if (!_Actor)
		return;

	APaperSpriteActor* LAMapBackground = this->GetWorld()->SpawnActor<APaperSpriteActor>(_Actor->GetClass(), _ActorTranform);
	LAMapBackground->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	LAMapBackground->GetRenderComponent()->SetSprite(_PSprite);
}

UMesheControllComponent* AGameMapInstance::GetMesheControllComponent()
{
	return this->M_MesheControllComponent;
}

void AGameMapInstance::InitGameMapStructManager(UGameMapStructManager* _UGameMapStructManager)
{
	this->M_UGameMapStructManager = _UGameMapStructManager;
}

UGameMapStructManager* AGameMapInstance::GetGameMapStructManager()
{
	return this->M_UGameMapStructManager;
}

UGameMapFogManager* AGameMapInstance::GetGameMapFogManager()
{
	return this->FogManager;
}

AGamePlayer* AGameMapInstance::GetPlayer(FString PlayerName)
{
	AGamePlayer** CurPlayer = this->M_PlayerInstance.Find(PlayerName);
	if (CurPlayer)
	{
		return *CurPlayer;
	}

	return nullptr;
}

AGamePlayer* AGameMapInstance::GetMainPlayer()
{
	AGamePlayer** CurPlayer = this->M_PlayerInstance.Find(
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerName
	);
	if (CurPlayer)
	{
		return *CurPlayer;
	}

	return nullptr;
}

void AGameMapInstance::PlaySound(const FSoftObjectPath& _MusicName)
{
	//如果正在播放，那么判断正在播放的音频是否一样如果一样则不改变
	if (_MusicName.ToString().Equals(this->M_CurrentPlaySoundName))
	{
		return;
	}

	UFVMGameInstance::PlayBGM_Static(_MusicName);

	this->M_CurrentPlaySoundName = _MusicName.ToString();
}

bool AGameMapInstance::GetGameOver()
{
	return !this->IsInGame;
}

bool AGameMapInstance::GetGamePause()
{
	return this->IsInPause;
}

bool AGameMapInstance::GetIsPlayPlayer()
{
	return this->M_bIsPlayPlayer;
}

void AGameMapInstance::SetGameOver(bool _Value)
{
	this->IsInGame = _Value;
}

void AGameMapInstance::SetGamePause(bool _Value)
{
	//如果角色已经放置了，才可以自由设置
	if (this->M_bIsPlayPlayer)
		this->IsInPause = _Value;
	else
		this->IsInPause = true;
}

void AGameMapInstance::SetGameStartNow()
{
	if (IsValid(this->M_UI_PlayPlayerTipsWidget))
		this->M_UI_PlayPlayerTipsWidget->RemoveFromParent();

	this->M_bIsPlayPlayer = true;

	this->SetGamePause(false);
}


void AGameMapInstance::SetPlayer(FString PlayerName, AGamePlayer* NewPlayerIns)
{
	if (this->M_PlayerInstance.Find(PlayerName))
	{
		return;
	}

	this->M_PlayerInstance.Emplace(PlayerName, NewPlayerIns);
}

void AGameMapInstance::BeginPlay()
{
	//初始化静态变量
	this->InitStaticGameMapInstance();

	Super::BeginPlay();

	//初始化背景
	this->InitGameMapBackground();
	//初始化迷雾
	this->FogManager = NewObject<UGameMapFogManager>();

	//初始化游戏
	if (!this->M_bIsPlayPlayer)
	{
		//如果玩家没有放置->显示界面---提示玩家放置角色
		this->M_UI_PlayPlayerTipsWidget = CreateWidget<UWidgetBase>(
			this->GetWorld(),
			LoadClass<UWidgetBase>(0,
				TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/BPUI_PlayerPlayTips.BPUI_PlayerPlayTips_C'")
			)
		);
		this->M_UI_PlayPlayerTipsWidget->AddToViewport(0);

		this->SetGamePause(false);
	}
}

void AGameMapInstance::InitGameMapBackground()
{
	//创建背景
	this->M_AMapBackground = NewObject<APaperSpriteActor>(this->GetWorld(), "M_AMapBackground");

	this->CreatePaperSpriteActorToWorld(this->M_AMapBackground, this->M_MapSprite.M_MapBackgroundSprite,
		this->GetActorTransform());

	//设置摄像机位置
	//FVector Location = FVector::ZeroVector;
	//Location.Z = this->GetActorLocation().Z;

	//APlayerView::SetPlayerPositionAndFOV(Location,90.f);
}

// Called every frame
void AGameMapInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMapInstance::BeginDestroy()
{
	Super::BeginDestroy();

	//销毁实例
	AGameMapInstance::M_AGameMapInstance = nullptr;
}

void AGameMapInstance::GameStart()
{
	this->IsInGame = true;

	if (IsValid(this->M_VSManagerComponent))
	{

		if (UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
		{

			UGameLogSubsystem::GetGameLogSubsystemStatic()->ExeInstructions(TEXT("/clear"));
			UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(FALD(TEXT("【1键】可以连续铲除卡片")));
			UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(FALD(TEXT("【空格键】可以暂停游戏，暂停的同时可以使用指令！")));
			UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(FALD(TEXT("【F11键】可以退出或者进入全屏模式")));
		}

		this->M_VSManagerComponent->GameStart();
		this->M_CardManagerComponent->GameStart();
		this->M_MouseManagerComponent->GameStart();


		//放置预备卡片
		UMesheControllComponent* MesheCtlComp = this->M_MesheControllComponent;
		for (AActor* MeshActor : MesheCtlComp->GetAllMapMeshes())
		{
			if (AMapMeshe* CurMapMeshe = Cast<AMapMeshe>(MeshActor))
			{
				//如果没有UI则跳过
				if (!CurMapMeshe->GetUIMapMeshe())
				{
					continue;
				}

				TArray<FMesheColPrePlayCard> CurDatas = CurMapMeshe->GetPrePlayCard();
				//排序->从小到大，依次放置
				CurDatas.StableSort(
					[](const FMesheColPrePlayCard& A, const FMesheColPrePlayCard& B) {return A.CardLayer < B.CardLayer; }
				);

				for (const FMesheColPrePlayCard& CurCardData : CurDatas)
				{
					if (CurMapMeshe->GetUIMapMeshe()->CreateCard(
						CurMapMeshe->GetUIMapMeshe(),
						this->M_CardManagerComponent,
						LoadClass<ACardActor>(0, *CurCardData.CardActorResource.ToString()),
						CurCardData.CardName,
						0,
						false,
						false
					))
					{
						CurMapMeshe->GetUIMapMeshe()->UpdateAllCardsCollision();
					}
				}
			}
		}

		this->M_ResourceManagerComponent->GameStart();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("请重新启动项目!!!"));
	}

	//音乐初始化
	if (this->M_UGameMapStructManager)
		this->PlaySound(this->M_UGameMapStructManager->LevelConfig.LevelDefMusic);

}

void AGameMapInstance::GameOver()
{
	this->IsInGame = false;

	this->OnPlayPlayerDelegate.RemoveAll(this);
	this->OnSpawnMouseDelegate.RemoveAll(this);
	this->OnSpawnMouseBossDelegate.RemoveAll(this);
	this->OnUpdateRoundDelegate.RemoveAll(this);
	this->OnUpdateRoundLastDelegate.RemoveAll(this);

	FInputModeGameAndUI GameUI;
	GameUI.SetHideCursorDuringCapture(false);
	GameUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UGameplayStatics::GetPlayerController(this->GetWorld(), 0)->SetInputMode(GameUI);

	//优先结算资源管理器
	if (this->M_ResourceManagerComponent)
	{
		this->M_ResourceManagerComponent->GameOver();
	}

	if (this->M_VSManagerComponent)
	{
		this->M_VSManagerComponent->GameOver();
	}

	if (this->M_CardManagerComponent)
	{
		this->M_CardManagerComponent->GameOver();
	}

	if (this->M_MouseManagerComponent)
	{
		this->M_MouseManagerComponent->GameOver();
	}

	//	UGameplayStatics::SetGamePaused(this->GetWorld(),true);
}

void AGameMapInstance::QuitGame()
{
	UGameSystemFunction::SetCaptureLook(this->GetWorld(), EMouseLockMode::DoNotLock);

	this->IsInGame = false;
	this->M_CardManagerComponent->GameOver();
	this->M_MouseManagerComponent->GameOver();
	this->M_ResourceManagerComponent->GameOver();
}

bool AGameMapInstance::GetVSGameOver()
{
	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		return  AGameMapInstance::GetGameMapInstance()->GetGameOver();
	}

	return true;
}

void AGameMapInstance::SpawnPlayerToMeshe(AMapMeshe* _MapMeshe, UUI_MapMeshe* _UI_MapMeshe)
{
	if (!IsValid(_MapMeshe))
	{
		return;
	}

	//通过名称加载角色
	UEquipmentDataAssetCache* Cache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);
	//获取角色数据
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();
	//角色名称
	FName CurPlayerName = FName(*PlayerData->PlayerData.ItemName.ToString());
	//角色实例对象
	TSoftClassPtr<AGamePlayer>* PlayerSoftIns = nullptr;
	for (FEquipment_Hero_Data& CurDatas : Cache->GetHeroes())
	{
		PlayerSoftIns = CurDatas.Heroes.Find(CurPlayerName);
		if (PlayerSoftIns)
		{
			break;
		}
	}

	if (!PlayerSoftIns)
	{
		return;
	}

	//查询角色表格
	AGamePlayer* PlayerIns = this->GetWorld()->SpawnActor<AGamePlayer>(
		PlayerSoftIns->LoadSynchronous()
	);

	//设置旋转
	PlayerIns->SetActorRotation(FRotator(0.f, 90.f, 0.f));
	//初始化网格
	PlayerIns->InitMeshe(_UI_MapMeshe, _MapMeshe);
	//如果UI网格有效果
	if (IsValid(_UI_MapMeshe))
	{
		_UI_MapMeshe->SetPlayer(PlayerIns);
	}
	//初始化排序
	PlayerIns->SetPlayerTranslucency(_UI_MapMeshe);
	//初始化角色数据
	PlayerIns->InitPlayerData();
	//初始化武器
	PlayerIns->InitPlayerWeapon();
	//添加角色
	this->SetPlayer(
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerName,
		PlayerIns
	);
	//更新角色位置
	_MapMeshe->UpdatePlayerLocation();
	//初始化角色
	PlayerIns->InitPlayer(PlayerData->PlayerData);
	//游戏开始
	this->SetGameStartNow();
	//调用放置角色的委托
	this->OnPlayPlayerDelegate.Broadcast();
}

bool AGameMapInstance::InitStaticGameMapInstance()
{
	AGameMapInstance::M_AGameMapInstance = this;

	return true;
}

bool FVM_VS_GamePause()
{
	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		return  AGameMapInstance::GetGameMapInstance()->GetGamePause();
	}

	return true;
}

bool FVM_VS_GameOver()
{
	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		return  AGameMapInstance::GetGameMapInstance()->GetGameOver();
	}

	return true;
}
