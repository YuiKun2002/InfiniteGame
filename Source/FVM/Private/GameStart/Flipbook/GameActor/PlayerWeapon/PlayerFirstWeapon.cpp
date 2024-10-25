// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/PlayerWeapon/PlayerFirstWeapon.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "SpineBoneFollowerComponent.h"
#include "SpineSkeletonRendererComponent.h"

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
	this->BoneFollowerComp = CreateDefaultSubobject<USpineBoneFollowerComponent>(TEXT("FristWeapon_SpineBoneFollowerComponent"));
	this->BulletLocationComp->SetupAttachment(this->GetPointComponent());
}

void APlayerFirstWeapon::InitWeapon(AGamePlayer* Player, const FMainWeaponData& WeaponData, UUI_MapMeshe* _UI_MapMeshe, AMapMeshe* _MapMeshe)
{
	this->M_FFPlayerWeaponFirstData = WeaponData;

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("计算射线位置"));
	}

	//交换坐标
	FVector PointLocation = this->GetPointComponent()->GetRelativeLocation();
	this->GetPointComponent()->SetRelativeLocation(
		FVector(
			PointLocation.Y,
			PointLocation.X,
			PointLocation.Z
		)
	);

	FVector BulletLocation = this->BulletLocationComp->GetRelativeLocation();
	this->BulletLocationComp->SetRelativeLocation(
		FVector(
			BulletLocation.Y,
			BulletLocation.X,
			BulletLocation.Z
		)
	);

	this->BoneFollowerComp->Target = Player;
	this->BoneFollowerComp->BoneName = TEXT("hand1");

	this->BoneFollowerComp->UseComponentTransform = false;
	this->BoneFollowerComp->UsePosition = true;
	this->BoneFollowerComp->UseRotation = false;
	this->BoneFollowerComp->UseScale = false;

	//初始化显示
	this->InitSpineShow();

	//初始化渲染层级
	this->SetRenderLayer(Player->GetSpineRenderLayer() + 1);

	//计算射线目标位置
	for (const auto& LineTrace : this->WeaponLineTraceSettings)
	{
		UGameSystemFunction::CalculateLineTracePosition(
			_MapMeshe->GetActorLocation(),
			_UI_MapMeshe->GetLine(),
			_UI_MapMeshe,
			LineTrace,
			this->M_FirstWeaponLineTraceSettings
		);
	}

	this->M_UFirstWeaponProjectionComponent->LoadResource();
}

const FMainWeaponData& APlayerFirstWeapon::GetPlayerFirstWeaponData()
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
	//this->M_FirstWeaponLineTraceSettings.Empty();

	////计算射线目标位置
	//for (const auto& LineTrace : this->M_FFPlayerWeaponFirstData.M_LineTraceSettings)
	//{
	//	UGameSystemFunction::CalculateLineTracePosition(
	//	_MapMeshe->GetActorLocation(), 
	//	_UI_MapMeshe->GetLine(),
	//	_UI_MapMeshe, LineTrace, this->M_FirstWeaponLineTraceSettings
	//	);
	//}
}

void APlayerFirstWeapon::SetPlayeActor(AGamePlayer* _Player)
{
	this->M_AGamePlayer = _Player;
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
