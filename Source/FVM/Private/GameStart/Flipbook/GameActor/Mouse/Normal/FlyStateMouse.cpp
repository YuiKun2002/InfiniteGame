// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/FlyStateMouse.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/GameMapInstance.h"
#include "Paper2D/Classes/PaperFlipbookActor.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

AFlyStateMouse::AFlyStateMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AFlyStateMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);

	this->SetAnimation(0,TEXT("SpineTag"),true);

	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AFlyStateMouse::OnAnimationPlayEnd);
}

void AFlyStateMouse::MouseInit()
{
	Super::MouseInit();

	this->bFly = true;
	this->bShoot = false;
	this->bFalling = false;
	this->bFallingStart = false;
	this->bEnableAttack = false;
	this->bShootDelay = 5.f;

	if (this->GetMouseLineType() != ELineType::Sky)
	{
		this->SetMouseLineType(ELineType::Sky);
		this->EnableCo(false);
	}
}

void AFlyStateMouse::EnableCo(bool COnGround)
{
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->MesheComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->MesheComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	if (COnGround)
	{
		this->MesheComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
		this->CollisionComp->SetRelativeLocation(FVector(0.f));//SetLocation(FVector(0.f));
	}
	else {
		this->MesheComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	}
	this->MesheComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block);

	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AFlyStateMouse::DisEnableCo()
{
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFlyStateMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bEnableAttack)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime);
	}
}

void AFlyStateMouse::AttackedBegin()
{
	Super::AttackedBegin();

	if (this->GetCurrentHP() > 0.f)
	{
		this->UpdateState();
	}
}

void AFlyStateMouse::AttackedEnd()
{
	Super::AttackedEnd();

	if (this->GetCurrentHP() > 0.f)
	{
		this->UpdateState();
	}

}

bool AFlyStateMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		return false;
	}

	if (this->GetMouseDeathByBomb())
	{
		this->PlayBombEffAnim();
	}
	else if (this->GetCurrentHP() > 0.f)
	{
		this->UpdateState();
	}

	return true;
}

void AFlyStateMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	//移动了5秒后，才可以就行投掷操作
	if (this->bShootDelay > 0.f)
	{
		this->bShootDelay -= DeltaTime;
	}

	//接触边缘掉落
	if ((this->GetActorLocation().Y <= -80.f) && !this->bFalling && this->bFly)
	{
		if (!this->bFalling && this->bFly)
		{
			this->bFalling = true;
			this->bFallingStart = true;
			this->bFly = false;
			this->MoveStop();
			this->DisEnableCo();
			//掉落，走路
			/*this->SetPlayAnimationOnce(
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Falling),
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Walk)
			);*/

			this->SetAnimation(0,TEXT("SpineTag"),true);
			this->SetAnimation(0,TEXT("SpineTag"),true);
		}
	}

	//移动
	//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

	this->UpdateMove(DeltaTime);
}

void AFlyStateMouse::MouseDeathed()
{
	this->bEnableAttack = false;

	this->DisEnableCo();
	//移除陆地碰撞
	this->MesheComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	//开始空中碰撞
	this->MesheComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);


	Super::MouseDeathed();

	if (!this->GetMouseDeathByBomb())
	{
		//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.IdleDeath), true);

		this->SetAnimation(0,TEXT("SpineTag"),true);
	}
}

void AFlyStateMouse::InMapMeshe(ELineType CurLineType)
{
	if (!this->bFallingStart && !this->bFly)
	{
		Super::InMapMeshe(CurLineType);
	}
	else {
		this->M_Proper_Condition.M_CurrentInType = CurLineType;
	}
}

void AFlyStateMouse::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (this->bFly)
	{
		if (BuffTag == EGameBuffTag::Solidification
			||
			BuffTag == EGameBuffTag::Freeze
			||
			BuffTag == EGameBuffTag::SlowDown
			)
		{
			CurBuffTime = 0.f;
		}
	}

	Super::ExecuteBuff(BuffTag, CurBuffTime);
}

void AFlyStateMouse::OnAnimationPlayEnd()
{
	//掉落开始
	if (this->bFallingStart && !this->bEnableAttack)
	{
		//切换路线【就是单纯更新老鼠管理器，加入到陆地行】
		UMouseManagerComponent::ChangeMouseLineType(this, this->GetMouseLine().Row, ELineType::OnGround);
		//修改碰撞类型
		this->EnableCo(true);
		this->bFallingStart = false;
		this->bEnableAttack = true;
		this->bFly = false;
		this->MoveStart();
		this->CollisionComp->SetRelativeLocation(FVector(0.f, 0.f, 25.f));

		ELineType Temp = this->M_Proper_Condition.M_CurrentInType;
		this->M_Proper_Condition.M_CurrentInType = ELineType::OnGround;
		//手动调用
		this->InMapMeshe(Temp);
	}

	//攻击防御卡
	if (this->bEnableAttack)
	{
		//如果老鼠处于攻击状态
		if (this->GetbAttackState())
		{
			//攻击防御卡
			if (!this->AttackCard())
			{
				//取消攻击
				this->SetbIsAttack(false);
			}
		}
	}

	//动画播放结束时
	if (!this->bShoot && this->bShootDelay <= 0.f && this->bFly)
	{
		//随机,当随机数小于5时，抛射
		if (UKismetMathLibrary::RandomIntegerInRange(0, 300) < 50)
		{
			this->bShoot = true;

			//更换新动画
			//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Fly));

			this->SetAnimation(0,TEXT("SpineTag"),true);

			//生成障碍物
			AObstacleActor* CurBoxClass = this->GetWorld()->SpawnActor<AObstacleActor>(
				UGameSystemFunction::LoadRes(this->MouseBoxClass)
				);
			if (IsValid(CurBoxClass))
			{
				CurBoxClass->SetLine(this->GetMouseLine());
				CurBoxClass->SetActorLocation(this->GetActorLocation());
				CurBoxClass->OnInit();
			}
		}
	}
}

void AFlyStateMouse::OnResourceLoadFinished()
{
}

void AFlyStateMouse::UpdateState()
{
	if (this->GetCurrentHP() > this->GetTotalHP() * 0.8f)
	{
		if (!this->bFalling && this->bFly)
		{
			if (this->bEnableProjection)
			{
				if (this->bShoot)
				{
					//保持飞行[已经投掷]
					//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Fly));

					this->SetAnimation(0,TEXT("SpineTag"),true);
				}
				else {
					//保持飞行[未投掷]
					//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.FlyTake));

					this->SetAnimation(0,TEXT("SpineTag"),true);
				}
			}
			else {
				//保持飞行[无投掷]
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Fly));

				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
		}
		else {
			if (this->GetbIsAttack())
			{
				//攻击
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Attack));

				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
			else {
				//保持走路
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Walk));

				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
		}
	}
	else if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		if (!this->bFalling && this->bFly)
		{
			this->bFalling = true;
			this->bFallingStart = true;
			this->bFly = false;
			this->MoveStop();
			this->DisEnableCo();
			//掉落，走路
		/*	this->SetPlayAnimationOnce(
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Falling),
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Walk)
			);*/

			this->SetAnimation(0,TEXT("SpineTag"),true);
			this->SetAnimation(0,TEXT("SpineTag"),true);
		}
		else {
			//开始掉落并且掉落启动
			if (this->bFalling && this->bFallingStart)
			{
				return;
			}

			if (this->GetbIsAttack())
			{
				//攻击
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Attack));

				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
			else {
				//保持走路
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Walk));

				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
		}
	}
	else if (this->GetCurrentHP() > this->GetTotalHP() * 0.25f)
	{
		if (!this->bFalling && this->bFly)
		{
			this->bFalling = true;
			this->bFallingStart = true;
			this->bFly = false;
			this->MoveStop();
			this->DisEnableCo();
			//掉落，走路
			/*this->SetPlayAnimationOnce(
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Falling),
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.WalkLow)
			);*/

			this->SetAnimation(0,TEXT("SpineTag"),true);
			this->SetAnimation(0,TEXT("SpineTag"),true);
		}
		else {
			//开始掉落并且掉落启动
			if (this->bFalling && this->bFallingStart)
			{
				return;
			}

			if (this->GetbIsAttack())
			{
				//攻击
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.AttackLow));
				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
			else {
				//保持走路
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.WalkLow));
				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
		}
	}
	else if (this->GetCurrentHP() < this->GetTotalHP() * 0.25f && this->GetCurrentHP() > 0.f) {

		if (!this->bFalling && this->bFly)
		{
			this->bFalling = true;
			this->bFallingStart = true;
			this->bFly = false;
			this->MoveStop();
			this->DisEnableCo();
			//掉落，走路
			/*this->SetPlayAnimationOnce(
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Falling),
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Walk)
			);*/

			this->SetAnimation(0,TEXT("SpineTag"),true);
			this->SetAnimation(0,TEXT("SpineTag"),true);
		}
		else {

			//开始掉落并且掉落启动
			if (this->bFalling && this->bFallingStart)
			{
				return;
			}

			if (this->GetbIsAttack())
			{
				//攻击
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.AttackLow));

				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
			else {
				//保持走路
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.WalkLow));

				this->SetAnimation(0,TEXT("SpineTag"),true);
			}
		}
	}
	else {
		if (!this->bFalling && this->bFly)
		{
			this->bFalling = true;
			this->bFallingStart = true;
			this->bFly = false;
			this->MoveStop();
			this->DisEnableCo();
			//掉落，走路
			/*this->SetPlayAnimationOnce(
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Falling),
				UGameSystemFunction::LoadRes(this->FlyStateResourceStruct.Walk)
			);*/

			this->SetAnimation(0,TEXT("SpineTag"),true);
			this->SetAnimation(0,TEXT("SpineTag"),true);
		}
	}
}
