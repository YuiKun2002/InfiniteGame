// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSystem/Tools/GameSystemFunction.h"
#include <Components/Button.h>
#include "UI/WidgetBase.h"
#include "Data/GameLogSubsystem.h"

#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

#include "GameStart/Flipbook/SpriteActor.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"

#include "Paper2D/Classes/PaperFlipbook.h"

#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameFramework/GameUserSettings.h"
#include "GameSystem/GameConfigSubsystem.h"
#include "GameSystem/PlayerDataSubsystem.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetStringLibrary.h>

#include <iomanip>

FDateTime UGameSystemFunction::GameSystem_Time;
FDateTime UGameSystemFunction::GameSystem_Time_Now;
FRandomStream UGameSystemFunction::Random_Now;
bool UGameSystemFunction::bEnable = false;

void UGameSystemFunction::UpdateTime()
{
	if (UGameSystemFunction::bEnable)
	{
		UGameSystemFunction::GameSystem_Time_Now = UGameSystemFunction::GameSystem_Time.Now();
		UGameSystemFunction::Random_Now.Initialize(UGameSystemFunction::GameSystem_Time_Now.GetHour() + UGameSystemFunction::GameSystem_Time_Now.GetMinute() + UGameSystemFunction::GameSystem_Time_Now.GetSecond() + UGameSystemFunction::GameSystem_Time_Now.GetMillisecond());
	}
}

void UGameSystemFunction::OpenUpdate()
{
	UGameSystemFunction::bEnable = true;
}

void UGameSystemFunction::CloseUpdate()
{
	UGameSystemFunction::bEnable = false;
}

void UGameSystemFunction::ClearTimeClip(FTimerManager& _TimeManager, FTimeClip& _Clip)
{
	_TimeManager.ClearTimer(_Clip.M_FTimerHandle);
	_Clip.M_Count = 0;
}

void UGameSystemFunction::SaveCurrentPlayerData(const FString& SaveLogMsg)
{
	if (IsValid(UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance()))
	{
		UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SavePlayerData(
			UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetLocalPlayerDataHandle(), SaveLogMsg
		);
	}
}

UPlayerStructManager* UGameSystemFunction::LoadCurrentPlayerData(const FString& LoadLogMsg)
{
	if (IsValid(UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance()))
	{
		ULocalPlayerDataHandle* Hd = UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetLocalPlayerDataHandle();
		if (UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->LoadPlayerData(
			Hd, LoadLogMsg
		))
		{
			return Cast<IPlayerDataInterface>(Hd)->Execute_GetPlayerData(Hd);
		}
	}

	return nullptr;
}

void UGameSystemFunction::SaveCurrentGameConfigData()
{
	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("游戏配置数据已保存"));
	}

	UGameConfigSubsystem::GetGameConfigSubsystemStatic()->SaveLocalGameConfigInstance();
}

void UGameSystemFunction::SendMailToCurrentPlayer(FMail& _Mail)
{
	//默认赋值ID从0 开始
	int32 _ID_Count = 0;
	for (FMail& Mail : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails)
	{
		//如果遇到相同的则叠加
		Mail.M_Mail_ID = _ID_Count;
		_ID_Count++;
	}

	_Mail.M_Mail_ID = _ID_Count + 1;

	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails.Emplace(_Mail);

	UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(
		FALD(TEXT("【公告】说：玩家【")) + FAL(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName,
			TEXT("player")) + FALD(TEXT("】新增邮件【")) + FAL(_Mail.M_MailSendItemName, TEXT("g")) + FALD(TEXT("】"))
	);
}

FMaterialBase* UGameSystemFunction::GetMaterialDataFromBag(TArray<FMaterialBase>& _Arrays, const FString& _ItemName)
{
	for (FMaterialBase& Items : _Arrays)
	{
		if (Items.ItemName.ToString().Equals(_ItemName))
			return &Items;
	}
	return nullptr;
}

void UGameSystemFunction::GetMaterialsArrayByType(
	TArray<FMaterialBase>& _ArraysSource,
	const TArray<EMaterialType>& _Type,
	TArray<FMaterialBase*>& OutArrays
)
{
	//更具类型添加材料
	for (const auto& LType : _Type)
	{
		for (FMaterialBase& Item : _ArraysSource)
		{
			if (Item.M_MaterialType == LType)
			{
				OutArrays.Emplace(&Item);
			}
		}
	}
}

FString UGameSystemFunction::GetCardGradeImagePath(int32 _Grade)
{
	// Texture2D'/Game/Resource/Texture/CardGrade/1.1'

	if (_Grade == 0)
		return FString();

	FString _GradePath = "Texture2D'/Game/Resource/Texture/CardGrade/" + FString::FormatAsNumber(_Grade) + "." + FString::FormatAsNumber(_Grade) + "'";
	return _GradePath;
}

bool UGameSystemFunction::GetRange(int32 _ValueMaxRange, int32 _EndMaxRange)
{
	int32 _Result = UGameSystemFunction::Random_Now.RandRange(1, _EndMaxRange);;

	//UGameSystemFunction::FVMLog(__FUNCTION__, FString::FromInt(_ValueMaxRange) + TEXT(" <= ") + FString::FromInt(_Result));

	if (_ValueMaxRange >= _Result)
		return true;

	return false;
}

int32 UGameSystemFunction::GetRange_Int(int32 _ValueMaxRange, int32 _EndMaxRange)
{
	return UGameSystemFunction::Random_Now.RandRange(1, _EndMaxRange);
}

int32 UGameSystemFunction::GetRandomRange(int32 Random_Min, int32 RandomNums)
{
	return UGameSystemFunction::Random_Now.RandRange(Random_Min, RandomNums);
}

float UGameSystemFunction::GetRandomRangeFloat(float Random_Min, float RandomNums)
{
	return UGameSystemFunction::Random_Now.FRandRange(Random_Min, RandomNums);
}

float UGameSystemFunction::GetGCD_f(int32 X, int32 Y)
{
	if (X % Y == 0)
	{
		return Y;
	}

	return GetGCD_f(Y, X % Y);
}

void UGameSystemFunction::GetScale(
	const int32& X,
	const int32& Y,
	FString& OutScaleX,
	FString& OutScaleY
)
{
	//X 看成 宽
	//Y 看成 高
	//分别匹配大小到Max和Min
	int32 Max = X;
	int32 Min = Y;
	if (Max < Y)
	{
		Max = Y;
		Min = X;
	}

	// Max宽高最大
	// Min宽高最小
	// 10 9 3 1  以9为基数
	float Target9 = Max * 9.f / Min;

	constexpr int Accuracy = 10;
	const int BigRoundResult = FMath::RoundToInt(Target9 * Accuracy);
	const FString IntPart = FString::FromInt(BigRoundResult / Accuracy);
	const FString DigPart = FString::FromInt(
		BigRoundResult > 0 ? BigRoundResult % Accuracy : (-1) * BigRoundResult % Accuracy
	);

	OutScaleX = IntPart + TEXT(".") + DigPart;

	OutScaleY = TEXT("9");
}

void UGameSystemFunction::TabSelect(const TArray<UButton*>& _ButtonArrays, const  FString& _UniformPath, const  TArray<FString>& _DefButtonStyle, const TArray<FString>& _ClickButtonStyle, int32 _Index)
{
	//确保索引相同
	if (_ButtonArrays.Num() == _DefButtonStyle.Num() && _DefButtonStyle.Num() == _ClickButtonStyle.Num())
	{
		int32 Count = _ButtonArrays.Num();

		if (_Index < 0 || _Index >= Count)
			return;

		//先全部使用默认样式
		for (int32 i = 0; i < Count; i++)
		{
			FString Path = FString(_UniformPath + _DefButtonStyle[i] + TEXT(".") + _DefButtonStyle[i] + TEXT("'"));
			UWidgetBase::SetButtonStyle(_ButtonArrays[i], Path, true);
			_ButtonArrays[i]->SetVisibility(ESlateVisibility::Visible);
		}

		//激活指定的选项卡
		FString Path = FString(_UniformPath + _ClickButtonStyle[_Index] + TEXT(".") + _ClickButtonStyle[_Index] + TEXT("'"));
		UWidgetBase::SetButtonStyle(_ButtonArrays[_Index], Path, true);
		_ButtonArrays[_Index]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UGameSystemFunction::WidgetsSelect(const TArray<UWidget*>& _Widgets, ESlateVisibility _ShowType, int32 _Index)
{
	UGameSystemFunction::WidgetSelect<UWidget>(_Widgets, _ShowType, _Index);
}

void UGameSystemFunction::FVMLog(const FString& _FuncName, const FString& _Content)
{
#if WITH_EDITOR
	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s"), *_FuncName, *_Content);
		if (UGameLogSubsystem::GetGameLogSubsystemStatic())
		{
			UGameLogSubsystem::GetGameLogSubsystemStatic()->AddPrivate(
				FALD(_FuncName + TEXT(" ") + _Content)
			);
		}
	}
#endif
}

void UGameSystemFunction::AddGameLog(const UObject* WorldContext, const FString& _Content)
{
	UGameLogSubsystem* Cur = UGameplayStatics::GetGameInstance(
		WorldContext
	)->GetSubsystem<UGameLogSubsystem>();
	if (IsValid(Cur))
	{
		Cur->AddCurrent(_Content);
	}
}

void UGameSystemFunction::AddDebugLog(const UObject* WorldContext, const FString& _Content)
{
	if (UFVMGameInstance::GetDebug())
	{
		UGameLogSubsystem* Cur = UGameplayStatics::GetGameInstance(
			WorldContext
		)->GetSubsystem<UGameLogSubsystem>();
		if (IsValid(Cur))
		{
			Cur->AddPrivate(_Content);
		}
	}
}

bool UGameSystemFunction::AddGameinstructions(const UObject* WorldContext, const FString& InputContent, bool bTip)
{
	if (bTip)
	{
		UGameSystemFunction::AddGameLog(WorldContext, FAL(InputContent, TEXT("Ins")));
	}

	UGameLogSubsystem* Cur = UGameplayStatics::GetGameInstance(
		WorldContext
	)->GetSubsystem<UGameLogSubsystem>();
	if (IsValid(Cur))
	{
		return Cur->ExeInstructions(InputContent, bTip);
	}

	return false;
}

FString UGameSystemFunction::BlueprintAddLog(const FString& Content, const FString& Flag /*= FString(TEXT("t")*/)
{
	return FVMAddLogPanel(Content, Flag);
}

void UGameSystemFunction::LoadMap()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(Player))
	{
		UGameUserInterfaceSubsystem* UserInter = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();

		FString MapPah = TEXT("MainMap");
		switch (Player->M_PlayerInMap.CurrentInMap)
		{
		case EGameVsMap::VM_Map_1:
			MapPah = TEXT("Map1");
			break;
		case EGameVsMap::VM_Map_2:
			MapPah = TEXT("Map2");
			break;
		case EGameVsMap::VM_Map_3:
			MapPah = TEXT("Map3");
			break;
		case EGameVsMap::VM_Map_4:
			MapPah = TEXT("Map4");
			break;
		case EGameVsMap::VM_Map_5:
			MapPah = TEXT("Map5");
			break;
		case EGameVsMap::VM_Main:
		default:
			MapPah = TEXT("MainMap");
			break;
		}

		//关闭当前进入的UI地图
		UserInter->GetUserInterInstance(UI_WORLDMAPUINAME)->RemoveUI_ByName(FName(Player->M_PlayerInMap.CurrentInMapUIName));
		//设置新的UI地图名称
		Player->M_PlayerInMap.CurrentInMapUIName = MapPah;
		//显示新的UI地图
		UserInter->GetUserInterInstance(UI_WORLDMAPUINAME)->ShowUI_ByName(FName(Player->M_PlayerInMap.CurrentInMapUIName));

		//重新显示框架
		UserInter->GetUserInterInstance(UI_GLOBALUINAME)->RemoveUI_ByName(TEXT("MainFrame"));
		UserInter->GetUserInterInstance(UI_GLOBALUINAME)->ShowUI_ByName(TEXT("MainFrame"));

		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("{加载当前进入的地图UI}")));
	}

}

void UGameSystemFunction::LoadLastMap()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		UGameUserInterfaceSubsystem* UserInter = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();

		FString MapPah = TEXT("MainMap");
		switch (Player->M_PlayerInMap.LastInMap)
		{
			//美味镇不能是上一次
		case EGameVsMap::VM_Main:
		case EGameVsMap::VM_Map_1:
			MapPah = TEXT("Map1");
			break;
		case EGameVsMap::VM_Map_2:
			MapPah = TEXT("Map2");
			break;
		case EGameVsMap::VM_Map_3:
			MapPah = TEXT("Map3");
			break;
		case EGameVsMap::VM_Map_4:
			MapPah = TEXT("Map4");
			break;
		case EGameVsMap::VM_Map_5:
			MapPah = TEXT("Map5");
			break;
		}

		//关闭当前进入的UI地图
		UserInter->GetUserInterInstance(UI_WORLDMAPUINAME)->RemoveUI_ByName(FName(Player->M_PlayerInMap.CurrentInMapUIName));
		//设置新的UI地图名称
		Player->M_PlayerInMap.CurrentInMapUIName = MapPah;
		//显示新的UI地图
		UserInter->GetUserInterInstance(UI_WORLDMAPUINAME)->ShowUI_ByName(FName(Player->M_PlayerInMap.CurrentInMapUIName));

		//重新显示框架
		UserInter->GetUserInterInstance(UI_GLOBALUINAME)->RemoveUI_ByName(TEXT("MainFrame"));
		UserInter->GetUserInterInstance(UI_GLOBALUINAME)->ShowUI_ByName(TEXT("MainFrame"));

		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("{加载上一次进入的地图UI}")));
	}
}

void UGameSystemFunction::SetMap(EGameVsMap _Map)
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		if (Player->M_PlayerInMap.CurrentInMap != _Map)
		{
			Player->M_PlayerInMap.CurrentInMap = _Map;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("设置当前的地图")));
			}
		}
	}
}

void UGameSystemFunction::SetLastMap(EGameVsMap _Map)
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		if (Player->M_PlayerInMap.LastInMap != _Map)
		{
			Player->M_PlayerInMap.LastInMap = _Map;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("设置上一次的地图")));
			}
		}
	}
}

FString UGameSystemFunction::GetPlayerGradeImagePath()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();
	FString _Grade_Path = TEXT("Texture2D'/Game/Resource/Texture/PlayerGrade/") + FString::FromInt(Player->M_FPlayerEx.M_PlayerGrade) + TEXT(".") + FString::FromInt(Player->M_FPlayerEx.M_PlayerGrade) + TEXT("'");
	return _Grade_Path;
}

EGameVsMap UGameSystemFunction::GetPlayerInMap()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		return Player->M_PlayerInMap.LastInMap;
	}

	return EGameVsMap::VM_Main;
}


EGameVsMap UGameSystemFunction::GetPlayerCurrentMap()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		return Player->M_PlayerInMap.CurrentInMap;
	}

	return EGameVsMap::VM_Main;
}

EGameVsMap UGameSystemFunction::GetPlayerLastInMap()
{
	return EGameVsMap::VM_Map_1;
}


void UGameSystemFunction::ResetDataTable(class UDataTable* _datatable)
{
#if WITH_EDITOR
	for (const auto& Name : _datatable->GetRowNames())
	{
		FItemCard* CardData = nullptr;

		FCard_Spawn_Data* CurRow = _datatable->FindRow<FCard_Spawn_Data>(Name, TEXT("Card"));
		if (CurRow)
			CardData = &CurRow->M_FItemCard;

		if (!CardData)
		{
			FCard_Defence_Data* CurRowA = _datatable->FindRow<FCard_Defence_Data>(Name, TEXT("Card"));
			if (CurRowA)
				CardData = &CurRowA->M_FItemCard;
		}

		if (!CardData)
		{
			FCard_RangeATK_Data* CurRowB = _datatable->FindRow<FCard_RangeATK_Data>(Name, TEXT("Card"));
			if (CurRowB)
				CardData = &CurRowB->M_FItemCard;
		}

		if (!CardData)
		{
			FCard_ATK_Data* CurRowC = _datatable->FindRow<FCard_ATK_Data>(Name, TEXT("Card"));
			if (CurRowC)
				CardData = &CurRowC->M_FItemCard;
		}

		if (CardData)
		{
			//加载Actor资源
			//FString Ref = FString(TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Card/" + CardData->ItemName + "." + CardData->ItemName + "_C'");
			//CardData->CardActorResource = FSoftClassPath(Ref);
		//	CardData->CardActorHead = FSoftObjectPath(FString(TEXT("Texture2D'/Game/Resource/Texture/Sprite/VS/Sprite/Card/CardBg2/T") + CardData->ItemName + ".T" + CardData->ItemName + "'"));
		}
	}

	_datatable->MarkPackageDirty();
#endif
}

FString UGameSystemFunction::GetObjectName(const UObject* Obj)
{
	return UKismetSystemLibrary::GetObjectName(Obj);
}

bool UGameSystemFunction::AddSphereTraceMulti(AActor* M_Owner, TArray<FHitResult>& OutActors, const FVector& _BeginOffset, const FVector& _EndOffset, float _Radius, ETraceTypeQuery _Type)
{

	if (!IsValid(M_Owner))
	{
		return false;
	}

	TArray<AActor*> Actors;
	if (UKismetSystemLibrary::SphereTraceMulti(
		M_Owner->GetWorld(),
		M_Owner->GetActorLocation() + _BeginOffset,
		M_Owner->GetActorLocation() + _EndOffset,
		_Radius, _Type, false, Actors, EDrawDebugTrace::ForDuration, OutActors, false
	))
	{
		return true;
	}

	return false;
}

bool UGameSystemFunction::AddSphereTrace(AActor* M_Owner, FHitResult& OutActor, const FVector& _BeginOffset, const FVector& _EndOffset, float _Radius, ETraceTypeQuery _Type)
{
	if (!M_Owner)
		return false;

	if (IsValid(M_Owner))
	{
		TArray<AActor*> Actors;
		if (UKismetSystemLibrary::SphereTraceSingle(
			M_Owner->GetWorld(),
			M_Owner->GetActorLocation() + _BeginOffset,
			M_Owner->GetActorLocation() + _EndOffset,
			_Radius,
			_Type, false, Actors, EDrawDebugTrace::ForDuration, OutActor, false)
			)
		{
			return true;
		}
	}

	return false;
}

bool UGameSystemFunction::AddLineTrance(
	const UWorld* World,
	const FVector& CurLoca,
	const FVector& BeginOffset,
	const FVector& EndOffset,
	ECollisionChannel CC,
	UObject* InputObj,
	TFunctionRef<void(UObject* Obj, AActor* CurHit)> Func
)
{
	return UGameSystemFunction::AddLineTrance(World, CurLoca + BeginOffset, CurLoca + EndOffset, CC, InputObj, Func);
}

bool UGameSystemFunction::AddLineTrance(const UWorld* World, const FVector& Begin, const FVector& End, ECollisionChannel CC, UObject* InputObj, TFunctionRef<void(UObject* Obj, AActor* CurHit)> Func)
{
	//添加射线
	FHitResult Hit;

	if (UFVMGameInstance::GetDebug())
	{
		DrawDebugLine(World,
			Begin,
			End,
			FColor(1.f, 0.f, 0.f, 1.f), false, 5.f);
	}

	if (World->LineTraceSingleByChannel(Hit,
		Begin,
		End,
		CC))
	{
		Func(InputObj, Hit.GetActor());

		return true;
	}
	return false;
}

bool UGameSystemFunction::AddLineTranceMulti(
	const UWorld* World,
	const FVector& BeginLocation,
	const FVector& EndLocation,
	const TArray<TEnumAsByte<EObjectTypeQuery>> CC,
	UObject* InputObj,
	TFunctionRef<void(UObject* Obj, AActor* CurHit)> Func
)
{
	//添加射线
	TArray<FHitResult> OutHits;

	if (UFVMGameInstance::GetDebug())
	{
		DrawDebugLine(World,
			BeginLocation,
			EndLocation,
			FColor(1.f, 0.f, 0.f, 1.f), false, 5.f);
	}

	if (UKismetSystemLibrary::LineTraceMultiForObjects(
		World,
		BeginLocation, EndLocation, CC, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, OutHits, true
	))
	{
		for (const FHitResult& CurHit : OutHits)
		{
			Func(InputObj, CurHit.GetActor());
		}

		return true;
	}
	return false;
}

void UGameSystemFunction::InitMouseMeshe(class UBoxComponent* Box, USceneComponent* BoxChildComp, FVector2D BoxRange, FVector2D Offset, bool bBack)
{
	if (IsValid(Box))
	{
		//FVector CurLocation = BoxChildComp->GetRelativeLocation();

		if (!IsValid(BoxChildComp))
		{
			return;
		}

		if (bBack)
		{
			Box->SetBoxExtent(FVector(20.f, BoxRange.X, BoxRange.Y), true);
			Box->AddLocalOffset(FVector(0.f, Offset.X, Offset.Y));
			BoxChildComp->AddLocalOffset(FVector(0.f, -Offset.X, -Offset.Y));
		}
		else {
			Box->SetBoxExtent(FVector(BoxRange.X, 20.f, BoxRange.Y), true);
			Box->AddLocalOffset(FVector(Offset.X, 0.f, Offset.Y));
			BoxChildComp->AddLocalOffset(FVector(-Offset.X, 0.f, -Offset.Y));
		}


		if (UFVMGameInstance::GetDebug())
		{
			Box->SetHiddenInGame(false);
			BoxChildComp->SetHiddenInGame(false);
		}
	}
}

void UGameSystemFunction::InintMouseCollisonByBox(class UBoxComponent* Box, FVector2D BoxRange /*= FVector2D(20.f)*/, bool bBack /*= false*/)
{
	if (IsValid(Box))
	{
		if (bBack)
		{
			Box->SetBoxExtent(FVector(20.f, BoxRange.X, BoxRange.Y), true);
		}
		else {
			Box->SetBoxExtent(FVector(BoxRange.X, 20.f, BoxRange.Y), true);
		}

		if (UFVMGameInstance::GetDebug())
		{
			Box->SetHiddenInGame(false);
		}
	}
}

void UGameSystemFunction::InintMouseCollisonBySphere(class USphereComponent* Sphere, float Radius)
{
	if (IsValid(Sphere))
	{
		Sphere->SetSphereRadius(Radius);

		if (UFVMGameInstance::GetDebug())
		{
			Sphere->SetHiddenInGame(false);
		}
	}
}

AActor* UGameSystemFunction::CreateIceBlock(AActor* M_Owner, const FVector& _Offset, const int32& _Layer, float _Time)
{
	if (IsValid(M_Owner))
	{
		//创建冰块图层
		ASpriteActor* Obj = M_Owner->GetWorld()->SpawnActor<ASpriteActor>(
			LoadClass<ASpriteActor>(0, TEXT("Class'/Script/FVM.SpriteActor'")));
		//Obj->SetTranslucency(8000);
		Obj->SetLifeSpan(_Time);
		Obj->SetSprite(LoadObject<UPaperSprite>(0, TEXT("PaperSprite'/Game/Resource/Texture/Sprite/VS/Sprite/Items/19/t/T_public_Sprite.T_public_Sprite'")));
		Obj->SetSpriteOffset(_Offset);
		Obj->SetSpriteLocation(M_Owner->GetActorLocation(), true);
		Obj->SetTranslucency(_Layer);
		return Obj;
	}
	return nullptr;
}

void UGameSystemFunction::CreateMapMatrix(FLine _Line, int32 _RowCount, int32 _ColCount, FVector _Offset, FString _FlipBookAnimPath)
{
	UPaperFlipbook* Anim = LoadObject<UPaperFlipbook>(0, *_FlipBookAnimPath);

	if (!Anim)
		return;

	int32 MaxRows = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMeshRowAndCol().Row;
	int32 MaxCols = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMeshRowAndCol().Col;

	//确定初始索引(确定偏移)
	int32 LInitRow = _Line.Row - _RowCount / 2;
	for (int32 i = 0; i < _ColCount; i++, LInitRow++)
	{
		//确定初始索引(确定偏移)
		int32 LInitCol = _Line.Col - _ColCount / 2;
		for (int32 j = 0; j < _ColCount; j++, LInitCol++)
		{
			if (LInitRow >= 0 && LInitCol >= 0)
				if (LInitRow < MaxRows && LInitCol < MaxCols)
				{

					AMapMeshe* Mesh = AGameMapInstance::GetGameMapInstance()->
						GetMesheControllComponent()->GetMapMesh(LInitRow, LInitCol);

					FVector Location = AGameMapInstance::GetGameMapInstance()->
						GetMesheControllComponent()->GetMapMeshLocation(LInitRow, LInitCol);

					AGameActorFlipbookBase* AAnims = Mesh->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
						LoadClass<AGameActorFlipbookBase>(0,
							TEXT("Class'/Script/FVM.GameActorFlipbookBase'")));
					AAnims->SetRenderLayer(8000);
					AAnims->InitRotation();
					AAnims->GetMyActor()->SetFlipbook(Anim);
					AAnims->SetAnimationPlayEndDestroy();
					AAnims->SetActorLocation(Location + _Offset);
				}
		}
	}
}




bool UGameSystemFunction::HitMouse(UObject* CurHitMouseObj, float _HitValue, AMouseActor* _Mouse, FGameBuffInfor& _Buff, EFlyItemAttackType AttackType)
{
	if (IsValid(_Mouse))
	{
		//设置老鼠状态->被击中
		_Mouse->SetbIsHurt(true);
		//被命中传入伤害数值
		_Mouse->BeHit(CurHitMouseObj, _HitValue, AttackType);
		//解析Buff信息
		_Mouse->ParseBuff_Information(_Buff);
		return true;
	}
	return false;
}

AActor* UGameSystemFunction::CreateCheckMouseLineTrace(class UWorld* World, const TArray<FLineTraceSetting>& InputLineTraceSetting, ELineTraceType& OutCheckLineType)
{
	//线路检测【如果发现老鼠则返回】
	//如果检测到老鼠，符合条件直接返回当前老鼠

	//检测
	auto TempCheck = [&](const FLineTraceSetting& Line, const ELineTraceType& Type, AActor*& OutMouseActor)->bool {
		//创建一次单通道的射线检测
		return UGameSystemFunction::CheckMouseLineType(World,
			Line.M_BeginLocation, Line.M_EndLocation,
			UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(Type),
			OutMouseActor);
		};

	//当前老鼠
	AActor* CurMouse = nullptr;
	for (const auto& Line : InputLineTraceSetting)
	{
		DrawDebugLine(World, Line.M_BeginLocation, Line.M_EndLocation, FColor::Red);

		//获取当前的线路碰撞
		ECollisionChannel CurCollision = UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(Line.M_ELineTraceType);
		//检测线路
		if (TempCheck(Line, Line.M_ELineTraceType, CurMouse))
		{
			//如果当前线路检测成功有老鼠，并且是一个不确定的类型
			if (CurCollision == ECollisionChannel::ECC_GameTraceChannel9)
			{
				//进一步检测
				switch (Line.M_ELineTraceType)
				{
					//陆地空中混合
				case ELineTraceType::E_MouseGroundAndSky: {
					//优先检测空中的老鼠
					if (TempCheck(Line, ELineTraceType::E_MouseSky, CurMouse))
					{
						OutCheckLineType = ELineTraceType::E_MouseSky;
						//如果检测成功
						return CurMouse;
					}
					//检测陆地老鼠
					if (TempCheck(Line, ELineTraceType::E_MouseGround, CurMouse))
					{
						OutCheckLineType = ELineTraceType::E_MouseGround;
						//如果检测成功
						return CurMouse;
					}
				}; break;
				case ELineTraceType::E_MouseVisibility: {
					OutCheckLineType = ELineTraceType::E_MouseVisibility;
					return CurMouse;
				}break;
				}
			}
			else {
				OutCheckLineType = Line.M_ELineTraceType;
				//返回一个确定的类型老鼠
				return CurMouse;
			}
		}
	}

	return nullptr;
}

bool UGameSystemFunction::CheckMouseLineType(UWorld* World, const FVector& BeginLoc, const FVector& EndLoc, const ECollisionChannel& Type, AActor*& OutMouse)
{
	FHitResult TraceResult;
	FCollisionQueryParams TraceParams;
	FCollisionResponseParams TraceResParams;

	if (World->LineTraceSingleByChannel(
		TraceResult,
		BeginLoc,
		EndLoc,
		Type,
		TraceParams,
		TraceResParams))
	{
		OutMouse = TraceResult.GetActor();
		return true;
	}

	return false;
}

ETraceTypeQuery UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType _EMouseCollisionType)
{
	switch (_EMouseCollisionType)
	{
		//陆地
	case EMouseCollisionType::MouseGround:
		return ETraceTypeQuery::TraceTypeQuery4;
		//空中
	case EMouseCollisionType::MouseSky:
		return ETraceTypeQuery::TraceTypeQuery5;
		//地下
	case EMouseCollisionType::MouseUnder:
		return ETraceTypeQuery::TraceTypeQuery6;
		//水上
	case EMouseCollisionType::MouseOnWater:
		return ETraceTypeQuery::TraceTypeQuery12;
	default:
		return ETraceTypeQuery::TraceTypeQuery11;
	}
	return ETraceTypeQuery::TraceTypeQuery11;
}

ECollisionChannel UGameSystemFunction::GetMouseCollisionBoxTypeByELineType(ELineType _EMouseLineType)
{
	switch (_EMouseLineType)
	{
	case ELineType::OnGround:return ECollisionChannel::ECC_GameTraceChannel2; break;
	case ELineType::Sky:return ECollisionChannel::ECC_GameTraceChannel3; break;
	case ELineType::Underground:return ECollisionChannel::ECC_GameTraceChannel4; break;
	case ELineType::OnWater:return ECollisionChannel::ECC_GameTraceChannel10; break;
	default:
		return ECollisionChannel::ECC_GameTraceChannel9;
	}

	return ECollisionChannel::ECC_GameTraceChannel9;
}

ETraceTypeQuery UGameSystemFunction::GetMouseCollisionTraceTypeByELineType(ELineType _EMouseLineType)
{
	switch (_EMouseLineType)
	{
	case ELineType::OnGround:return ETraceTypeQuery::TraceTypeQuery4;; break;
	case ELineType::Sky:return ETraceTypeQuery::TraceTypeQuery5; break;
	case ELineType::Underground:return ETraceTypeQuery::TraceTypeQuery6; break;
	case ELineType::OnWater:return ETraceTypeQuery::TraceTypeQuery12; break;
	default:
		return ETraceTypeQuery::TraceTypeQuery11;
	}

	return ETraceTypeQuery::TraceTypeQuery11;
}

ECollisionChannel UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(ELineTraceType _EMouseLineType)
{
	switch (_EMouseLineType)
	{
	case ELineTraceType::E_MouseGround:return ECollisionChannel::ECC_GameTraceChannel2; break;
	case ELineTraceType::E_MouseSky:return ECollisionChannel::ECC_GameTraceChannel3; break;
	case ELineTraceType::E_MouseUnder:return ECollisionChannel::ECC_GameTraceChannel4; break;
	case ELineTraceType::E_MouseWater:return ECollisionChannel::ECC_GameTraceChannel10; break;
	default:
		return ECollisionChannel::ECC_GameTraceChannel9;
		break;
	}

	return ECollisionChannel::ECC_GameTraceChannel9;
}

ETraceTypeQuery UGameSystemFunction::GetMouseCollisonTraceTypeByLineType(ELineTraceType _EMouseLineType)
{
	return UEngineTypes::ConvertToTraceType(UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(_EMouseLineType));
}

ETraceTypeQuery UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType _ECardCollisionType)
{
	switch (_ECardCollisionType)
	{
	case ECardCollisionType::E_CardActor2:
		return ETraceTypeQuery::TraceTypeQuery9;

	case ECardCollisionType::E_CardActor3:
		return ETraceTypeQuery::TraceTypeQuery10;
	default:
		return ETraceTypeQuery::TraceTypeQuery3;
	}
	return ETraceTypeQuery::TraceTypeQuery3;
}

ECollisionChannel UGameSystemFunction::GetCardCollisionBoxType(ECardCollisionType _ECardCollisionType)
{
	switch (_ECardCollisionType)
	{
	case ECardCollisionType::E_CardActor2:
		return ECollisionChannel::ECC_GameTraceChannel7;

	case ECardCollisionType::E_CardActor3:
		return ECollisionChannel::ECC_GameTraceChannel8;
	default:
		return ECollisionChannel::ECC_GameTraceChannel1;
	}
	return ECollisionChannel::ECC_GameTraceChannel1;
}

void UGameSystemFunction::CalculateLineTracePosition(const FVector& _Location, const FLine& _Line, UUI_MapMeshe* _UI_Meshe, const FLineTraceSetting& _LineTraceSetting, TArray<FLineTraceSetting>& _LineTraceSettingOut)
{
	//基本坐标 
	FVector BaseLocation = _Location;
	BaseLocation.X -= 5.f;

	float TargetLY = _UI_Meshe->GetMapMeshCurrentLeft(_Line.Row).Y - 55.f;
	float TargetRY = _UI_Meshe->GetMapMeshCurrentRight(_Line.Row).Y + 55.f;
	FVector Top = _UI_Meshe->GetMapMeshCurrentTop(_Line.Col);
	FVector Bottom = _UI_Meshe->GetMapMeshCurrentBottom(_Line.Col);
	float GridCountMax = _LineTraceSetting.M_GridCount * (55.f);

	FLineTraceSetting _Item;
	_Item.M_ELineTraceType = _LineTraceSetting.M_ELineTraceType;

	switch (_LineTraceSetting.M_ELineTracePosition)
	{
	case ELineTracePosition::E_Left:
	{
		FVector LocationEnd = FVector(BaseLocation.X, TargetLY, BaseLocation.Z);
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = LocationEnd;
	}
	break;
	case ELineTracePosition::E_Right:
	{
		FVector LocationEnd = FVector(BaseLocation.X, TargetRY, BaseLocation.Z);

		_Item.M_BeginLocation = BaseLocation;

		_Item.M_EndLocation = _LineTraceSetting.M_bEnableSettingGridCount ? BaseLocation + FVector(0.f, GridCountMax, 0.f) : LocationEnd;

		if (_Item.M_EndLocation.Y > LocationEnd.Y)
			_Item.M_EndLocation = LocationEnd;

	}
	break;
	case ELineTracePosition::E_LeftRight_M:
	{
		FVector LocationBegin = FVector(BaseLocation.X, TargetLY, BaseLocation.Z);
		FVector LocationEnd = FVector(BaseLocation.X, TargetRY, BaseLocation.Z);
		_Item.M_BeginLocation = LocationBegin;
		_Item.M_EndLocation = LocationEnd;
	}
	break;
	case ELineTracePosition::E_Top:
	{
		Top.X = BaseLocation.X;
		Top.Y = BaseLocation.Y;
		Top.Z += 45.f;

		_Item.M_BeginLocation = BaseLocation;
		//_Item.M_EndLocation = Top;
		_Item.M_EndLocation = _LineTraceSetting.M_bEnableSettingGridCount ? BaseLocation + FVector(0.f, 0.f, GridCountMax) : Top;

	}
	break;
	case ELineTracePosition::E_Bottom:
	{
		Bottom.X = BaseLocation.X;
		Bottom.Y = BaseLocation.Y;
		Bottom.Z -= 45.f;

		_Item.M_BeginLocation = BaseLocation;
		//_Item.M_EndLocation = Bottom;
		_Item.M_EndLocation = _LineTraceSetting.M_bEnableSettingGridCount ? BaseLocation + FVector(0.f, 0.f, -GridCountMax) : Bottom;

	}
	break;
	case ELineTracePosition::E_TopBottom_M:
	{
		Top.X = BaseLocation.X;
		Top.Y = BaseLocation.Y;
		Top.Z += 45.f;
		Bottom.X = BaseLocation.X;
		Bottom.Y = BaseLocation.Y;
		Bottom.Z -= 45.f;
		_Item.M_BeginLocation = Top;
		_Item.M_EndLocation = Bottom;
	}
	break;
	}

	//加上偏移量
	_Item.M_BeginLocation += _LineTraceSetting.M_LineTraceOffset;
	_Item.M_EndLocation += _LineTraceSetting.M_LineTraceOffset;

	//UE_LOG(LogTemp, Error, TEXT("%.2f  %.2f %.2f"), _LineTraceSetting.M_LineTraceOffset.X, _LineTraceSetting.M_LineTraceOffset.Y, _LineTraceSetting.M_LineTraceOffset.Z);

	_LineTraceSettingOut.Emplace(_Item);
}

void UGameSystemFunction::GetCurveLineByTimeLineBlue(FVector CurLocation, FVector TargetLocation, float MaxHight, const float& TimeTick, const float& HightTick, FVector& OutLocation)
{
	UGameSystemFunction::GetCurveLineByTimeLine(CurLocation, TargetLocation, MaxHight, TimeTick, HightTick, OutLocation);
}

void UGameSystemFunction::GetCurveLineByTimeLine(const FVector& CurLocation, const FVector& TargetLocation, const float& MaxHight, const float& TimeTick, const float& HightTick, FVector& OutLocation)
{
	FVector A = CurLocation;
	A.Z = HightTick * MaxHight + CurLocation.Z;

	FVector B = TargetLocation;
	B.Z = HightTick * MaxHight + TargetLocation.Z;

	OutLocation = UKismetMathLibrary::VLerp(A, B, TimeTick);
}

void UGameSystemFunction::SetGlobalGameTime(const UObject* WorldContextObject, float _TimeRate)
{
	UGameplayStatics::SetGlobalTimeDilation(WorldContextObject, _TimeRate);
}

void UGameSystemFunction::GenerateMatrixByPoint(
	UMesheControllComponent* Comp,
	FLine PlayerLine,
	int32 Rate,
	TArray<FLine>& OutLines
)
{
	if (Rate == 0)
	{
		OutLines.Add(PlayerLine);
		return;
	}

	FLine ColAndRow(6, 10);
	if (IsValid(Comp))
	{
		ColAndRow = Comp->GetMapMeshRowAndCol();
	}

	//从中心点减去，得到左上角的起点位置
	FLine Begin;
	Begin.Row = PlayerLine.Row - Rate;
	Begin.Col = PlayerLine.Col - Rate;

	//从中心点减去，得到右下角的终点位置
	FLine End;
	End.Row = PlayerLine.Row + Rate;
	End.Col = PlayerLine.Col + Rate;

	//计算起点行有效位置
	if (Begin.Row < 0)
	{
		Begin.Row = 0;
	}

	//计算起点列位置
	if (Begin.Col < 0)
	{
		Begin.Col = 0;
	}

	//计算终点行有效位置
	if (End.Row >= ColAndRow.Row)
	{
		End.Row = ColAndRow.Row - 1;
	}

	//计算终点列位置
	if (End.Col >= ColAndRow.Col)
	{
		End.Col = ColAndRow.Col - 1;
	}

	OutLines.Reset((End.Row + 1) * (End.Col + 1));
	for (int32 Row = Begin.Row; Row <= End.Row; Row++)
	{
		for (int32 Col = Begin.Col; Col <= End.Col; Col++)
		{
			OutLines.Add(FLine(Row, Col));
		}
	}
}

void UGameSystemFunction::PlayerHitRangeAlienByMapMouseManager(
	FLine PlayerLine,
	int32 Rate,
	FGameBuffInfor Buffs,
	ELineType AlienLineType,
	float ATK,
	UMesheControllComponent* ControllComponent
)
{
	if (IsValid(ControllComponent))
	{
		FLine ColAndRow = ControllComponent->GetMapMeshRowAndCol();

		//从中心点减去，得到左上角的起点位置
		FLine Begin;
		Begin.Row = PlayerLine.Row - Rate;
		Begin.Col = PlayerLine.Col - Rate;

		//从中心点减去，得到右下角的终点位置
		FLine End;
		End.Row = PlayerLine.Row + Rate;
		End.Col = PlayerLine.Col + Rate;

		//计算起点行有效位置
		if (Begin.Row < 0)
		{
			Begin.Row = 0;
		}

		//计算起点列位置
		if (Begin.Col < 0)
		{
			Begin.Col = 0;
		}

		//计算终点行有效位置
		if (End.Row >= ColAndRow.Row)
		{
			End.Row = ColAndRow.Row - 1;
		}

		//计算终点列位置
		if (End.Col >= ColAndRow.Col)
		{
			End.Col = ColAndRow.Col - 1;
		}

		for (int32 Row = Begin.Row; Row <= End.Row; Row++)
		{
			for (int32 Col = Begin.Col; Col <= End.Col; Col++)
			{
				AMapMouseMesheManager* Map = ControllComponent->GetMapMouseMesh(
					Row,
					Col
				);
				const TMap<FString, AMouseActor*>& Curs = Map->GetCurMouseCopy();
				for (const auto& CurAlien : Curs)
				{
					if (IsValid(CurAlien.Value) && CurAlien.Value->GetMouseLineType() == AlienLineType)
					{
						if (CurAlien.Value->BeHit(ControllComponent, ATK, EFlyItemAttackType::Def))
						{
							CurAlien.Value->SetbIsHurt(true);
							CurAlien.Value->ParseBuff_Information(Buffs);

							if (UFVMGameInstance::GetDebug())
							{
								UGameSystemFunction::FVMLog(
									__FUNCTION__,
									UGameSystemFunction::GetObjectName(CurAlien.Value) +
									TEXT("受到伤害：") + FString::SanitizeFloat(ATK)
								);
							}
						}
					}
				}
			}
		}
	}
}

void UGameSystemFunction::SetCaptureLook(const UObject* WorldContextObject, EMouseLockMode Lock)
{
	FInputModeGameAndUI GameUI;
	GameUI.SetHideCursorDuringCapture(false);
	GameUI.SetLockMouseToViewportBehavior(Lock);
	UGameplayStatics::GetPlayerController(WorldContextObject, 0)->SetInputMode(GameUI);
}

//发送补偿邮件
void UGameSystemFunction::SendVersionCompensate()
{
	//获取存档
	if (UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager())
	{
		TArray<FPlayerVersionCompensate_Data*> Datas;
		//获取数据表
		UGameSystemFunction::GetDataTableRows<FPlayerVersionCompensate_Data>(
			TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/GameVersionCompensate.GameVersionCompensate'"),
			Datas);

		//获取总数量
		int32 DataCount = Datas.Num();
		//获取背包存储数量
		int32 Count = Player->M_GameVersionCompensate.Num();

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("----------------------------------------------------------------")));

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前邮件数量：") + FString::FromInt(Count)) + TEXT("总量：") + FString::FromInt(DataCount));

		//防止出现版本更新时-已经领取的补偿-重复领取
		for (auto& Item : Player->M_GameVersionCompensate)
		{
			Item.M_SendState = true;
		}

		//计算需要添加多少补偿
		int32 Result = DataCount - Count;

		if (Result > 0)
		{

			if (UFVMGameInstance::GetDebug())
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("需要补发数量：") + FString::FromInt(Result)));

			for (int i = Count; i < DataCount; i++)
			{
				FPlayerVersionCompensate Data = Datas[i]->M_FPlayerVersionCompensate;
				Player->M_GameVersionCompensate.Emplace(Data);
			}

			for (auto& Item : Player->M_GameVersionCompensate)
			{
				if (!Item.M_SendState)
				{
					if (UGameSystemFunction::SendMailToPlayerMailBox(Item.M_SendMailName))
					{
						if (UFVMGameInstance::GetDebug())
							UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("发送补偿方案：") + Item.M_SendMailName));

						Item.M_SendState = true;
					}
				}
				else {
					if (UFVMGameInstance::GetDebug())
						UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前补偿方案失效：") + Item.M_SendMailName));
				}
			}
		}

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("----------------------------------------------------------------")));

		//保存
		//UGameSystemFunction::SaveCurrentPlayerData();
	}
}

//发送邮件给玩家
bool UGameSystemFunction::SendMailToPlayerMailBox(const FString& _Name)
{
	FMail _MailData;

	TArray<FMail> Mail_;
	UEquipmentDataAssetCache* Cache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);
	UEquipmentBaseStruct::GetEquipmentRowDatas<FEquipment_FMail_Data, FMail>(Cache->GetMail(), Mail_);

	if (UEquipmentBaseStruct::GetMailArraysData(_Name, Mail_, _MailData))
	{
		UGameSystemFunction::SendMailToCurrentPlayer(_MailData);
		//保存数据
		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("邮件发送，发送信息：")) + _Name);

		return true;
	}
	return false;
}

//获取指定技能书的等级
int32 UGameSystemFunction::GetPlayerSkillBooksLv(const FString& _Name)
{
	for (const auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas)
	{
		if (_Name.Equals(Item.M_Name))
			return Item.M_LV;
	}
	return 0;
}

//想背包发送指定的防御卡
bool UGameSystemFunction::SendCardToPlayerBag(const FString& _CardName, int32 _Lv)
{
	FItemCard CardData;
	if (UCardBaseStruct::SearchCardFromDataTable(_CardName, CardData))
	{
		CardData.M_CardGrade = _Lv;
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card.Emplace(CardData);

		UGameLogSubsystem::AddPlayerGetCardLog(_CardName, _Lv);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送防御卡：") + _CardName + TEXT("到背包"));
		}

		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("发送防御卡：")) + _CardName + TEXT("到背包"));

		return true;
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送防御卡失败：") + _CardName);
	}

	return false;
}

//发送指定装备到背包
bool UGameSystemFunction::SendEquipmentToPlayerBag(const FString& _EquipmentName, int32 _Count, bool _bSave)
{
	if (_Count > 1000000000)
		_Count = 1000000000;

	if (_Count < 1)
		_Count = 1;

	FEquipmentBase EquipmentData;

	if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(_EquipmentName, EquipmentData))
	{
		//当前物品是可以叠加的道具
		if (EquipmentData.M_IsOverlap)
		{
			//查询结果
			bool LResult = false;

			//背包查询->是否有重复的->如果有则直接添加对应个数
			for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment)
			{
				if (_EquipmentName.Equals(Item.ItemName.ToString()))
				{
					//根据类型发送背包
					if (Item.M_IsOverlap)
					{
						if (Item.M_Count + _Count > 9999)
							Item.M_Count = 9999;

						Item.M_Count += _Count;
					}
					LResult = true;
					break;
				}
			}

			if (!LResult)
			{
				//根据类型发送背包
				EquipmentData.M_Count = _Count;
				//申请ID
				UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(EquipmentData);
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment.Emplace(EquipmentData);
			}
		}
		else {
			for (int32 i = 0; i < _Count; i++)
			{
				//申请ID
				UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(EquipmentData);
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment.Emplace(EquipmentData);
			}
		}

		//添加装备内容
		UGameLogSubsystem::AddPlayerGetEquipmentLog(EquipmentData.ItemName.ToString(), _Count, EquipmentData.M_EquipmentGrade);

		if (_bSave)
		{
			UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("发送装备：")) + EquipmentData.ItemName.ToString() + TEXT("到背包"));
		}

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送装备：") + EquipmentData.ItemName.ToString() + TEXT("到背包"));
		}

		return true;
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送装备失败：") + EquipmentData.ItemName.ToString());
	}

	return false;
}

//发送指定材料到背包
bool UGameSystemFunction::SendMaterialToPlayerBag(const FString& _MaterialName, int32 _Count, bool _bSave)
{
	if (_Count > 1000000000)
		_Count = 1000000000;

	if (_Count < 1)
		_Count = 1;

	FMaterialBase MaterialBaseData;
	if (UMaterialBaseStruct::SearchMaterailFromDataTable(_MaterialName, MaterialBaseData))
	{
		//查询结果
		bool LResult = false;
		//背包查询->是否有重复的->如果有则直接添加对应个数
		for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
		{
			if (_MaterialName.Equals(Item.ItemName.ToString()))
			{
				//个数判断
				if (Item.M_Count + _Count > 1000000000)
				{
					Item.M_Count = 1000000000;
				}
				else {
					Item.M_Count += _Count;
				}

				LResult = true;
				break;
			}
		}

		//查询失败
		if (!LResult)
		{
			//未查询到
			MaterialBaseData.M_Count = _Count;
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material.Emplace(MaterialBaseData);
		}

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送材料：") + MaterialBaseData.ItemName.ToString() + TEXT("到背包"));
		}

		UGameLogSubsystem::AddPlayerGetMaterialLog(MaterialBaseData.ItemName.ToString(), _Count);

		//保存数据
		if (_bSave)
		{
			UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("发送材料：")) + MaterialBaseData.ItemName.ToString() + TEXT("到背包"));
		}

		return true;
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("发送材料失败：") + MaterialBaseData.ItemName.ToString());
	}

	return false;
}

//获取指向玩家存档卡片的数据(建立在加载存档之后)
void UGameSystemFunction::GetPlayerCardDatas(TArray<FItemCard*>& _datas, int32 _Count)
{
	int32 _Num_ = 0;
	for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card)
	{
		_datas.Emplace(&Item);

		if (_Count > 0)
		{
			_Num_++;
			if (_Num_ >= _Count)
				break;
		}

	}
}

//获取玩家存档卡片数据的引用(建立在加载存档之后)
TArray<FItemCard>& UGameSystemFunction::GetPlayerCardDatasByRef()
{
	return UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card;
}

int32 UGameSystemFunction::SerachPlayerEquipmentIndexByID(const FString& _ItemName, int32 _EquipID, uint8 _SerarchType)
{
	UPlayerStructManager* LPlayer = UFVMGameInstance::GetPlayerStructManager_Static();

	//2分查找  找出ID
	int32 LIndex = (LPlayer->M_PlayerItems_Equipment.Num() / 2) - 1;

	if (LIndex > _EquipID && _SerarchType == 0 || _SerarchType == 1u)
	{
		//通过前进行查找
		for (int32 I = 0; I <= LIndex; ++I)
		{
			if (LPlayer->M_PlayerItems_Equipment[I].M_ItemID == _EquipID && _ItemName.Equals(LPlayer->M_PlayerItems_Equipment[I].ItemName.ToString()))
			{
				//查询成功
				return I;
			}
		}
	}

	if (LIndex < _EquipID && _SerarchType == 0 || _SerarchType == 2u)
	{
		//通过后面进行查找
		for (int32 I = LIndex; I < LPlayer->M_PlayerItems_Equipment.Num(); ++I)
		{
			if (LPlayer->M_PlayerItems_Equipment[I].M_ItemID == _EquipID && _ItemName.Equals(LPlayer->M_PlayerItems_Equipment[I].ItemName.ToString()))
			{
				//查询成功
				return I;
			}
		}

	}

	//2分的结果刚好就是当前ID
	if (LPlayer->M_PlayerItems_Equipment[LIndex].M_ItemID == _EquipID && _ItemName.Equals(LPlayer->M_PlayerItems_Equipment[LIndex].ItemName.ToString()))
	{
		//查询成功
		return LIndex;
	}

	return -1;
}

FString UGameSystemFunction::GetItemGradePath(int32 _Grade)
{
	if (_Grade >= 1 && _Grade <= 16)
		return FString(TEXT("Texture2D'/Game/Resource/Texture/CardGrade/") + FString::FromInt(_Grade) + TEXT(".") + FString::FromInt(_Grade) + TEXT("'"));

	return FString();
}

void UGameSystemFunction::AddString(FString& _CurrentString, const FString& _Content)
{
	if (!_CurrentString.Contains(_Content))
		_CurrentString += _Content;
}


bool UGameSystemFunction::CheckStringIsValid(const FString str, const FString Reg)
{
	FRegexPattern Pattern(Reg);
	FRegexMatcher regMatcher(Pattern, str);
	regMatcher.SetLimits(0, str.Len());
	return regMatcher.FindNext();
}

//删除玩家背包中有待删除的道具->争对装备道具【会改变数组的内存地址】自动调用RemoveEquipmentFromPlayerBag
void UGameSystemFunction::ClearWaitingItemsForEquip(TArray<FEquipmentBase>& _Arrays)
{
	TArray<FEquipmentBase> _Buff;

	for (FEquipmentBase& _Items : _Arrays)
	{
		//将未标记的物品添加到缓冲区(满足条件的物品将得到保留，不需要销毁ID)
		if (!_Items.bWaitRemove)
			_Buff.Emplace(_Items);
		else {
			//销毁ID号
			UFVMGameInstance::GetPlayerStructManager_Static()->RemoveEquipmentFromPlayerBag(_Items);
		}
	}
	//清空
	_Arrays.Empty();
	//重新复制新值
	_Arrays = _Buff;
}

AMouseActor* UGameSystemFunction::LockingAttackComponentCheckAlien(
	UMouseManagerComponent* AlienManager,
	int32 CurRow,
	const TSet<ELineTraceType>& LType
)
{
	AMouseActor* CurrentAlien = nullptr;

	auto CheckAlien = [&](UMouseLineManager* AlienManagerComp) -> bool {
		for (const ELineTraceType& Type : LType)
		{
			//优先检测空中
			switch (Type)
			{
			case ELineTraceType::E_MouseSky:
			{
				if (AlienManagerComp->GetMouseSky().Num())
				{
					CurrentAlien = AlienManagerComp->GetMouseTopBySky();
					return true;
				}
			}break;
			case ELineTraceType::E_MouseGroundAndSky:
			{
				if (AlienManagerComp->GetMouseSky().Num())
				{
					CurrentAlien = AlienManagerComp->GetMouseTopBySky();
					return true;
				}

				if (AlienManagerComp->GetMouseGround().Num())
				{
					CurrentAlien = AlienManagerComp->GetMouseTopByGorund();
					return true;
				}
			}break;
			case ELineTraceType::E_MouseGround:
			{
				if (AlienManagerComp->GetMouseGround().Num())
				{
					CurrentAlien = AlienManagerComp->GetMouseTopByGorund();
					return true;
				}
			}break;
			case ELineTraceType::E_MouseUnder:
			{
				if (AlienManagerComp->GetMouseUnderGround().Num())
				{
					CurrentAlien = AlienManagerComp->GetMouseTopByUnderGorund();
					return true;
				}
			}
			break;
			default:
				CurrentAlien = AlienManagerComp->GetMouseTopByAllType();
				return true;
			}
		}
		return false;
		};

	auto CheckLineAlien = [&](int32 CurRow) -> bool {
		UMouseLineManager* AlienLine = AlienManager->GetMouseLineManager(CurRow);
		if (AlienLine->GetMouseExist())
		{
			if (CheckAlien(AlienLine))
			{
				return true;
			}
		}

		return false;
		};


	//查询结果
	bool CheckResult = CheckLineAlien(CurRow);
	if (CheckResult)
	{
		return CurrentAlien;
	}
	else {
		TArray<int32> Rows;
		for (int32 i = 0; i < AGameMapInstance::GetGameMapInstance()->
			M_MesheControllComponent->GetMapMeshRowAndCol().Row; i++)
		{
			if (i != CurRow)
			{
				if (CheckLineAlien(i))
				{
					Rows.Emplace(i);
				}
			}
		}

		if (Rows.Num())
		{
			int32 RanRow = UGameSystemFunction::GetRandomRange(0, Rows.Num() - 1);

			if (CheckLineAlien(Rows[RanRow]))
			{
				return CurrentAlien;
			}
		}

	}

	return nullptr;
}


class UWidgetBase* UGameSystemFunction::GetUserInterWidgetByClass(TSoftClassPtr<class UAssetCategoryName> ObjectType, FName Name)
{
	UGameUserInterfaceSubsystem* UIStaticClass = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();
	if (IsValid(UIStaticClass))
	{
		const FName& CurName = UIStaticClass->GetUserInterCategoryName(ObjectType);
		return UIStaticClass->GetUserInterInstance(CurName)->GetUI(Name);
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(
			__FUNCTION__,
			FString(TEXT("UI子系统无效"))
		);
	}

	return nullptr;
}

class UWidgetBase* UGameSystemFunction::GetUserInterWidgetByName(const FName& CategoryName, const FName& Name)
{
	UGameUserInterfaceSubsystem* UIStaticClass = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();
	if (IsValid(UIStaticClass))
	{
		return UIStaticClass->GetUserInterInstance(CategoryName)->GetUI(Name);
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(
			__FUNCTION__,
			FString(TEXT("UI子系统无效"))
		);
	}

	return nullptr;
}

class UClass* UGameSystemFunction::GetUserInterClassByClass(TSoftClassPtr<class UAssetCategoryName> ObjectType, FName Name)
{
	UGameUserInterfaceSubsystem* UIStaticClass = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();
	if (IsValid(UIStaticClass))
	{
		const FName& CurName = UIStaticClass->GetUserInterCategoryName(ObjectType);
		return UIStaticClass->GetUserInterInstance(CurName)->GetUIClass(Name);
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(
			__FUNCTION__,
			FString(TEXT("UI子系统无效"))
		);
	}
	return nullptr;
}

class UClass* UGameSystemFunction::GetUserInterClassByName(const FName& CategoryName, const FName& Name)
{
	UGameUserInterfaceSubsystem* UIStaticClass = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();
	if (IsValid(UIStaticClass))
	{
		return UIStaticClass->GetUserInterInstance(CategoryName)->GetUIClass(Name);
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(
			__FUNCTION__,
			FString(TEXT("UI子系统无效"))
		);
	}
	return nullptr;
}

UGameUserInterfaceInstance* UGameSystemFunction::GetUserInterInstanceByName(const FName& Name)
{
	UGameUserInterfaceSubsystem* UIStaticClass = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();
	if (IsValid(UIStaticClass))
	{
		return UIStaticClass->GetUserInterInstance(Name);
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(
			__FUNCTION__,
			FString(TEXT("UI子系统无效"))
		);
	}

	return nullptr;
}

bool UGameSystemFunction::AddTempUserInterInstanceByName(FName Name)
{
	UGameUserInterfaceSubsystem* UIStaticClass = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();
	if (IsValid(UIStaticClass))
	{
		return UIStaticClass->AddTempUserInterInstance(Name);
	}

	return false;
}

FName UGameSystemFunction::GetAssetCategoryName(TSoftClassPtr<UAssetCategoryName> ObjectType)
{
	UClass* NameClass = nullptr;
	if (!IsValid(ObjectType.Get()))
	{
		NameClass = ObjectType.LoadSynchronous();
	}
	else {
		NameClass = ObjectType.Get();
	}

	TSubclassOf<UAssetCategoryName> NameClassSource(NameClass);
	if (IsValid(NameClassSource.GetDefaultObject()))
	{
		return NameClassSource.GetDefaultObject()->GetCategoryName();
	}

	return FName();
}

void UGameSystemFunction::JsonToBinary(FString Json, TArray<uint8>& Binary)
{
	FTCHARToUTF8 Convert(*Json);
	TArray<uint8> output(reinterpret_cast<const uint8*>(Convert.Get()), Convert.Length());
	Binary = output;
}

void UGameSystemFunction::BinaryToJson(const TArray<uint8>& Binary, FString& Json)
{
	FUTF8ToTCHAR Convert(reinterpret_cast<const char*>(Binary.GetData()), Binary.Num());
	Json = Convert;
}

void UGameSystemFunction::BinaryToString(const TArray<uint8>& Binary, FString& BinaryString)
{
	BinaryString = FBase64::Encode(Binary);
}

void UGameSystemFunction::BinaryStringToBinary(FString BinaryString, TArray<uint8>& Binary)
{
	FBase64::Decode(BinaryString, Binary);
}

void UGameSystemFunction::SaveGameToBinary(class USaveGame* SaveGameObject, TArray<uint8>& OutSaveData)
{
	UGameplayStatics::SaveGameToMemory(SaveGameObject, OutSaveData);
}

void UGameSystemFunction::LoadBinaryToSaveGame(const TArray<uint8>& OutSaveData, class USaveGame*& SaveGameObject)
{
	SaveGameObject = UGameplayStatics::LoadGameFromMemory(OutSaveData);
}

UGameUserInterfaceInstance* UGameSystemFunction::GetUserInterInstanceByClass(TSoftClassPtr<class UAssetCategoryName> ObjectType)
{
	UGameUserInterfaceSubsystem* UIStaticClass = UGameUserInterfaceSubsystem::GetGameUserInterfaceSubsystemStatic();
	if (IsValid(UIStaticClass))
	{
		const FName& CurName = UIStaticClass->GetUserInterCategoryName(ObjectType);
		return UIStaticClass->GetUserInterInstance(CurName);
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(
			__FUNCTION__,
			FString(TEXT("UI子系统无效"))
		);
	}

	return nullptr;
}

UTimeLineClass* UTimeLineClass::MakeTimeLineClass()
{
	UTimeLineClass* Cur = NewObject<UTimeLineClass>();
	Cur->OnTimelineFloatEvent.BindUFunction(Cur, TEXT("TimeUpdate"));
	Cur->OnTimelineFinishedEvent.BindUFunction(Cur, TEXT("TimeFinish"));
	Cur->Timeline.SetTimelineFinishedFunc(Cur->OnTimelineFinishedEvent);
	return Cur;
}

void UTimeLineClass::AddCurve(
	TSoftObjectPtr<UCurveFloat> NewCurve,
	UObject* NewObj,
	TFunction<void(UTimeLineClass* TimeLine, UObject*, float)> NewUpdateFunc,
	TFunction<void(UTimeLineClass* TimeLine, UObject*)> NewFinishFunc
)
{
	UCurveFloat* Cur = NewCurve.Get();
	if (!IsValid(Cur))
	{
		Cur = UGameSystemFunction::LoadRes(NewCurve);
		if (!IsValid(Cur))
		{

			return;
		}
	}

	this->Timeline.AddInterpFloat(Cur, this->OnTimelineFloatEvent);
	this->Curves.Emplace(Cur);
	this->CurveFuncsUpdate.Emplace(NewUpdateFunc);
	this->CurveFuncsFinish.Emplace(NewFinishFunc);
	this->AllObjs.Emplace(NewObj);
}


void UTimeLineClass::Tick(const float& DeltaTime)
{
	this->Timeline.TickTimeline(DeltaTime);
}

void UTimeLineClass::Play()
{
	this->Timeline.Play();
	this->bStart = true;
}

void UTimeLineClass::Reverse()
{
	this->Timeline.Reverse();
	this->bStart = false;
}

void UTimeLineClass::PlayFromStart()
{
	this->Timeline.PlayFromStart();
	this->bStart = true;
}

void UTimeLineClass::ReverseFromEnd()
{
	this->Timeline.ReverseFromEnd();
	this->bStart = false;
}

void UTimeLineClass::Stop()
{
	this->Timeline.Stop();
}

void UTimeLineClass::End()
{
	if (this->GetStart())
	{
		this->PlayFromStart();
		this->Stop();
	}
	else {
		this->ReverseFromEnd();
		this->Stop();
	}
	this->TimeFinish();
}

bool UTimeLineClass::GetStart()
{
	return this->bStart;
}

void UTimeLineClass::TimeUpdate()
{
	for (int32 i = 0; i < Curves.Num(); i++)
	{
		if (IsValid(Curves[i]))
		{
			//执行
			this->CurveFuncsUpdate[i](this, this->AllObjs[i], Curves[i]->GetFloatValue(Timeline.GetPlaybackPosition()));
		}
	}
}

void UTimeLineClass::TimeFinish()
{
	for (int32 i = 0; i < Curves.Num(); i++)
	{
		if (IsValid(Curves[i]))
		{
			//执行
			this->CurveFuncsFinish[i](this, this->AllObjs[i]);
		}
	}
}
