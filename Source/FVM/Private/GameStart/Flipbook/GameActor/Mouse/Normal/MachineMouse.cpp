// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MachineMouse.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>

AMachineMouse::AMachineMouse()
{
	this->MMesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MMouseMeshComp"));
	this->MBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MMouseBodyComp"));

	//设置依附
	this->MMesheComponent->SetupAttachment(this->GetRootComponent());
	this->MBodyComponent->SetupAttachment(this->MMesheComponent);
}

void AMachineMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMachineMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);
}

void AMachineMouse::BeginPlay()
{
	Super::BeginPlay();

	this->bBomb = false;
	this->bTime = 0.2f;
}

void AMachineMouse::MouseInit()
{
	Super::MouseInit();

	UGameSystemFunction::InitMouseMeshe(this->MMesheComponent, this->MBodyComponent);
	//初始化碰撞
	this->GetMouseManager()->ChangeMouseLineType(
		this, this->GetMouseLine().Row, ELineType::OnGround, this->MMesheComponent, this->MBodyComponent
	);

	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMachineMouse::OnAnimationPlayEnd);
}

void AMachineMouse::MoveingUpdate(float DeltaTime)
{

	if (this->bBomb)
	{
		return;
	}

	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);

	//检查周围是否有障碍物
	if (this->bTime > 0.f)
	{
		this->bTime -= DeltaTime;
	}
	else {
		this->bTime = 0.2f;

		FHitResult CurHit;
		if (UGameSystemFunction::AddSphereTrace(
			this, CurHit, Offset, Offset, 5.f,
			UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor2)
		))
		{
			ACardActor* Cur = Cast<ACardActor>(CurHit.GetActor());
			if (IsValid(Cur))
			{
				this->ClosedBoxComponent(this->MBodyComponent);
				this->ClosedBoxComponent(this->MMesheComponent);
				this->bBomb = true;
				if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
				{
					//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Idle), true);

					this->SetAnimation(0, TEXT("SpineTag"), true);

				}
				else {
					//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.IdleLow), true);

					this->SetAnimation(0, TEXT("SpineTag"), true);
				}
			}
		}
	}
}

bool AMachineMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);

	this->UpdateState();

	return true;
}

void AMachineMouse::MouseDeathed()
{
	Super::MouseDeathed();

	this->ClosedBoxComponent(this->MBodyComponent);
	this->ClosedBoxComponent(this->MMesheComponent);

	//如果没有触发爆炸
	if (!this->bBomb)
	{
		if (!this->GetPlayPlayBombEffAnim())
		{
			//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Death), true);

			this->SetAnimation(0, TEXT("SpineTag"), true);
		}
	}

}

void AMachineMouse::InMapMeshe(ELineType CurLineType)
{
	Super::InMapMeshe(CurLineType);

	if (CurLineType == ELineType::OnWater)
	{
		Offset = FVector(0.f, 0.f, 20.f);
	}
	else {
		Offset = FVector::ZeroVector;
	}
}

void AMachineMouse::OnAnimationPlayEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->bBomb)
	{
		//this->SetPlayAnimation(nullptr);

		this->SetEmptyAnimation(0, 0.2f);

		//生成爆炸对象
		UClass* CurBomb = UGameSystemFunction::LoadRes(this->BombClass);
		if (IsValid(CurBomb))
		{
			AMachineBombAnim* NewBombActor = this->GetWorld()->SpawnActor<AMachineBombAnim>(CurBomb);
			NewBombActor->SetActorLocation(this->GetActorLocation());
			NewBombActor->OnInit();
		}

		this->SetMouseDeath(true);

		return;
	}

	this->UpdateState();
}

void AMachineMouse::UpdateState()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (!bBomb)
	{
		if (this->GetCurrentHP() > this->GetTotalHP() * 0.5)
		{
			//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.Def));

			this->SetAnimation(0, TEXT("SpineTag"), true);
		}
		else {
			//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimRes.DefLow));

			this->SetAnimation(0, TEXT("SpineTag"), true);
		}
	}
}
