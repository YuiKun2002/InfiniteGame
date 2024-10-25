// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Card/LockingAttackComponent.h"
#include "GameStart/Flipbook/GameActor/CardActor/AttackCardActor.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/GameMapInstance.h"


void ULockingAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化卡片对象
	this->CardActor = Cast<AAttackCardActor>(this->GetOwner());
	//初始化外星人管理器
	this->AlienManager = AGameMapInstance::GetGameMapInstance()->GetMouseManagerComponent_Static();

	if (!IsValid(this->CardActor) || !IsValid(this->AlienManager))
	{
		this->SetTickableWhenPaused(true);
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("[追踪组件]或者[外星人管理器]初始化失败，需要附加在攻击卡片"));
	}
}

void ULockingAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->Time < 0.2f)
	{
		this->Time += DeltaTime;
	}
	else {
		this->Time = 0.f;

		if (AGameMapInstance::GetGameMapInstance()->GetGameOver())
		{
			return;
		}

		//如果当前有外星人被锁定
		if (IsValid(CurrentAlien) && !CurrentAlien->GetMouseIsDeath())
		{
			return;
		}


		this->SetAttackModEnabled(false);
		//查询结果
		bool CheckResult = CheckLineAlien(this->CardActor->GetLine().Row);
		if (CheckResult)
		{
			this->SetAttackModEnabled(true);
		}
		else {
			TArray<int32> Rows;
			for (int32 i = 0; i < AGameMapInstance::GetGameMapInstance()->
				M_MesheControllComponent->GetMapMeshRowAndCol().Row; i++)
			{
				if (i != this->CardActor->GetLine().Row)
				{
					Rows.Emplace(i);
				}
			}

			int32 RanRow = UGameSystemFunction::GetRandomRange(0, Rows.Num() - 1);

			if (CheckLineAlien(Rows[RanRow]))
			{
				this->SetAttackModEnabled(true);
				return;
			}
		}

	}
}

void ULockingAttackComponent::LoadResource()
{
	Super::LoadResource();


	//初始化数据
	this->InitLaunchProperty(
		this->CardActor->GetCurrentAttackCount(),
		this->CardActor->GetCurrentAttackSpeed(),
		this->CardActor->GetCurrentFristAttackDelay(),
		this->CardActor->GetCurrentSecondAttackDelay()
	);

	//初始化检测类型
	const TArray<FLineTraceSetting>& Line = this->CardActor->GetSourceLineTraceSetting();
	for (const FLineTraceSetting& LineTrace : Line)
	{
		this->CheckType.Emplace(LineTrace.M_ELineTraceType);
	}

	//初始化动画
	this->InitLaunchBulletByDef(this->CardActor->CardActor_BulletClassObj);

	//播放默认动画
	this->PlayIdleAnim();
}

void ULockingAttackComponent::SpawnBullet(AFlyItemActor* NewBullet)
{
	if (IsValid(this->CurrentAlien) && !this->CurrentAlien->GetMouseIsDeath())
	{
		//重新检测外星人【动态变动】
		if (CheckLineAlien(this->CurrentAlien->GetMouseLine().Row))
		{
			//因为重新检测了一次，那么需要重新判断一次
			if (IsValid(this->CurrentAlien))
			{
				FTransform Trans;
				Trans.SetLocation(CurrentAlien->GetActorLocation());

				NewBullet->SetLine(this->CardActor->GetLine().Row);
				NewBullet->SetActorTransform(Trans);
				NewBullet->SetObjectActorLocation(this->CardActor->GetCurrentMouse());
				NewBullet->SetATK(this->CardActor->GetCurrentATK());
				NewBullet->SetSecondATK(
					this->CardActor->GetCurrentSecondATK(
						this->CardActor->GetATKCardData().M_SputteringATKRate)
				);
				NewBullet->SetFloatModeEnable(true);
				NewBullet->Init();
				NewBullet->HitMouse_OverlapBegin(CurrentAlien);
			}
		}
		else {
			this->CurrentAlien = nullptr;
		}
	}
}

void ULockingAttackComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->PlayAttackAnim();
}

void ULockingAttackComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->PlayIdleAnim();
}

void ULockingAttackComponent::AnimationPlayEnd(UTrackEntry* Track)
{
	this->OnAnimationPlayEnd();
	this->SetTrackEntry(nullptr);
}

void ULockingAttackComponent::PlayIdleAnim()
{
	UTrackEntry* Track = this->CardActor->SetAnimation(0, this->GetIdleAnimName(), true);
	//BINDANIMATION(Track,this,&ULockingAttackComponent::OnAnimationPlayEnd);
	this->SetTrackEntry(Track);
}

void ULockingAttackComponent::PlayAttackAnim()
{
	UTrackEntry* Track = this->CardActor->SetAnimation(0, this->GetAttackAnimName(), true);
	BINDANIMATION(Track, this, &ULockingAttackComponent::AnimationPlayEnd);
	this->SetTrackEntry(Track);
}

bool ULockingAttackComponent::CheckLineAlien(int32 Row)
{
	UMouseLineManager* AlienLine = this->AlienManager->GetMouseLineManager(Row);
	if (AlienLine->GetMouseExist())
	{
		if (this->CheckAlien(AlienLine))
		{
			return true;
		}
	}

	return false;
}

bool ULockingAttackComponent::CheckAlien(UMouseLineManager* AlienManagerComp)
{
	for (const ELineTraceType& Type : this->CheckType)
	{
		//优先检测空中
		switch (Type)
		{
		case ELineTraceType::E_MouseSky:
		{
			if (AlienManagerComp->GetMouseSky().Num())
			{
				this->CurrentAlien = AlienManagerComp->GetMouseTopBySky();
				return true;
			}
		}break;
		case ELineTraceType::E_MouseGroundAndSky:
		{
			if (AlienManagerComp->GetMouseSky().Num())
			{
				this->CurrentAlien = AlienManagerComp->GetMouseTopBySky();
				return true;
			}

			if (AlienManagerComp->GetMouseGround().Num())
			{
				this->CurrentAlien = AlienManagerComp->GetMouseTopByGorund();
				return true;
			}
		}break;
		case ELineTraceType::E_MouseGround:
		{
			if (AlienManagerComp->GetMouseGround().Num())
			{
				this->CurrentAlien = AlienManagerComp->GetMouseTopByGorund();
				return true;
			}
		}break;
		case ELineTraceType::E_MouseUnder:
		{
			if (AlienManagerComp->GetMouseUnderGround().Num())
			{
				this->CurrentAlien = AlienManagerComp->GetMouseTopByUnderGorund();
				return true;
			}
		}
		break;
		default:
			this->CurrentAlien = AlienManagerComp->GetMouseTopByAllType();
			return true;
		}
	}

	return false;
}
