// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/DoubleStateMouse.h"
#include <Kismet/KismetMathLibrary.h>
#include "GameSystem/Tools/GameSystemFunction.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

ADoubleStateMouse::ADoubleStateMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void ADoubleStateMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MesheComp,this->CollisionComp);

	//绑定动画播放结束
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ADoubleStateMouse::OnAnimationPlayEnd);
}

void ADoubleStateMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bShoot)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime);
	}
}

bool ADoubleStateMouse::BeHit(UObject* CurHitMouseObj,float _HurtValue, EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj,_HurtValue, AttackType))
	{
		return false;
	}

	if (this->GetMouseDeathByBomb())
	{
		this->PlayBombEffAnim();

		return true;
	}

	//如果已经抛出炸弹
	if (this->bShoot)
	{
		//更新默认状态
		if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
		{
			//更换新动画
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.Idle));
		}
		else {
			//更换新动画[残血]
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.IdleLow));
		}
	}
	else
	{
		//更新拿着炸弹的状态
		if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
		{
			//更换新动画
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.IdleTake));
		}
		else {
			//更换新动画[残血]
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.IdleTakeLow));
		}
	}


	//当受到攻击时
	this->UpdateHP(_HurtValue);

	return true;
}

void ADoubleStateMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	//移动了3秒后，才可以就行投掷操作
	if (this->bShootDelay > 0.f)
	{
		this->bShootDelay -= DeltaTime;
	}

	//移动
	//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

	this->UpdateMove(DeltaTime);

}

void ADoubleStateMouse::MouseDeathed()
{
	Super::MouseDeathed();

	//关闭碰撞
	this->ClosedBoxComponent(this->MesheComp);
	this->ClosedBoxComponent(this->CollisionComp);


	if (!this->GetMouseDeathByBomb())
	{
		//老鼠死亡
		if (this->bShoot)
		{
			//更换新动画
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.IdleDeath));
		}
		else
		{
			//更换新动画
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.IdleTakeDeath));
		}
	}
}

void ADoubleStateMouse::InMapMeshe(ELineType CurLineType)
{
	this->M_Proper_Condition.M_CurrentInType = CurLineType;
}

void ADoubleStateMouse::OnAnimationPlayEnd()
{
	//如果老鼠处于攻击状态
	if (this->bShoot)
	{
		//攻击卡片
		this->AddAttackCardUpdate();
	}

	//动画播放结束时
	if (!this->bShoot && this->bShootDelay <= 0.f)
	{
		//随机,当随机数小于5时，抛射
		if (UKismetMathLibrary::RandomIntegerInRange(0, 300) < 50)
		{
			this->bShoot = true;

			if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
			{
				//更换新动画
				this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.Idle));
			}
			else {
				//更换新动画[残血]
				this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->DoubleStateResourceStruct.IdleLow));
			}

			//生成炸弹
			AObstacleActor* CurBombClass = this->GetWorld()->SpawnActor<AObstacleActor>(UGameSystemFunction::LoadRes(this->MouseBombClass));
			if (IsValid(CurBombClass))
			{
				CurBombClass->SetLine(this->GetMouseLine());
				CurBombClass->SetActorLocation(this->GetActorLocation());
				CurBombClass->OnInit();
			}
		}
	}
}
