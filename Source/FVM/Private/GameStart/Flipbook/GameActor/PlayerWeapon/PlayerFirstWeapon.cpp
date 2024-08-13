// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/PlayerWeapon/PlayerFirstWeapon.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"

#include "GameStart/VS/Components/PlayerWeapon/FirstWeaponProjectionComponent.h"

#include "GameSystem/PlayerStructManager.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/Flipbook/GameActor/MouseActor.h"

APlayerFirstWeapon::APlayerFirstWeapon()
{
	this->M_UFirstWeaponProjectionComponent = CreateDefaultSubobject<UFirstWeaponProjectionComponent>(TEXT("FristWeapon_Projection"));
	this->BulletLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("FristWeapon_BulletLocationComp"));
	this->BulletLocationComp->SetupAttachment(this->GetRootComponent());
}

void APlayerFirstWeapon::InitWeaponData(UPlayerStructManager* _Player, const FString& _WeaponName, UUI_MapMeshe* _UI_MapMeshe, AMapMeshe* _MapMeshe)
{
	FPlayerWeaponFirst TempData;

	UEquipmentDataAssetCache* Cache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);
	//获取基础数据(从数据库中查询具体数据)
	for (const auto& Data : Cache->GetWeaponFirst())
	{
		if (Data.M_FEquipment.ItemName.ToString().Equals(_WeaponName))
		{
			TempData = Data.M_FEquipment;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("武器数据查询成功，并且赋值"));
			}

			break;
		}
	}

	//初始化名称
	if (_Player->M_FPlayerSuit.M_PlayerWeapons.M_PlayerFirstWeapon.M_bUse)
	{
		//查询角色武器库数据

	}

	this->M_FFPlayerWeaponFirstData = TempData;

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("计算射线位置"));
	}

	//计算射线目标位置
	for (const auto& LineTrace : this->M_FFPlayerWeaponFirstData.M_LineTraceSettings)
	{
		UGameSystemFunction::CalculateLineTracePosition(
			_MapMeshe->GetActorLocation(),
			_UI_MapMeshe->GetLine(),
			_UI_MapMeshe,
			LineTrace,
			this->M_FirstWeaponLineTraceSettings
		);
	}


	//初始化动画数据
	this->M_Anim_FirstWeaponDef = LoadObject<UPaperFlipbook>(0,
		*this->M_FFPlayerWeaponFirstData.M_WeaponDef_Path.ToString()
		);

	this->M_Anim_FirstWeaponAttack = LoadObject<UPaperFlipbook>(0,
		*this->M_FFPlayerWeaponFirstData.M_WeaponAttack_Path.ToString()
		);


	this->M_UFirstWeaponProjectionComponent->LoadResource();
}

const FPlayerWeaponFirst& APlayerFirstWeapon::GetPlayerFirstWeaponData()
{
	return this->M_FFPlayerWeaponFirstData;
}

void APlayerFirstWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerFirstWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerFirstWeapon::MeshMoveUpdate(float DeltaTime, UUI_MapMeshe* _UI_MapMeshe, AMapMeshe* _MapMeshe)
{
	this->M_FirstWeaponLineTraceSettings.Empty();

	//计算射线目标位置
	for (const auto& LineTrace : this->M_FFPlayerWeaponFirstData.M_LineTraceSettings)
	{
		UGameSystemFunction::CalculateLineTracePosition(
		_MapMeshe->GetActorLocation(), 
		_UI_MapMeshe->GetLine(),
		_UI_MapMeshe, LineTrace, this->M_FirstWeaponLineTraceSettings
		);
	}
}

void APlayerFirstWeapon::SetPlayeActor(AGamePlayer* _Player)
{
	this->M_AGamePlayer = _Player;
}

void APlayerFirstWeapon::PlayerDef_Anim()
{
	this->SetPlayAnimation(this->M_Anim_FirstWeaponDef);
}

void APlayerFirstWeapon::PlayerAttack_Anim()
{
	this->SetPlayAnimation(this->M_Anim_FirstWeaponAttack);
}

USceneComponent* APlayerFirstWeapon::GetBulletLocationComp()
{
	return this->BulletLocationComp;
}

AGamePlayer* APlayerFirstWeapon::GetPlayerActor()
{
	return this->M_AGamePlayer;
}

const TArray<FLineTraceSetting>& APlayerFirstWeapon::GetLineTraceSetting()
{
	return this->M_FirstWeaponLineTraceSettings;
}
