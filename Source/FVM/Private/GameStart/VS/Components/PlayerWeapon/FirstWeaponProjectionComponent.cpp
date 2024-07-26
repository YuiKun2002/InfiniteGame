// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/PlayerWeapon/FirstWeaponProjectionComponent.h"
#include "GameStart/Flipbook/GameActor/PlayerWeapon/PlayerFirstWeapon.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"

void UFirstWeaponProjectionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器投射组件，初始化"));
	}

	this->M_Owner = Cast<APlayerFirstWeapon>(this->GetOwner());
	if (IsValid(this->M_Owner))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器对象获取成功"));
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器对象获取失败"));
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("主武器投射组件，构造完毕"));
	}
}

void UFirstWeaponProjectionComponent::Spawn()
{
	Super::Spawn();

	//调用动态多播代理物体投射
	this->OnSpawn.Broadcast();
}

void UFirstWeaponProjectionComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();


	this->M_Owner->GetPlayerActor()->PlayerAttack_Anim();
}

void UFirstWeaponProjectionComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->M_Owner->GetPlayerActor()->PlayerDef_Anim();
}

void UFirstWeaponProjectionComponent::InitCondition()
{
	Super::InitCondition();

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("初始化条件"));
	}
}

void UFirstWeaponProjectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->M_time < 0.3f)
	{
		this->M_time += DeltaTime;
	}
	else {
		this->M_time = 0.f;

		if (!IsValid(this->M_Owner))
		{
			return;
		}

		if (!IsValid(this->M_Owner->GetPlayerActor()))
		{
			return;
		}

		bool LResult = false;

		//当前检测到的老鼠线路
		ELineTraceType CurCheckLineType;
		//当前老鼠
		AActor* CurMouse = UGameSystemFunction::CreateCheckMouseLineTrace(
			this->GetWorld(),
			this->M_Owner->GetLineTraceSetting(), CurCheckLineType
		);

		//老鼠有效则设置攻击模式
		if (Cast<AMouseActor>(CurMouse) && IsValid(CurMouse))
		{
			this->M_Owner->GetPlayerActor()->SetCurrentMouse(Cast<AMouseActor>(CurMouse));
			this->SetAttackModEnabled(true);
		}
		else
		{
			this->M_Owner->GetPlayerActor()->SetCurrentMouse(nullptr);
			this->SetAttackModEnabled(false);
		}

		//for (const auto& Line : this->M_Owner->GetLineTraceSetting())
		//{
		//	DrawDebugLine(this->GetWorld(), Line.M_BeginLocation, Line.M_EndLocation, FColor::Red);

		//	M_Trance_Target = ECollisionChannel::ECC_Visibility;

		//	switch (Line.M_ELineTraceType)
		//	{
		//	case ELineTraceType::E_MouseGround:M_Trance_Target = ECollisionChannel::ECC_GameTraceChannel2; break;
		//	case ELineTraceType::E_MouseSky:M_Trance_Target = ECollisionChannel::ECC_GameTraceChannel3; break;
		//	case ELineTraceType::E_MouseUnder:M_Trance_Target = ECollisionChannel::ECC_GameTraceChannel4; break;
		//	}

		//	if (this->GetWorld()->LineTraceSingleByChannel(M_Trance_Result, Line.M_BeginLocation, Line.M_EndLocation, M_Trance_Target, M_Trance_Params, M_Trance_ResParams))
		//	{
		//		this->M_Owner->GetPlayerActor()->SetCurrentMouse(Cast<AMouseActor>(M_Trance_Result.GetActor()));

		//		LResult = true;
		//	}
		//}

		////所有的线扫描结束之后结果是false，则关闭当前攻击对象
		//if (!LResult)
		//{
		//	this->M_Owner->GetPlayerActor()->SetCurrentMouse(nullptr);
		//}

		////如果老鼠存在
		//if (IsValid(this->M_Owner->GetPlayerActor()->GetCurrentMouse()))
		//{
		//	this->SetAttackModEnabled(true);
		//}
		//else {
		//	this->SetAttackModEnabled(false);
		//}
	}
}

void UFirstWeaponProjectionComponent::LoadResource()
{
	Super::LoadResource();

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("加载资源"));
	}

	const FPlayerWeaponFirst& LData = this->M_Owner->GetPlayerFirstWeaponData();
	this->InitLaunchProperty(
		LData.M_ProjectionItemCount,
		LData.M_ShootSpeed,
		LData.M_FirstShootSpeed,
		LData.M_ShootDelay
	);
}

AFlyItemActor* UFirstWeaponProjectionComponent::SpawnFlyItem(TSoftClassPtr<AFlyItemActor> _Path_C, FVector _Offset)
{
	if (!IsValid(this->Pool))
	{
		this->Pool = UObjectPoolManager::MakePoolManager(this->GetWorld(), _Path_C, 1);
	}

	//子弹的相对位置
	FVector CurLocation = this->M_Owner->GetBulletLocationComp()->GetRelativeLocation();
	//整体角色的世界位置
	FVector OwnerLocation = this->M_Owner->GetGameLocation()->GetComponentLocation();

	FTransform Trans;
	Trans.SetLocation(FVector(OwnerLocation.X, OwnerLocation.Y + CurLocation.X, OwnerLocation.Z + CurLocation.Z));

	AFlyItemActor* L_AFlyItemActor_ = Cast<AFlyItemActor>(this->Pool->GetObjectActor());
	L_AFlyItemActor_->SetActorTransform(Trans);
	L_AFlyItemActor_->AddActorLocalOffset(L_AFlyItemActor_->M_OffsetPosition);
	L_AFlyItemActor_->M_CustomActorOwner = this->M_Owner;
	L_AFlyItemActor_->SetMouseActorLocation(this->M_Owner->GetPlayerActor()->GetCurrentMouse());
	L_AFlyItemActor_->SetATK(this->M_Owner->GetPlayerFirstWeaponData().M_ATKBase);
	L_AFlyItemActor_->SetSecondATK(this->M_Owner->GetPlayerFirstWeaponData().M_SputteringATKRate);
	L_AFlyItemActor_->SetLine(this->M_Owner->GetPlayerActor()->GetLine().Row);
	L_AFlyItemActor_->SetFloatModeEnable(this->M_Owner->GetPlayerActor()->GetMapMeshe()->GetMove());
	L_AFlyItemActor_->Init();
	L_AFlyItemActor_->OnInit();
	//L_AFlyItemActor_->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));
	//AFlyItemActor* L_AFlyItemActor_ = Cast<AFlyItemActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this->M_Owner->GetWorld(), LoadClass<AFlyItemActor>(0, *_Path_C), Trans));
	//UGameplayStatics::FinishSpawningActor(L_AFlyItemActor_, Trans);

	return L_AFlyItemActor_;
}

void UFirstWeaponProjectionComponent::UpdateAutoAttack(float _DeltaTime)
{
	if (AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->IsMouseExist())
	{
		this->SetAttackModEnabled(true);
	}
}
