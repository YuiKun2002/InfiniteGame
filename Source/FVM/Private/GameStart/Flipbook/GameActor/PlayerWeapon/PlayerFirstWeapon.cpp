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

void UWeaponSkillObject::Run(FMainWeaponData& Data, FFlyItemProOverride& ProOverride)
{
	FWeaponSkillObjectReturn RetunrData = this->SkillRun(Data, ProOverride);

	Data = RetunrData.Data;
	ProOverride = RetunrData.ProOverride;
}


FFlyItemProOverride UFlyItemProOverrideFunc::SetWeaponBulletClassObjOverride(
	const FFlyItemProOverride& FlyItemProOverride,
	const TSoftClassPtr<class AFlyItemActor>& Fly)
{
	FFlyItemProOverride Temp = FlyItemProOverride;
	Temp.bOverride = true;
	Temp.WeaponBulletClassObjOverride = Fly;
	return Temp;
}

FFlyItemProOverride UFlyItemProOverrideFunc::SetFlyItemAttackType(const FFlyItemProOverride& FlyItemProOverride, EFlyItemAttackType Type)
{
	FFlyItemProOverride Temp = FlyItemProOverride;
	Temp.bOverride = true;
	Temp.FlyItemAttackType = Type;
	return Temp;
}

FFlyItemProOverride UFlyItemProOverrideFunc::SetPanetrateLayers(const FFlyItemProOverride& FlyItemProOverride, int32 PanetrateLayers)
{
	FFlyItemProOverride Temp = FlyItemProOverride;
	Temp.bOverride = true;
	Temp.PanetrateLayers = PanetrateLayers;
	return Temp;
}

FFlyItemProOverride UFlyItemProOverrideFunc::SetAttackTypeOverride(const FFlyItemProOverride& FlyItemProOverride, const TArray<ELineType>& AttackTypeOverride)
{
	FFlyItemProOverride Temp = FlyItemProOverride;
	Temp.bOverride = true;
	Temp.AttackTypeOverride = AttackTypeOverride;
	return Temp;
}

FFlyItemProOverride UFlyItemProOverrideFunc::SetItemBuffOverride(const FFlyItemProOverride& FlyItemProOverride, const FItem_Buff& ItemBuffOverride)
{
	FFlyItemProOverride Temp = FlyItemProOverride;
	Temp.bOverride = true;
	Temp.ItemBuffOverride = ItemBuffOverride;
	return Temp;
}

FFlyItemProOverride UFlyItemProOverrideFunc::SetBaseAttackUpRate(const FFlyItemProOverride& FlyItemProOverride, float BaseAttackUpRate /*= 1.f */)
{
	FFlyItemProOverride Temp = FlyItemProOverride;
	Temp.bOverride = true;
	Temp.BaseAttackUpRate = BaseAttackUpRate;
	return Temp;
}

FFlyItemProOverride UFlyItemProOverrideFunc::SetSecondAttackOverwrite(const FFlyItemProOverride& FlyItemProOverride, float SecondAttackOverwrite /*= 0.f */)
{
	FFlyItemProOverride Temp = FlyItemProOverride;
	Temp.bOverride = true;
	Temp.SecondAttackOverwrite = SecondAttackOverwrite;
	return Temp;
}

APlayerFirstWeapon::APlayerFirstWeapon()
{
	this->M_UFirstWeaponProjectionComponent = CreateDefaultSubobject<UFirstWeaponProjectionComponent>(
		TEXT("FristWeapon_Projection")
	);
	this->BulletLocationComp = CreateDefaultSubobject<USceneComponent>(
		TEXT("FristWeapon_BulletLocationComp")
	);
	this->BoneFollowerComp = CreateDefaultSubobject<USpineBoneFollowerComponent>(
		TEXT("FristWeapon_SpineBoneFollowerComponent")
	);
	this->BulletLocationComp->SetupAttachment(this->GetPointComponent());
}

void APlayerFirstWeapon::InitSkill(FMainWeaponData& WeaponData)
{
	FMainWeaponData OldData = WeaponData;
	//初始化技能
	this->WeaponSkils = OldData.Skils;
	//当前武器等级
	int32 CurWeaponLevel = OldData.StarsLevel + 1;

	UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("武器星级：【") +
		FString::FromInt(CurWeaponLevel) +
		TEXT("】"));

	for (auto WeaponPP = this->WeaponSkils.CreateConstIterator(); WeaponPP; ++WeaponPP)
	{
		if (WeaponPP->Key <= CurWeaponLevel)
		{
			UWeaponSkillObject* Obj = WeaponPP->Value.GetDefaultObject();
			if (IsValid(Obj))
			{
				Obj->Run(WeaponData, this->FlyItemProOverride);

				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("执行武器技能：【") +
					FString::FromInt(WeaponPP->Key) +
					TEXT("】"));
			}
		}
	}
}

void APlayerFirstWeapon::InitWeapon(AGamePlayer* Player, const FMainWeaponData& WeaponData, UUI_MapMeshe* _UI_MapMeshe, AMapMeshe* _MapMeshe)
{
	//初始化数据
	this->M_FFPlayerWeaponFirstData = WeaponData;

	//覆盖子弹
	if (!this->FlyItemProOverride.WeaponBulletClassObjOverride.GetAssetName().IsEmpty())
	{
		this->WeaponBulletClassObj = this->FlyItemProOverride.WeaponBulletClassObjOverride;
	}

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
	this->BoneFollowerComp->BoneName = TEXT("WeaponSlot");

	this->BoneFollowerComp->UseComponentTransform = false;
	this->BoneFollowerComp->UsePosition = true;
	this->BoneFollowerComp->UseRotation = false;
	this->BoneFollowerComp->UseScale = false;

	//初始化显示
	this->InitSpineShow();

	//初始化渲染层级
	this->SetRenderLayer(Player->GetSpineRenderLayer() + 1);

	//计算射线目标位置
	for (const auto& LineTrace : this->GetPlayerFirstWeaponData().LineTraceSettings)
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
