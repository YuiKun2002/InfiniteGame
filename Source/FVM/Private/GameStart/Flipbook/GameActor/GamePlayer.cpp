// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/PlayerWeapon/PlayerFirstWeapon.h"

AGamePlayer::AGamePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AGamePlayer::InitMeshe(UUI_MapMeshe* _UUI_MapMeshe, AMapMeshe* _AMapMeshe)
{
	this->M_UUI_MapMeshe = _UUI_MapMeshe;
	this->M_AMapMeshe = _AMapMeshe;

	this->M_PlayerLine = _AMapMeshe->GetLine();
}

void AGamePlayer::SetPlayerSuit(FPlayerSuitItem SuitData)
{
	
}

void AGamePlayer::InitPlayerWeapon()
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	//加载武器(主武器)
	if (PlayerData->M_FPlayerSuit.M_PlayerWeapons.M_PlayerFirstWeapon.M_bUse)
	{
		this->LoadPlayerFirstWeapon(PlayerData->M_FPlayerSuit.M_PlayerWeapons.M_PlayerFirstWeapon.M_WeaponName,
			PlayerData->M_FPlayerSuit.M_PlayerWeapons.M_PlayerFirstWeapon.M_WeaponResource_C_Path
		);
	}
	else {
		//如果没有装备则使用默认武器
		this->LoadPlayerFirstWeapon(
			TEXT("小笼机枪"),
			TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Player/Weapon/First/BP_小笼机枪.BP_小笼机枪_C'")
		);
	}

	//加载武器(副武器)
	if (PlayerData->M_FPlayerSuit.M_PlayerWeapons.M_PlayerSecondWeapon.M_bUse)
	{

	}
	//加载武器(超级武器)
	if (PlayerData->M_FPlayerSuit.M_PlayerWeapons.M_PlayerSuperWeapon.M_bUse)
	{

	}
}

void AGamePlayer::LoadPlayerFirstWeapon(const FString& _WeapinName, const FString& _ClassPath)
{

	this->M_PlayerFirstWeapon = this->GetWorld()->SpawnActor<APlayerFirstWeapon>(
		LoadClass<APlayerFirstWeapon>(0, *_ClassPath)
		);

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("生成主武器"));
	}

	if (IsValid(this->M_PlayerFirstWeapon))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("设置角色数据"));
		}

		//设置角色
		this->M_PlayerFirstWeapon->SetPlayeActor(this);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("初始化武器数据"));
		}

		//初始化武器数据
		this->M_PlayerFirstWeapon->InitWeaponData(
			UFVMGameInstance::GetPlayerStructManager_Static(),
			_WeapinName,
			this->M_UUI_MapMeshe,
			this->M_AMapMeshe
		);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器创建完成"));
		}

		this->M_PlayerFirstWeapon->InitRotation();
		this->M_PlayerFirstWeapon->SetActorLocation(this->GetActorLocation());
		this->M_PlayerFirstWeapon->SetActorScale3D(FVector(0.7f, 0.7f, 0.7f));

		//更新角色武器位置
		this->M_PlayerFirstWeapon->MeshMoveUpdate(0.f, this->M_UUI_MapMeshe, this->M_AMapMeshe);
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("资源路径有错，确保路径末尾含有_C"));
		}
	}
}

void AGamePlayer::SetPlayerLine(const FLine& _Line)
{
	this->M_PlayerLine = _Line;
}

void AGamePlayer::SetPlayerTranslucency(UUI_MapMeshe* _CurMeshe)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	// SetTranslucency(_CurMeshe->GetTranslucency() + 1);

}

void AGamePlayer::SetCurrentMouse(AMouseActor* _MouseActor)
{
	this->M_MouseActor = _MouseActor;
}

FLine AGamePlayer::GetLine() const
{
	return this->M_PlayerLine;
}

bool AGamePlayer::PlayerFirstWeaponIsValid() const
{
	return IsValid(this->M_PlayerFirstWeapon);
}

AMouseActor* AGamePlayer::GetCurrentMouse()
{
	return this->M_MouseActor;
}

AMapMeshe* const AGamePlayer::GetMapMeshe()
{
	return this->M_AMapMeshe;
}

UUI_MapMeshe* const AGamePlayer::GetUIMapMeshe()
{
	return this->M_UUI_MapMeshe;
}

int32 AGamePlayer::GetTranslucency()
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	if (PlayerData->M_FPlayerSuit.M_bPlayerSuit && PlayerData->M_FPlayerSuit.M_ShowPlayerSuit)
	{
		return this->M_UUI_MapMeshe->GetTranslucency() + 4;
	}

	return this->M_UUI_MapMeshe->GetTranslucency() + 11;
}

void AGamePlayer::PlayerDef_Anim()
{
	

	this->M_PlayerFirstWeapon->PlayerDef_Anim();
}

void AGamePlayer::PlayerAttack_Anim()
{
	

	this->M_PlayerFirstWeapon->PlayerAttack_Anim();
}

void AGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->M_AMapMeshe->GetMove())
	{
		//更新角色路线
		this->WepaonFirstMoveUpdate(DeltaTime);
		this->PlayerLocationUpdate(DeltaTime);
		this->SetPlayerTranslucency(this->M_UUI_MapMeshe);
	}
}

void AGamePlayer::PlayerLocationUpdate(const float& DeltaTime)
{
	this->SetActorLocation(
		this->M_AMapMeshe->GetActorLocation() + this->M_AMapMeshe->M_PlayerMesheOffset
	);

	this->M_PlayerFirstWeapon->SetActorLocation(
		this->M_AMapMeshe->GetActorLocation() + this->M_AMapMeshe->M_PlayerMesheOffset
	);

	this->M_PlayerLine = this->M_AMapMeshe->GetLine();
}

void AGamePlayer::WepaonFirstMoveUpdate(const float& DeltaTime)
{
	if (this->time > 0.f)
	{
		this->time -= DeltaTime;
	}
	else {
		this->time = 0.1f;

		this->UpdateWeaponLocation(DeltaTime);
	}
}

void AGamePlayer::UpdateWeaponLocation(const float& DeltaTime)
{
	//更新主武器的计算
	if (IsValid(this->M_PlayerFirstWeapon))
	{
		this->M_PlayerFirstWeapon->MeshMoveUpdate(DeltaTime, this->M_UUI_MapMeshe, this->M_AMapMeshe);
	}
}
