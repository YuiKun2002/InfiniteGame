// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_BZ.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>

ABoss_BZ::ABoss_BZ() : ABossSphereBase() {}

void ABoss_BZ::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞位置
	UGameSystemFunction::InitMouseMeshe(
		this->MMeshe,
		this->MBody,
		FVector2D(20.f),
		FVector2D(0.f)
	);
}

void ABoss_BZ::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(TEXT("冰渣"), TEXT("579"));

	this->ChangeState(NewObject<UBZ_OutState>());
}

void ABoss_BZ::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (IsValid(this->Get()))
	{
		this->Get()->ExecuteBuff(BuffTag, CurBuffTime);
	}
}

void ABoss_BZ::MouseDeathed()
{
	Super::MouseDeathed();

	this->ClosedBoxComponent(this->MMeshe);
	this->ClosedBoxComponent(this->MBody);

	if (this->bDoubleState)
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimA_Death),true);
	}
	else {
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimB_Death),true);
	}
}

ABoss_BZ* UBZ_BaseState::GetBZ()
{
	return Cast<ABoss_BZ>(this->Get());
}

void UBZ_BaseState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{

	if (AttackType == EFlyItemAttackType::Bomb && !this->GetBZ()->bDoubleState)
	{
		if (this->GetLevel() < 3)
		{
			//20%
			_HurtValue = _HurtValue * 0.8f;
		}
		else {
			//火渣伤害减半 50%
			_HurtValue = _HurtValue * 0.5f;
		}
	}
	else {
		AFlyItemActor* CurFly = Cast<AFlyItemActor>(CurHitMouseObj);
		if (IsValid(CurFly))
		{
			if (!this->GetBZ()->bDoubleState)
			{
				if (CurFly->GetBuff().M_bCondition)
				{
					for (const auto& CurBuff : CurFly->GetBuff().M_Buffers)
					{
						if (CurBuff.Key == Buff_Infor::Burn)
						{
							_HurtValue = _HurtValue * 0.90f;
							break;
						}
					}
				}
			}
			else {
				if (this->GetLevel() >= 3)
				{
					if (CurFly->GetBuff().M_bCondition)
					{
						for (const auto& CurBuff : CurFly->GetBuff().M_Buffers)
						{
							if (CurBuff.Key == Buff_Infor::E_SlowDown)
							{
								_HurtValue = _HurtValue * 0.95f;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void UBZ_BaseState::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (this->GetBZ()->bDoubleState)
	{
		if (
			BuffTag == EGameBuffTag::Freeze
			||
			BuffTag == EGameBuffTag::Solidification
			||
			BuffTag == EGameBuffTag::SlowDown
			)
		{
			CurBuffTime = 0.f;
		}
	}
	else {
		if (BuffTag == EGameBuffTag::Solidification)
		{
			CurBuffTime = 0.f;
		}

		if (
			BuffTag == EGameBuffTag::Freeze
			||
			BuffTag == EGameBuffTag::SlowDown
			)
		{
			CurBuffTime = CurBuffTime * 0.5f;
		}
	}
}

void UBZ_OutState::Init()
{
	Super::Init();

	this->BossOutDef();

	if (this->GetLevel() < 3)
	{
		if (this->Get()->GetCurrentHP() > this->Get()->GetTotalHP() * 0.5f)
		{
			this->GetBZ()->bDoubleState = true;
			this->PlayAnim(
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_Out),
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_OutLow)
			);
		}
		else {
			this->GetBZ()->bDoubleState = false;
			this->PlayAnim(
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_Out),
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_OutLow)
			);
		}
	}
	else {
		if (UGameSystemFunction::GetRange(50, 100))
		{
			this->GetBZ()->bDoubleState = true;
			this->PlayAnim(
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_Out),
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_OutLow)
			);
		}
		else {
			this->GetBZ()->bDoubleState = false;
			this->PlayAnim(
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_Out),
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_OutLow)
			);
		}
	}

	if (this->GetLevel() < 3)
	{
		this->Get()->DefSpawnMouse(1);
	}
	else {
		this->Get()->DefSpawnMouse(this->GetLevel());
	}

	if (this->GetBZ()->bDoubleState)
	{
		this->Get()->SetBossName(TEXT("冰渣-冰系"));
	}
	else {
		this->Get()->SetBossName(TEXT("冰渣-火系"));
	}
}

void UBZ_OutState::AnimPlayEnd()
{
	//技能选择
	if (UGameSystemFunction::GetRange(50, 100))
	{
		this->Get()->ChangeState(NewObject<UBZ_MagicATKState>());
	}
	else {
		this->Get()->ChangeState(NewObject<UBZ_ElementSphereATKState>());
	}
}

void UBZ_MagicATKState::Init()
{
	if (this->GetBZ()->bDoubleState)
	{
		this->bCount = 4;
		this->PlayAnim(
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_Atk),
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_AtkLow)
		);
	}
	else {
		this->bCount = 3;
		this->PlayAnim(
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_Atk),
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_AtkLow)
		);
	}

	this->Get()->DefSpawnMouse(1);
}

void UBZ_MagicATKState::AnimPlayEnd()
{
	if (this->GetBZ()->bDoubleState)
	{
		int32 CurRow = this->Get()->GetMouseLine().Row;
		for (int32 j = 0; j < this->Get()->GetMouseLine().Col; j++)
		{
			AMapMeshe* Cur = this->Get()->GetMesheComp()->GetMapMesh(CurRow, j);
			if (IsValid(Cur) && IsValid(Cur->GetUIMapMeshe()))
			{
				if (
					IsValid(Cur->GetUIMapMeshe()->GetCardByID(0))
					||
					IsValid(Cur->GetUIMapMeshe()->GetCardByID(1))
					)
				{
					//创建
					this->GetBZ()->OnSpawnBullet(
						true,
						this->Get()->GetActorLocation(),
						this->GetMesheLocation(CurRow, j)
					);
					this->bCount--;
					if (this->bCount < 1)
					{
						//切换形态->退出
						this->Get()->ChangeState(NewObject<UBZ_ExitState>());
					}
					return;
				}
			}
		}
	}
	else {
		int32 CurRow = this->Get()->GetMouseLine().Row;
		for (int32 j = this->Get()->GetMouseLine().Col - 1; j >= 0; j--)
		{
			AMapMeshe* Cur = this->Get()->GetMesheComp()->GetMapMesh(CurRow, j);
			if (IsValid(Cur) && IsValid(Cur->GetUIMapMeshe()))
			{
				if (
					IsValid(Cur->GetUIMapMeshe()->GetCardByID(0))
					||
					IsValid(Cur->GetUIMapMeshe()->GetCardByID(1))
					)
				{
					//创建
					this->GetBZ()->OnSpawnBullet(
						false,
						this->Get()->GetActorLocation(),
						this->GetMesheLocation(CurRow, j)
					);

					this->bCount--;
					if (this->bCount < 1)
					{
						//切换形态->退出
						this->Get()->ChangeState(NewObject<UBZ_ExitState>());
					}
					return;
				}
			}
		}
	}

	this->bCount--;
	if (this->bCount < 1)
	{
		//切换形态->退出
		this->Get()->ChangeState(NewObject<UBZ_ExitState>());
	}
}

void UBZ_ExitState::Init()
{
	this->BossExitDef();

	this->Get()->DefSpawnMouse(1);

	this->fTime = 13.5f;

	if (this->GetBZ()->bDoubleState)
	{
		this->PlayAnim(
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_Exit),
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_ExitLow)
		);
	}
	else {
		this->PlayAnim(
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_Exit),
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_ExitLow)
		);
	}
}

void UBZ_ExitState::MouseTick(const float& DeltaTime)
{
	if (this->fTime > 0.f)
	{
		this->fTime -= DeltaTime;
	}
	else {
		this->Get()->ChangeState(NewObject<UBZ_OutState>());
	}
}

void UBZ_ExitState::AnimPlayEnd()
{
	this->Get()->AddActorLocalOffset(FVector(0.f, 500.f, 0.f));
}

void UBZ_ElementSphereATKState::Init()
{
	this->Count = 5;
	this->bInit = false;

	if (this->GetBZ()->bDoubleState)
	{
		this->PlayAnim(
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_IceSphere),
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_IceSphereLow)
		);
	}
	else {
		this->PlayAnim(
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_FireSphere),
			UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_FireSphereLow)
		);
	}
}

void UBZ_ElementSphereATKState::AnimPlayEnd()
{
	this->Count--;
	if (this->Count > 0)
	{
		return;
	}

	if (this->bInit)
	{
		if (!this->GetBZ()->bDoubleState)
		{
			this->Get()->DefSpawnMouse(2);
		}

		//状态切换
		this->Get()->ChangeState(NewObject<UBZ_ExitState>());
	}
	else {
		if (this->GetBZ()->bDoubleState)
		{
			this->PlayAnim(
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_PushIceSphere),
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimA_PushIceSphereLow)
			);
		}
		else {
			this->PlayAnim(
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_PushFireSphere),
				UGameSystemFunction::LoadRes(this->GetBZ()->AnimB_PushFireSphereLow)
			);
		}

		//创建球体
		if (this->GetBZ()->bDoubleState)
		{
			this->GetBZ()->OnSpawnElementSphere(true, this->Get()->GetMouseLine(), this->Get()->GetActorLocation());
		}
		else {
			this->GetBZ()->OnSpawnElementSphere(false, this->Get()->GetMouseLine(), this->Get()->GetActorLocation());
		}

		this->bInit = true;
	}

}
