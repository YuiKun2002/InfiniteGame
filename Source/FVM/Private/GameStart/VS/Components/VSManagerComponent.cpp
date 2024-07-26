// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/VS/UI/UI_GamePlayerHead.h"
#include "GameStart/VS/UI/UI_VSCountDown.h"
#include "GameStart/VS/UI/UI_GameOver.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/UI/UI_CardBar.h"

#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameSystem/MouseStructManager.h"
#include "GameSystem/DialogueSubsystem.h"

#include "Math/UnrealMathUtility.h"

#include "Game/UI/Tips/UI_BottomTip.h"

#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UVSManagerComponent::UVSManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVSManagerComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...
	//UKismetSystemLibrary::ExecuteConsoleCommand(this->GetWorld(), FString(TEXT("viewmode unlit")));
	//UKismetSystemLibrary::ExecuteConsoleCommand(this->GetWorld(), FString(TEXT("stat fps")));

}


// Called every frame
void UVSManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (AGameMapInstance::GetGameMapInstance()->GetGamePause())
		return;


	if (AGameMapInstance::GetMouseManagerComponent_Static()->IsMouseSpawnEnd())
	{
		if (!AGameMapInstance::GetMouseManagerComponent_Static()->IsMouseExist())
		{
			this->SetComponentTickEnabled(false);

			//游戏结束
			this->GameWin();
		}
	}
}

bool UVSManagerComponent::CreatePlayer(
	bool bMainPlayer,
	FString PlayerName,
	int32 Row,
	int32 Col,
	int32 PlayerGrade,
	FSoftObjectPath PlayerHead,
	FPlayerSuitItem NewSuit,
	FString FirstWeaponName
)
{
	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		if (IsValid(AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent))
		{
			FLine RowAndCol = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshRowAndCol();
			if (Row >= RowAndCol.Row || Col >= RowAndCol.Col)
			{
				return false;
			}

			//网格对象
			AMapMeshe* MesheActor = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMesh(
				Row,
				Col
			);

			if (!IsValid(MesheActor))
			{
				return false;
			}
			UUI_MapMeshe* UIMeshe = MesheActor->GetUIMapMeshe();
			if (!IsValid(UIMeshe))
			{
				return false;
			}

			//判断当前网格是否存在对象
			if (!UIMeshe->TestID(0))
			{
				return false;
			}

			//根据地形生成不同的平台【水上，水中，岩浆等地形】
			switch (MesheActor->GetLineType())
			{
			case ELineType::OnWater:
			{
				UFVMGameInstance::PlayBGM_S_Static("PlayCardToOnWater", "ItemAudio");

				if (UIMeshe->TestID(-1))
				{
					FItemCard CardData = UCardDataComponent::GetCardBaseData(TEXT("木盘子"));
					UIMeshe->PlayCard(
						AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent
						, LoadClass<ACardActor>(0,
							TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Card/木盘子.木盘子_C'")
							), CardData, 0, false);
				}
			}break;
			case ELineType::Underwater:
			{
				UFVMGameInstance::PlayBGM_S_Static("PlayCardToOnWater", "ItemAudio");
				if (UIMeshe->TestID(3))
				{
					FItemCard CardData = UCardDataComponent::GetCardBaseData(TEXT("苏打气泡"));
					UIMeshe->PlayCard(
						AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent
						, LoadClass<ACardActor>(0,
							TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Card/苏打气泡.苏打气泡_C'")
							), CardData, 0, false);
				}
			}break;
			default:
			{
				UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio");
			}break;
			}


			AGamePlayer* PlayerIns = MesheActor->GetWorld()->SpawnActor<AGamePlayer>(LoadClass<AGamePlayer>(0,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Player/MyGamePlayer.MyGamePlayer_C'")));

			//添加ID
			UIMeshe->IdAdd(0, nullptr);
			UIMeshe->SetPlayer(PlayerIns);
			AGameMapInstance::GetGameMapInstance()->SetPlayer(PlayerName, PlayerIns);

			//设置旋转
			PlayerIns->SetActorRotation(FRotator(0.f, 90.f, 0.f));
			//初始化网格
			PlayerIns->InitMeshe(UIMeshe, MesheActor);

			if (bMainPlayer)
			{
				//初始化角色形象
				PlayerIns->InitPlayerData();
				//初始化武器
				PlayerIns->InitPlayerWeapon();
			}
			else {

				//设置新套装
				PlayerIns->SetPlayerSuit(NewSuit);

				//设置武器
				for (const auto& Data : UGlobalDatas::Global_SourceEquipmentData_WeaponFirst)
				{
					if (Data.M_FEquipment.ItemName.Equals(FirstWeaponName))
					{
						PlayerIns->LoadPlayerFirstWeapon(
							FirstWeaponName,
							Data.M_FEquipment.M_WeaponClassPath.ToString()
						);
						break;
					}
				}

				//武器无效
				if (!PlayerIns->PlayerFirstWeaponIsValid())
				{
					return false;
				}
			}

			//初始化排序
			PlayerIns->SetPlayerTranslucency(UIMeshe);
			//更新角色位置
			MesheActor->UpdatePlayerLocation();

			//更新网格碰撞
			UIMeshe->UpdateAllCardsCollision();
			//更新角色位置
			UIMeshe->GetMapMeshe()->UpdatePlayerLocation();

			//添加角色头像UI
			UUI_GamePlayerHead* UIPlayerHead = CreateWidget<UUI_GamePlayerHead>(
				UFVMGameInstance::GetFVMGameInstance(),
				LoadClass<UUI_GamePlayerHead>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/BP_PlayerHead.BP_PlayerHead_C'"))
				);
			UIPlayerHead->Init(bMainPlayer, PlayerHead, PlayerGrade, PlayerName);
			AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->M_UUI_CardBar->AddPlayerInfor(UIPlayerHead);

			return true;
		}
	}

	return false;
}

bool UVSManagerComponent::CreatePlayerByData(FString PlayerName, int32 Row, int32 Col, int32 PlayerGrade, TSoftObjectPtr<UDataTable> PlayerDataPtr)
{
	if (!IsValid(PlayerDataPtr.Get()))
	{
		if (!IsValid(PlayerDataPtr.LoadSynchronous()))
		{
			return false;
		}
	}

	UDataTable* Datas = PlayerDataPtr.Get();
	TArray<FTableRowBase*> SourceDatas;
	Datas->GetAllRows(TEXT("PlayerSuitDataList"), SourceDatas);

	for (FTableRowBase* CurData : SourceDatas)
	{
		FGamePlayerData TempData = *((FGamePlayerData*)(CurData));
		if (TempData.PlayerName.Equals(PlayerName))
		{
			// TempData.Suit
			FPlayerSuitItem TempSuitData;
			//当前是否有服装装备
			TempSuitData.M_bUse = true;
			//服装名称
			TempSuitData.M_SuitName = TempData.Suit.ItemName;
			//服装头像路径
			TempSuitData.M_SuitHeadPath = TempData.Suit.ItemTexturePath;
			//服装展示图片路径1
			TempSuitData.M_SuitFirstShowPath = TempData.Suit.M_PlayerEq_Path_First;
			//服装展示图片路径2
			TempSuitData.M_SuitSecondShowPath = TempData.Suit.M_PlayerEq_Path_Second;
			//服装默认动画1
			TempSuitData.M_SuitFirstDefAnimPath = TempData.Suit.M_PlayerEq_Animaion_Path_First;
			//服装默认动画2
			TempSuitData.M_SuitSecondDefAnim2Path = TempData.Suit.M_PlayerEq_Animaion_Path_Second;
			//服装攻击动画1
			TempSuitData.M_SuitFirstAttackAnimPath = TempData.Suit.M_PlayerEq_Animaion_Path_First_Attack;
			//服装攻击动画2
			TempSuitData.M_SuitSecondAttackAnim2Path = TempData.Suit.M_PlayerEq_Animaion_Path_Second_Attack;
			//服装体力加成
			TempSuitData.M_Suit_HP_Value = TempData.Suit.M_PlayerEq_HP_Value;
			//服装魅力加成
			TempSuitData.M_Suit_Charm_Value = TempData.Suit.M_PlayerEq_Charm_Value;
			//坐标偏移
			TempSuitData.M_PlayerEq_Offset = TempData.Suit.M_PlayerEq_Offset;
			//服装ID号
			TempSuitData.M_ItemID = TempData.Suit.M_ItemID;

			if (PlayerGrade <= 0)
			{
				PlayerGrade = 1;
			}
			else if (PlayerGrade > 60)
			{
				PlayerGrade = 60;
			}

			return UVSManagerComponent::CreatePlayer(false, PlayerName, Row, Col, PlayerGrade, TempData.Suit.M_PlayerEq_Path_First, TempSuitData, TempData.PlayerFirstWeaponName);
		}
	}

	return false;
}

void UVSManagerComponent::SetGamePause()
{
	AGameMapInstance::GetGameMapInstance()->SetGamePause(true);
}

void UVSManagerComponent::SetGameContinue()
{
	AGameMapInstance::GetGameMapInstance()->SetGamePause(false);
}

void UVSManagerComponent::GameStart()
{

	//创建按钮
	this->M_UIBottomTip = CreateWidget<UUI_BottomTip>(
		this->GetWorld(),
		LoadClass<UUI_BottomTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_BottomTip.BPUI_BottomTip_C'"))
		);



	//创建倒计时
	if (IsValid(AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()))
	{
		if (AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.LevelTimeWidthLevelConfig.bEnable)
		{
			this->UI_VSCountDown = CreateWidget<UUI_VSCountDown>(
				this->GetWorld(),
				LoadClass<UUI_VSCountDown>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/BPUI_Timer.BPUI_Timer_C'"))
				);

			//开始倒计时
			this->UI_VSCountDown->Enable(
				AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.LevelTimeWidthLevelConfig.MaxTime
			);

			//添加到屏幕
			this->UI_VSCountDown->AddToViewport();
		}
	}
}

void UVSManagerComponent::GameOver()
{
	//暂停游戏
	this->SetGamePause();

	//关闭计时
	if (IsValid(this->UI_VSCountDown))
	{
		this->UI_VSCountDown->DisEnable();
	}

	//结算界面
	this->M_UI_GameOver = CreateWidget<UUI_GameOver>(this->GetWorld(), LoadClass<UUI_GameOver>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UI_GameOver.UI_GameOver_C'")));
	this->M_UI_GameOver->AddToViewport();
	if (M_Win)
	{
		this->M_UI_GameOver->ShowOver1();
	}
	this->M_UI_GameOver->ShowOver2(M_Win);
	this->M_UI_GameOver->ShowOver3();

	//播放音乐
	if (M_Win)
	{
		UFVMGameInstance::PlayBGM_Static(FString(TEXT("SoundWave'/Game/Resource/BP/Audio/GameBG/Win.Win'")), false);
	}
	else {
		UFVMGameInstance::PlayBGM_Static(FString(TEXT("SoundWave'/Game/Resource/BP/Audio/GameBG/Failed.Failed'")), false);
	}
}

void UVSManagerComponent::ShowText(const FString& _Title)
{
	if (IsValid(this->M_UIBottomTip))
	{
		if (this->M_UIBottomTip->IsInViewport())
			this->M_UIBottomTip->RemoveFromViewport();

		this->M_UIBottomTip->AddToViewport(1);
		this->M_UIBottomTip->SetTitleAndPlay(_Title);
	}
}

void UVSManagerComponent::GameWin()
{
	AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->M_Win = true;

	//如果触发了剧情，才会进行保存
	if (UFVMGameInstance::GetPlayerStructManager_Static()->bShowDialogue)
	{
		//保存对话
		UDialogueSubsystem::SaveCurrentDialogue();
	}

	AGameMapInstance::GetGameMapInstance()->GameOver();
}

void UVSManagerComponent::GameFailed()
{
	AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->M_Win = false;

	//触发剧情
	UFVMGameInstance::GetPlayerStructManager_Static()->bShowDialogue = false;

	AGameMapInstance::GetGameMapInstance()->GameOver();
}

