// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/RubbishCardMouse.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"

void ARubbishObstacle::Init(FRubbishCardMouseResourceStruct& Res, const FVector& Location, int32 Translucenty, float Time, float HurtCardRate)
{
	this->RubbishGen = UGameSystemFunction::LoadRes(Res.RubbishGen);
	this->RubbishFinish = UGameSystemFunction::LoadRes(Res.RubbishFinish);

	//初始化旋转
	this->SetActorLocation(Location);
	this->InitRotation();
	this->fTime = 0.5f;
	this->fSpawnDelay = 8.f;
	this->SetRenderLayer(Translucenty);
	this->SetLifeSpan(Time + this->fSpawnDelay);
	this->fRate = HurtCardRate;
}

void ARubbishObstacle::Tick(float DeltaTime)
{
	if (this->bFinish)
	{
		if (this->fTime > 0.f)
		{
			this->fTime -= DeltaTime;
		}
		else {
			this->fTime = 0.5f;

			FHitResult CurActor;
			if (UGameSystemFunction::AddSphereTrace(
				this, CurActor, FVector(), FVector(), 25.f,
				UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor)
			))
			{
				ACardActor* Cur = Cast<ACardActor>(CurActor.GetActor());
				if (IsValid(Cur) && Cur->GetCurrentHP() > 0.f)
				{
					if (Cur->UpdateCardState(Cur->GetTotalHP() * this->fRate, 0.f))
					{
						Cur->KillCard();
					}
				}
			}
		}
	}
	else {
		if (this->fSpawnDelay > 0.f)
		{
			this->fSpawnDelay -= DeltaTime;
			return;
		}
		else {
			if (!this->bFinish)
			{
				this->bFinish = true;
				//播放动画
				this->SetPlayAnimationOnce(this->RubbishGen, this->RubbishFinish);
			}
		}
	}
}

ARubbishCardMouse::ARubbishCardMouse()
{
	///*
	this->MMesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RubbishMouseMesheComp"));
	this->MBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RubbishMouseBodyComp"));
	//设置依附
	this->MMesheComponent->SetupAttachment(this->GetRootComponent());
	this->MBodyComponent->SetupAttachment(this->MMesheComponent);
}

void ARubbishCardMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

}

void ARubbishCardMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MMesheComponent, this->MBodyComponent, FVector2D(35.f, 20.f));
	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ARubbishCardMouse::AnimationPlayEnd);
}

void ARubbishCardMouse::MouseInit()
{
	Super::MouseInit();

	this->LastRubbishObstacle = nullptr;

	this->EnableBoxComponent(this->MMesheComponent);
	this->EnableBoxComponent(this->MBodyComponent);
}

void ARubbishCardMouse::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (
		BuffTag == EGameBuffTag::Freeze
		||
		BuffTag == EGameBuffTag::SlowDown
		||
		BuffTag == EGameBuffTag::Solidification
		)
	{
		CurBuffTime = 0.f;
	}

	Super::ExecuteBuff(BuffTag, CurBuffTime);
}

void ARubbishCardMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}

bool ARubbishCardMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	ACardActor* CurCard = Cast<ACardActor>(CurHitMouseObj);
	if (IsValid(CurCard))
	{
		if (CurCard->GetCardData().M_ECardCollisionType == ECardCollisionType::E_CardActor3)
		{
			if (CurCard->UpdateCardState(1.f, 0.f))
			{
				CurCard->KillCard();
			}

			//设置老鼠死亡
			this->SetMouseDeath(true);

			return false;
		}
	}

	bool Re = Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);

	this->UpdateState();

	return Re;
}

void ARubbishCardMouse::MouseDeathed()
{
	Super::MouseDeathed();

	if (IsValid(this->LastRubbishObstacle))
	{
		this->LastRubbishObstacle->Destroy();
	}

	if (!this->GetPlayPlayBombEffAnim())
	{
		/*this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.Death), true);*/

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}

	this->ClosedBoxComponent(this->MMesheComponent);
	this->ClosedBoxComponent(this->MBodyComponent);
}

void ARubbishCardMouse::AnimationPlayEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	this->UpdateState();
}

void ARubbishCardMouse::UpdateState()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		/*this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.Def));*/
		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else {
		/*this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->MouseAnimalRes.DefLow));*/

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
}

void ARubbishCardMouse::OnSpawnObstacle(AMapMeshe* CurMeshe)
{
	if (!IsValid(CurMeshe))
	{
		return;
	}

	//生成障碍物
	ARubbishObstacle* NewObj = this->GetWorld()->SpawnActor<ARubbishObstacle>();
	if (IsValid(NewObj))
	{

		NewObj->Init(this->MouseAnimalRes,
			CurMeshe->GetActorLocation(),
			TranslucencyConst::GetLayer(TranslucencyConst::ETranslucency::EBgBlock) + 200,
			60.f, 0.5f
		);
	}

	this->LastRubbishObstacle = NewObj;
}
