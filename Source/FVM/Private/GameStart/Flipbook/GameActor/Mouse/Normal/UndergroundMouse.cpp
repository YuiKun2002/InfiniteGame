// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/UndergroundMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/MapMeshe.h"

AUndergroundMouse::AUndergroundMouse()
{
	this->MMesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ReMouseMesheComponent"));
	this->MBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ReMouseBodyComponent"));

	//设置依附
	this->MMesheComponent->SetupAttachment(this->GetRootComponent());
	this->MBodyComponent->SetupAttachment(this->MMesheComponent);
}

void AUndergroundMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUndergroundMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bEnableAttakcLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->MECardCollisionType), DeltaTime, false);
	}
}

void AUndergroundMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MMesheComponent, this->MBodyComponent);
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AUndergroundMouse::AnimationPlayEnd);
}

void AUndergroundMouse::MouseInit()
{
	Super::MouseInit();

	this->GetMouseManager()->ChangeMouseLineType(this, this->GetMouseLine().Row, ELineType::Underground);
	this->MMesheComponent->SetCollisionResponseToChannel(
		UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(ELineTraceType::E_MouseGround), ECollisionResponse::ECR_Ignore
	);
	this->MMesheComponent->SetCollisionResponseToChannel(
		UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(ELineTraceType::E_MouseUnder), ECollisionResponse::ECR_Block
	);

	this->bEnableAttakcLine = false;
	this->MouseAnimalRes.bOut = false;
}

void AUndergroundMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);


	if (!this->bEnOut && !this->bFinish)
	{
		if (this->GetActorLocation().Y <= -160.0)
		{
			//播放动画
			this->SetPlayAnimationOnce(
				UGameSystemFunction::LoadRes(this->MouseAnimalRes.Out),
				UGameSystemFunction::LoadRes(this->MouseAnimalRes.Idle)
			);
			this->MoveStop();
			this->bEnOut = true;

			//设置渲染
			int32 CurTran = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMesh(
				this->GetMouseLine().Row,
				AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMeshRowAndCol().Col - 1
			)->GetTranslucency();

			this->SetTranslucentSortPriority(CurTran);

			return;
		}
	}

	if (this->GetActorLocation().Y > 470.f)
	{
		this->SetMouseDeath(true);
		return;
	}


	this->UpdateMove(DeltaTime);
}

void AUndergroundMouse::AttackedBegin()
{
	Super::AttackedBegin();

	this->UpdateState();
}

void AUndergroundMouse::AttackedEnd()
{
	Super::AttackedEnd();

	this->UpdateState();
}

bool AUndergroundMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		return false;
	}

	this->UpdateState();

	return true;
}

void AUndergroundMouse::MouseDeathed()
{
	this->bEnableAttakcLine = false;

	//关闭碰撞
	this->ClosedBoxComponent(this->MMesheComponent);
	this->ClosedBoxComponent(this->MBodyComponent);

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.Death), true);
	}
}

void AUndergroundMouse::AnimationPlayEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	//开启钻出动画
	if (this->bEnOut && !this->MouseAnimalRes.bOut)
	{
		this->bEnOut = false;
		this->MouseAnimalRes.bOut = true;
		return;
	}

	if (this->MouseAnimalRes.bOut && !this->bFinish)
	{
		this->bFinish = true;
		this->bEnableAttakcLine = true;
		this->MoveStart();
		//设置移动速度
		this->SetMoveSpeed(15.f);
		//设置移动方向
		this->SetMouseBaseMoveDirection(false);
		//切换线路类型
		this->GetMouseManager()->ChangeMouseLineType(this, this->GetMouseLine().Row, ELineType::OnGround);
		//设置碰撞
		this->MMesheComponent->SetCollisionResponseToChannel(
			UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(ELineTraceType::E_MouseUnder), ECollisionResponse::ECR_Ignore
		);
		this->MMesheComponent->SetCollisionResponseToChannel(
			UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(ELineTraceType::E_MouseGround), ECollisionResponse::ECR_Block
		);
	}


	this->AddAttackCardUpdate();

	this->UpdateState();
}

void AUndergroundMouse::UpdateState()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (!this->bFinish)
	{
		return;
	}

	bool bATK = this->GetbIsAttack();
	float Cur = this->GetCurrentHP();
	float HP = this->GetTotalHP();

	if (Cur > 0.5f * HP)
	{
		if (bATK)
		{
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.ATK), true);
		}
		else {
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.Walk));
		}
	}
	else {
		if (bATK)
		{
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.ATKLow), true);

		}
		else {
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.WalkLow));
		}
	}

}
