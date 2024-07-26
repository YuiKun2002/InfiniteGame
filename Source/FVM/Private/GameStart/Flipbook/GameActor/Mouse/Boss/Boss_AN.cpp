// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_AN.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/MachineMouse.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/BoxComponent.h> 
#include <Components/SphereComponent.h> 
#include "GameStart/VS/MapMeshe.h"

ABoss_AN::ABoss_AN()
{
	this->MMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheBox"));
	this->MBody = CreateDefaultSubobject<USphereComponent>(TEXT("SphereBody"));

	this->MMesh->SetupAttachment(this->GetRootComponent());
	this->MBody->SetupAttachment(this->MMesh);
}

void ABoss_AN::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MMesh, this->MBody, FVector2D(65.f, 15.f), FVector2D(0.f, 0.f), true);
	UGameSystemFunction::InintMouseCollisonBySphere(this->MBody, 55.f);
}

void ABoss_AN::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(TEXT("阿诺"), TEXT("577"));

	if (IsValid(this->GetMouseManager()))
	{
		this->GetMouseManager()->ChangeMouseLineType(
			this, this->GetMouseLine().Row, ELineType::OnGround, this->MMesh, this->MBody
		);
	}


	this->ChangeState(NewObject<UAN_Out>());
}

void ABoss_AN::MouseDeathed()
{
	Super::MouseDeathed();

	this->SetCollision(false);

	this->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->DeathAnim), true
	);
}

void ABoss_AN::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	Super::ExecuteBuff(BuffTag, CurBuffTime);
}

void ABoss_AN::ExitBuff(EGameBuffTag BuffTag)
{
	Super::ExitBuff(BuffTag);
}


void ABoss_AN::SetCollision(bool bEnable)
{
	if (bEnable)
	{
		this->EnableBoxComponent(this->MBody);
		this->EnableBoxComponent(this->MMesh);
	}
	else {
		this->ClosedBoxComponent(this->MBody);
		this->ClosedBoxComponent(this->MMesh);
	}
}

void UAN_Out::Init()
{
	//显示boss
	this->Get()->SetActorHiddenInGame(false);
	this->Get()->UpdateBossOpacity(1.f);

	if (this->Get()->GetMouseLevel() > 2)
	{
		this->Get()->UpdateBossOpacity(0.f);
		//初始化时间线
		this->OutTimeLine = UTimeLineClass::MakeTimeLineClass();
		this->OutTimeLine->AddCurve(
			Cast<ABoss_AN>(this->Get())->MouseOutScaleCurve, this,
			[](UTimeLineClass* time, UObject* obj, float value) {
				UAN_Out* CurM = Cast<UAN_Out>(obj);
				ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
				FVector NewS(1.f);
				Cur->SetActorScale3D(NewS * value);
			},
			[](UTimeLineClass* time, UObject* obj)
			{
				//切换状态
				UAN_Out* CurM = Cast<UAN_Out>(obj);
				ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
				Cur->SetActorScale3D(FVector(1.f));
			}
			);
		this->OutTimeLine->AddCurve(
			Cast<ABoss_AN>(this->Get())->MouseOpacityCurve, this,
			[](UTimeLineClass* time, UObject* obj, float value) {
				UAN_Out* CurM = Cast<UAN_Out>(obj);
				ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
				Cur->UpdateBossOpacity(value);
			},
			[](UTimeLineClass* time, UObject* obj)
			{
				//切换状态
				UAN_Out* CurM = Cast<UAN_Out>(obj);
				ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
				Cur->UpdateBossOpacity(1.f);
			}
			);
	}


	//随机切换一次线路
	int32 RanRow = UGameSystemFunction::GetRandomRange(0,
		this->Get()->GetMesheComp()->GetMapMeshRowAndCol().Row - 1
	);

	this->Get()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->Get()),
		this->Get()->GetMouseLine().Row,
		RanRow,
		this->Get()->GetMesheComp()->GetMapMeshRowAndCol().Col - 1
	);

	//设置移动位置[取消入水，出池的位移动画，防止位置冲突]
	this->Get()->InMapMeshe(ELineType::OnGround);
	this->Get()->CloseInWaterTimeLine();

	//设置第一次的随机移动位置
	FVector FirstVec = this->Get()->GetMesheComp()->GetMapMeshLocation(
		RanRow, this->Get()->GetMouseLine().Col
	);
	FirstVec.Y = MOUSE_BOSS_Y_LOCATION;

	this->Get()->SetActorLocation(FirstVec);

	this->Get()->SetTranslucentSortPriority(
		this->Get()->GetMesheComp()->GetMapMesh(RanRow, this->Get()->GetMouseLine().Col)->GetTranslucency()
	);

	this->RanCount = 5;

	ABoss_AN* An = Cast<ABoss_AN>(this->Get());
	this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(An->OutAnim));
}

void UAN_Out::MouseTick(const float& DeltaTime) {
	if (IsValid(this->OutTimeLine))
	{
		this->OutTimeLine->Tick(DeltaTime);
	}
}

void UAN_Out::MoveingUpdate(const float& DeltaTime) {}

void UAN_Out::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) {}

void UAN_Out::AnimPlayEnd()
{
	int32 Level = this->Get()->GetMouseLevel();
	if (Level <= 2)
	{
		UAN_SkillSelect* NewState = NewObject<UAN_SkillSelect>();
		this->Get()->ChangeState(NewState);
		NewState->Select(Level);

		if (Level == 1)
		{
			this->Get()->DefSpawnMouseTemp(1, Cast<ABoss_AN>(this->Get())->SpawnMouseNames);
		}
		else {
			if (UGameSystemFunction::GetRange(30, 100))
			{
				this->Get()->DefSpawnMouseTemp(2, Cast<ABoss_AN>(this->Get())->SpawnMouseNames);
			}
			else {
				this->Get()->DefSpawnMouseTemp(1, Cast<ABoss_AN>(this->Get())->SpawnMouseNames);
			}
		}
	}
	else {
		if (this->RanCount > 0)
		{
			this->RanCount--;
			this->RandomChangePosition();
		}
		else {
			UAN_SkillSelect* NewState = NewObject<UAN_SkillSelect>();
			this->Get()->ChangeState(NewState);
			NewState->Select(Level);

			if (UGameSystemFunction::GetRange(20, 100))
			{
				this->Get()->DefSpawnMouseTemp(3, Cast<ABoss_AN>(this->Get())->SpawnMouseNames);
			}
			else {
				this->Get()->DefSpawnMouseTemp(1, Cast<ABoss_AN>(this->Get())->SpawnMouseNames);
			}
		}
	}
}


void UAN_Out::RandomChangePosition()
{
	if (IsValid(this->OutTimeLine))
	{
		this->OutTimeLine->PlayFromStart();
	}

	int32 RanRow = UGameSystemFunction::GetRandomRange(0, this->Get()->GetMesheComp()->GetMapMeshRowAndCol().Row - 1);
	int32 RanCol = UGameSystemFunction::GetRandomRange(5, this->Get()->GetMesheComp()->GetMapMeshRowAndCol().Col - 1);

	//切换路线
	this->Get()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->Get()),
		this->Get()->GetMouseLine().Row, RanRow,
		RanCol
	);

	//设置移动位置[取消入水，出池的位移动画，防止位置冲突]
	this->Get()->InMapMeshe(
		this->Get()->GetMesheComp()->GetMapMesh(RanRow, RanCol)->GetLineType()
	);
	this->Get()->CloseInWaterTimeLine();

	//设置位置
	this->Get()->SetActorLocation(
		this->Get()->GetMesheComp()->GetMapMesh(RanRow, RanCol)->GetActorLocation()
	);

	//设置渲染层
	this->Get()->SetTranslucentSortPriority(
		this->Get()->GetMesheComp()->GetMapMesh(RanRow, RanCol)->GetTranslucency()
	);
}

void UAN_SkillSelect::Select(int32 MouseLevel)
{
	if (UGameSystemFunction::GetRange(45, 100))
	{
		UAN_Move* NewState = NewObject<UAN_Move>();
		NewState->Level = MouseLevel;
		this->Get()->ChangeState(NewState);
	}
	else {
		UAN_Shoot* NewState = NewObject<UAN_Shoot>();
		if (MouseLevel <= 2)
		{
			NewState->bRandom = false;
			if (MouseLevel == 1)
			{
				NewState->ShootCount = 1;
			}
			else {
				NewState->ShootCount = 2;
			}
		}
		else {
			NewState->bRandom = true;
			NewState->ShootCount = UGameSystemFunction::GetRandomRange(1, 2);
		}

		this->Get()->ChangeState(NewState);
	}
}

void UAN_Move::Init()
{
	if (this->Get()->GetCurrentHP() > this->Get()->GetTotalHP() * 0.5f)
	{
		this->Get()->SetPlayAnimation(
			UGameSystemFunction::LoadRes((Cast<ABoss_AN>(this->Get()))->MoveReadyAnim));
	}
	else {
		this->Get()->SetPlayAnimation(
			UGameSystemFunction::LoadRes((Cast<ABoss_AN>(this->Get()))->MoveReadyLowAnim));
	}

	if (this->Get()->GetMouseLevel() == 2)
	{
		this->Get()->DefSpawnMouse(2);
	}
	else if (this->Get()->GetMouseLevel() >= 3)
	{
		this->Get()->DefSpawnMouse(3);
	}

	this->PlayCount = 3;
	this->bInitAnimEnd = false;
}

void UAN_Move::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->TimeLine))
	{
		this->TimeLine->Tick(DeltaTime);
	}
}

void UAN_Move::AnimPlayEnd()
{
	if (this->bInitAnimEnd)
	{
		return;
	}
	else {
		if (this->PlayCount > 0)
		{
			this->PlayCount--;
			return;
		}

		this->PlayAnim(
			UGameSystemFunction::LoadRes((Cast<ABoss_AN>(this->Get()))->MoveAnim),
			UGameSystemFunction::LoadRes((Cast<ABoss_AN>(this->Get()))->LowAnim)
		);

		this->bInitAnimEnd = true;

		this->TimeLine = UTimeLineClass::MakeTimeLineClass();
		this->CurLocation = this->Get()->GetActorLocation();
		if (this->Level <= 2)
		{
			this->TargetLocation = this->Get()->GetMesheComp()->
				GetMapMeshLocation(this->Get()->GetMouseLine().Row, this->Get()->GetMouseLine().Col - 2);
		}
		else {
			this->TargetLocation = this->Get()->GetMesheComp()->
				GetMapMeshLocation(this->Get()->GetMouseLine().Row, 2);
		}

		this->TimeLine->AddCurve(
			Cast<ABoss_AN>(this->Get())->MouseMoveCurve, this,
			[](UTimeLineClass* time, UObject* obj, float value) {
				UAN_Move* CurM = Cast<UAN_Move>(obj);
				ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
				FVector L = CurM->CurLocation;
				L.Z = Cur->GetActorLocation().Z;
				FVector R = CurM->TargetLocation;
				R.Z = Cur->GetActorLocation().Z;

				Cur->SetActorLocation(
					UKismetMathLibrary::VLerp(L, R, value)
				);

				UGameSystemFunction::AddLineTrance(Cur->GetWorld(),
					Cur->GetActorLocation(), FVector(0.f, -30.f, 0.f), FVector::ZeroVector,
					UGameSystemFunction::GetCardCollisionBoxType(ECardCollisionType::E_CardActor),
					Cur,
					[](UObject* Obj, AActor* HitA) {
						ACardActor* Card = Cast<ACardActor>(HitA);
						if (IsValid(Card))
						{
							Card->UpdateCardState(999999, 0.f);
							Card->KillCard();
							UResourceManagerComponent::ResourceAddBadCard();
						}
					}
				);

			},
			[](UTimeLineClass* time, UObject* obj)
			{
				//切换状态
				UAN_Move* CurM = Cast<UAN_Move>(obj);
				ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
				Cur->ChangeState(NewObject<UAN_Hidd>());
			}
			);

		this->TimeLine->PlayFromStart();
	}
}

void UAN_Hidd::Init()
{
	ABoss_AN* An = Cast<ABoss_AN>(this->Get());
	this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(An->OutAnim));
	bHid = false;
	this->HidTime = 10.f;

	this->TimeLine = UTimeLineClass::MakeTimeLineClass();
	this->TimeLine->AddCurve(
		Cast<ABoss_AN>(this->Get())->MouseMoveCurve, this->Get(),
		[](UTimeLineClass* time, UObject* obj, float value) {
			ABoss_AN* Cur = Cast<ABoss_AN>(obj);
			Cur->UpdateBossOpacity(1.f - value);
		},
		[](UTimeLineClass* time, UObject* obj)
		{
			ABoss_AN* Cur = Cast<ABoss_AN>(obj);
			Cur->AddActorLocalOffset(FVector(0.f, 2000.f, 0.f));
			Cur->InMapMeshe(ELineType::OnGround);
			Cur->CloseInWaterTimeLine();
		}
		);

	this->TimeLine->PlayFromStart();
}

void UAN_Hidd::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->TimeLine))
	{
		this->TimeLine->Tick(DeltaTime * 3.f);
	}

	if (this->bHid)
	{
		if (this->HidTime > 0.f)
		{
			this->HidTime -= DeltaTime;
		}
		else {
			this->Get()->ChangeState(NewObject<UAN_Out>());
		}
	}
}

void UAN_Hidd::AnimPlayEnd()
{
	if (this->bHid)
	{
		return;
	}
	else {
		bHid = true;
		//隐藏boss
		this->Get()->SetActorHiddenInGame(true);
	}
}

void UAN_Shoot::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes((Cast<ABoss_AN>(this->Get()))->ShootAnim),
		UGameSystemFunction::LoadRes((Cast<ABoss_AN>(this->Get()))->ShootLowAnim)
	);

	this->bOut = false;
}

void UAN_Shoot::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->OutTimeLine))
	{
		this->OutTimeLine->Tick(DeltaTime);
	}
}

void UAN_Shoot::AnimPlayEnd()
{
	if (this->bOut)
	{
		return;
	}

	if (this->ShootCount > 0)
	{
		this->ShootCount--;

		if (this->bRandom)
		{
			this->Ran();
		}
		else {
			this->Def();
		}
		if (this->ShootCount == 0)
		{
			this->Out();
			this->bOut = true;
		}
	}
	else {
		this->Out();
		this->bOut = true;
		//this->Get()->ChangeState(NewObject<UAN_Out>());
	}
}

void UAN_Shoot::Ran()
{
	int32 CurRow = this->Get()->GetMouseLine().Row;
	TArray<FVector> TargetLocations;
	TArray<AMapMeshe*> CurMeshes;
	for (int32 i = 0; i < this->Get()->GetMesheComp()->GetMapMeshRowAndCol().Row; i++)
	{
		for (int32 j = 0; j < this->Get()->GetMouseLine().Col; j++)
		{
			AMapMeshe* Cur = this->Get()->GetMesheComp()->GetMapMesh(i, j);
			if (IsValid(Cur) && IsValid(Cur->GetUIMapMeshe()))
			{
				if (
					IsValid(Cur->GetUIMapMeshe()->GetCardByID(0))
					||
					IsValid(Cur->GetUIMapMeshe()->GetCardByID(1))
					)
				{
					CurMeshes.Emplace(Cur);
				}
			}
		}
	}

	this->RanSelect(2, CurMeshes, TargetLocations);

	for (const FVector& TargetLocation : TargetLocations)
	{
		Cast<ABoss_AN>(this->Get())->SpawnBullet(this->Get()->GetActorLocation(), TargetLocation);
	}
}

void UAN_Shoot::RanSelect(int32 GetCount, const TArray<AMapMeshe*>& InputMeshe, TArray<FVector>& OutLocation)
{
	if (GetCount == 0)
	{
		return;
	}

	if (InputMeshe.Num() == 0)
	{
		return;
	}

	TArray<AMapMeshe*> CurTemp = InputMeshe;
	int32 IIndex = UGameSystemFunction::GetRandomRange(0, CurTemp.Num() - 1);
	OutLocation.Emplace(CurTemp[IIndex]->GetActorLocation());
	CurTemp.RemoveAt(IIndex);

	//选择目标
	RanSelect(GetCount - 1, CurTemp, OutLocation);
}

void UAN_Shoot::Def()
{
	FVector TargetLocation;
	int32 CurRow = this->Get()->GetMouseLine().Row;
	bool bRe = false;
	for (int32 i = 0; i < this->Get()->GetMesheComp()->GetMapMeshRowAndCol().Col; i++)
	{
		AMapMeshe* Cur = this->Get()->GetMesheComp()->GetMapMesh(CurRow, i);
		if (IsValid(Cur) && IsValid(Cur->GetUIMapMeshe()))
		{
			if (
				IsValid(Cur->GetUIMapMeshe()->GetCardByID(0))
				||
				IsValid(Cur->GetUIMapMeshe()->GetCardByID(1))
				)
			{
				bRe = true;
				TargetLocation = Cur->GetActorLocation();
				break;
			}
		}
	}

	if (bRe)
	{
		Cast<ABoss_AN>(this->Get())->SpawnBullet(this->Get()->GetActorLocation(), TargetLocation);
	}
}

void UAN_Shoot::Out()
{
	this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes((Cast<ABoss_AN>(this->Get()))->OutAnim));

	this->Get()->UpdateBossOpacity(0.f);
	//初始化时间线
	this->OutTimeLine = UTimeLineClass::MakeTimeLineClass();
	this->OutTimeLine->AddCurve(
		Cast<ABoss_AN>(this->Get())->MouseOutScaleCurve, this,
		[](UTimeLineClass* time, UObject* obj, float value) {
			UAN_Shoot* CurM = Cast<UAN_Shoot>(obj);
			ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
			FVector NewS(1.f);
			Cur->SetActorScale3D(NewS * value);
		},
		[](UTimeLineClass* time, UObject* obj)
		{
			//切换状态
			UAN_Shoot* CurM = Cast<UAN_Shoot>(obj);
			ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
			Cur->SetActorScale3D(FVector(1.f));

			//切换状态【】
			CurM->Get()->ChangeState(NewObject<UAN_Out>());
		}
		);
	this->OutTimeLine->AddCurve(
		Cast<ABoss_AN>(this->Get())->MouseOpacityCurve, this,
		[](UTimeLineClass* time, UObject* obj, float value) {
			UAN_Shoot* CurM = Cast<UAN_Shoot>(obj);
			ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
			Cur->UpdateBossOpacity(value);
		},
		[](UTimeLineClass* time, UObject* obj)
		{
			//切换状态
			UAN_Shoot* CurM = Cast<UAN_Shoot>(obj);
			ABoss_AN* Cur = Cast<ABoss_AN>(CurM->Get());
			Cur->UpdateBossOpacity(1.f);
		}
		);
	this->OutTimeLine->PlayFromStart();
}
