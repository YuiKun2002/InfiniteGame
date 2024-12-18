// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/MapMeshe.h"
#include "SpineSkeletonAnimationComponent.h"
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
	if (PlayerData->PlayerEquipWeaponData.bMainEquip)
	{
		this->LoadPlayerWeapon(
			*PlayerData->PlayerEquipWeaponData.MainWeapon.ItemName.ToString(),
			PlayerData->PlayerEquipWeaponData.MainWeapon
		);
	}
	else if (PlayerData->PlayerEquipWeaponData.bSecondaryEquip)
	{
		this->LoadPlayerWeapon(
			*PlayerData->PlayerEquipWeaponData.SecondaryWeapon.ItemName.ToString(),
			PlayerData->PlayerEquipWeaponData.SecondaryWeapon
		);
	}
	else
	{
		FMainWeaponData DefWeaponData;
		//如果没有装备则使用默认武器
		this->LoadPlayerWeapon(TEXT("CatGun"), DefWeaponData);
	}
}

void AGamePlayer::LoadPlayerWeapon(const FName& WeapinName, const FMainWeaponData& WeaponData)
{
	if (IsValid(this->M_PlayerFirstWeapon))
	{
		this->M_PlayerFirstWeapon->Destroy();
		this->M_PlayerFirstWeapon = nullptr;
	}

	//通过名称加载角色
	UEquipmentDataAssetCache* Cache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);
	//武器实例对象
	TSoftClassPtr<APlayerFirstWeapon>* PlayerSoftIns = nullptr;
	for (FEquipment_Weapon_Data& CurDatas : Cache->GetWeapons())
	{
		PlayerSoftIns = CurDatas.Weapons.Find(WeapinName);
		if (PlayerSoftIns)
		{
			break;
		}
	}

	if (!PlayerSoftIns)
	{
		return;
	}

	//加载武器
	this->M_PlayerFirstWeapon = this->GetWorld()->SpawnActor<APlayerFirstWeapon>(
		PlayerSoftIns->LoadSynchronous()
	);

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("生成主武器"));
	}

	if (IsValid(this->M_PlayerFirstWeapon))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("设置武器的角色数据"));
		}

		//设置角色
		this->M_PlayerFirstWeapon->SetPlayeActor(this);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("初始化武器数据"));
		}

		//初始化武器数据
		this->M_PlayerFirstWeapon->InitWeapon(
			this,
			WeaponData,
			this->M_UUI_MapMeshe,
			this->M_AMapMeshe
		);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器创建完成"));
		}

		//this->M_PlayerFirstWeapon->InitRotation();
		//this->M_PlayerFirstWeapon->SetActorLocation(this->GetActorLocation());
		//更新角色武器位置
		//this->M_PlayerFirstWeapon->MeshMoveUpdate(0.f, this->M_UUI_MapMeshe, this->M_AMapMeshe);
	}
}

void AGamePlayer::SetPlayerLine(const FLine& _Line)
{
	this->M_PlayerLine = _Line;
}

void AGamePlayer::SetPlayerTranslucency(UUI_MapMeshe* _CurMeshe)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	this->SetRenderLayer(_CurMeshe->GetTranslucency() + 1);
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

int32 AGamePlayer::GetPlayerRenderLayerToCardLayer()
{
	return this->GetRenderLayer() + 11;
}

void AGamePlayer::PlayerDef_Anim()
{
	this->SetAnimation(0, TEXT("idle"), true);
}

void AGamePlayer::PlayerAttack_Anim(float TimeScale)
{
	UTrackEntry* Track = this->SetAnimation(0, TEXT("attack"), true);
	Track->SetTimeScale(TimeScale);
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
