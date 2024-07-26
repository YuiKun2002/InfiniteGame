// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_HLL.h"
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>

ABoss_HLL::ABoss_HLL() : ABossSphereBase() {}

void ABoss_HLL::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞位置
	UGameSystemFunction::InitMouseMeshe(
		this->MMeshe,
		this->MBody,
		FVector2D(50.f, 20.f),
		FVector2D(0.f),
		true
	);
}

void ABoss_HLL::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(TEXT("轰隆隆"), TEXT("581"));

	this->ChangeState(NewObject<UHLL_CreateState>());
}

void ABoss_HLL::MouseDeathed()
{
	Super::MouseDeathed();

	this->ClosedBoxComponent(this->MMeshe);
	this->ClosedBoxComponent(this->MBody);

	this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->AnimA_Death), true);
}

void ABoss_HLL::InMapMeshe(ELineType CurLineType)
{
	if (this->M_Proper_Condition.M_CurrentInType == CurLineType)
	{
		return;
	}

	this->ForceSetWaterAnimShow(CurLineType);
	this->M_Proper_Condition.M_CurrentInType = CurLineType;
}

ABoss_HLL* UHLL_BaseState::GetHLL()
{
	return Cast<ABoss_HLL>(this->Get());
}

void UHLL_BaseState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{

}

void UHLL_BaseState::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	CurBuffTime = 0.f;
}

void UHLL_CreateState::Init()
{
	Super::Init();

	this->PlayOpacity(true);

	//随机选择路线
	int32 RanRow = UGameSystemFunction::GetRandomRange(0, this->GetRowAndCol().Row - 1);
	int32 ConstCol = this->GetRowAndCol().Col - 1;

	//切换路线
	this->Get()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->Get()),
		this->GetLine().Row, RanRow, ConstCol
	);

	//切换类型
	this->GetHLL()->GetMouseManager()->ChangeMouseLineType(
		this->GetHLL(), this->GetHLL()->GetMouseLine().Row,
		ELineType::Sky, this->GetHLL()->MMeshe, this->GetHLL()->MBody
	);

	//设置老鼠进入网格
	this->Get()->InMapMeshe(ELineType::Sky);
	this->Get()->SetInWaterAnimEnable(false);

	//设置老鼠目标位置
	this->Get()->SetActorLocation(this->GetMesheLocation(RanRow, ConstCol));

	this->GetHLL()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_Create));
}

void UHLL_CreateState::AnimPlayEnd()
{
	this->GetHLL()->ChangeState(NewObject<UHLL_FlyState>());
}

void UHLL_FlyState::Init()
{
	//飞行
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_Fly),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FlyLow)
	);

	//切换类型
	this->GetHLL()->GetMouseManager()->ChangeMouseLineType(
		this->GetHLL(), this->GetHLL()->GetMouseLine().Row,
		ELineType::Sky, this->GetHLL()->MMeshe, this->GetHLL()->MBody
	);

	//设置老鼠进入网格
	this->Get()->InMapMeshe(ELineType::Sky);
	this->Get()->CloseInWaterTimeLine();
}

void UHLL_FlyState::AnimPlayEnd()
{
	this->GetHLL()->ChangeState(NewObject<UHLL_FlyItemLastMoveState>());
}

void UHLL_FlyItemLastMoveState::Init()
{
	this->IdleCount = 3;

	//飞行
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FlyReady),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FlyReadyLow)
	);
}

void UHLL_FlyItemLastMoveState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3)
	{
		if (AttackType == EFlyItemAttackType::Bomb)
		{
			_HurtValue = _HurtValue * 0.2f;
		}
	}
}

void UHLL_FlyItemLastMoveState::AnimPlayEnd()
{
	this->IdleCount--;
	if (this->IdleCount <= 0)
	{
		//选择前进点位
		this->SelectPoint();
	}
}

void UHLL_FlyItemLastMoveState::SelectPoint()
{
	//获取点位
	FLine CurLine = this->GetRowAndCol();

	//排除0和最后一行
	TArray<int32> CurRows;
	for (int32 CurRow = 1; CurRow < CurLine.Row - 1; CurRow++)
	{
		CurRows.Emplace(CurRow);
	}
	//随机选取点位
	int32 RanRowIndex = UGameSystemFunction::GetRandomRange(0, CurRows.Num() - 1);
	int32 CurRow = CurRows[RanRowIndex];

	//排除第1列和最后一列
	TArray<int32> CurCols;
	for (int32 CurCol = 1; CurCol < CurLine.Col - 4; CurCol++)
	{
		CurCols.Emplace(CurCol);
	}
	//随机选取点位
	int32 RanColIndex = UGameSystemFunction::GetRandomRange(0, CurCols.Num() - 1);
	int32 CurCol = CurCols[RanColIndex];

	//目标路线
	FLine TargetNewLine(CurRow, CurCol);

	UHLL_FlyForwardState* CurState = NewObject<UHLL_FlyForwardState>();
	this->Get()->ChangeState(CurState);
	CurState->PlayMove(TargetNewLine);
}

void UHLL_FlyForwardState::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_ForwardFly),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_ForwardFlyLow)
	);

	this->CurTimeLine = UTimeLineClass::MakeTimeLineClass();
}

void UHLL_FlyForwardState::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->CurTimeLine))
	{
		this->CurTimeLine->Tick(DeltaTime);
	}
}

void UHLL_FlyForwardState::PlayMove(FLine NewLine)
{
	this->CurLine = NewLine;

	this->Location = this->Get()->GetActorLocation();
	this->TargetLocation = this->GetMesheLocation(NewLine.Row, NewLine.Col);

	this->CurTimeLine->AddCurve(
		this->GetHLL()->BossMoveCurve, this,
		[](UTimeLineClass* timeline, UObject* Obj, float value) {
			UHLL_FlyForwardState* Cur = Cast<UHLL_FlyForwardState>(Obj);
			Cur->Get()->SetActorLocation(
				UKismetMathLibrary::VLerp(
					Cur->Location,
					Cur->TargetLocation,
					value
				)
			);
		},
		[](UTimeLineClass* timeline, UObject* Obj) {
			UHLL_FlyForwardState* Cur = Cast<UHLL_FlyForwardState>(Obj);
			//切换老鼠线路
			Cur->Get()->GetMouseManager()->ForceChangeLine(
				UGameSystemFunction::GetObjectName(Cur->GetHLL()),
				Cur->Get()->GetMouseLine().Row,
				Cur->CurLine.Row,
				Cur->CurLine.Col
			);
			Cur->Get()->ChangeState(NewObject<UHLL_FallATKtate>());
		}
		);

	this->Get()->SetInWaterAnimEnable(false);
	this->Get()->ForceSetWaterAnimShow(ELineType::Sky);
	this->CurTimeLine->PlayFromStart();
}

void UHLL_FlyForwardState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3)
	{
		if (AttackType == EFlyItemAttackType::Bomb)
		{
			_HurtValue = _HurtValue * 0.2f;
		}
	}
}

void UHLL_FallATKtate::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FallATK),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FallATKLow)
	);

	//切换类型
	this->GetHLL()->GetMouseManager()->ChangeMouseLineType(
		this->GetHLL(), this->GetHLL()->GetMouseLine().Row,
		ELineType::OnGround, this->GetHLL()->MMeshe, this->GetHLL()->MBody
	);

	//设置老鼠进入网格
	this->Get()->InMapMeshe(
		this->GetMeshe(this->GetHLL()->GetMouseLine().Row,
			this->GetHLL()->GetMouseLine().Col)->GetLineType()
	);
	this->Get()->CloseInWaterTimeLine();

	//生成伤害
	this->GetHLL()->SpawnRange();
}

void UHLL_FallATKtate::AnimPlayEnd()
{
	//切换状态
	this->Get()->ChangeState(NewObject<UHLL_FlySecondState>());
}

void UHLL_FlySecondState::Init()
{
	//切换类型
	this->GetHLL()->GetMouseManager()->ChangeMouseLineType(
		this->GetHLL(), this->GetHLL()->GetMouseLine().Row,
		ELineType::Sky, this->GetHLL()->MMeshe, this->GetHLL()->MBody
	);
	//设置老鼠进入网格
	this->Get()->InMapMeshe(ELineType::Sky);
	this->Get()->CloseInWaterTimeLine();

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_Fly),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FlyLow)
	);
}

void UHLL_FlySecondState::AnimPlayEnd()
{
	this->Get()->ChangeState(NewObject<UHLL_FlyICurMoveState>());
}

void UHLL_FlySecondState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3)
	{
		if (AttackType == EFlyItemAttackType::Bomb)
		{
			_HurtValue = _HurtValue * 0.2f;
		}
	}
}

void UHLL_FlyICurMoveState::Init()
{
	this->IdleCount = 3;

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FlyReady),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FlyReadyLow)
	);
}

void UHLL_FlyICurMoveState::AnimPlayEnd()
{
	this->IdleCount--;
	if (this->IdleCount <= 0)
	{
		this->SelectPoint();
	}
}

void UHLL_FlyICurMoveState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3)
	{
		if (AttackType == EFlyItemAttackType::Bomb)
		{
			_HurtValue = _HurtValue * 0.2f;
		}
	}
}

void UHLL_FlyICurMoveState::SelectPoint()
{
	//获取点位
	FLine CurLine = this->GetRowAndCol();

	//排除0和最后一行
	TArray<int32> CurRows;
	for (int32 CurRow = 0; CurRow < CurLine.Row; CurRow++)
	{
		CurRows.Emplace(CurRow);
	}
	//随机选取点位
	int32 RanRowIndex = UGameSystemFunction::GetRandomRange(0, CurRows.Num() - 1);
	int32 CurRow = CurRows[RanRowIndex];

	//目标路线
	FLine TargetNewLine(CurRow, CurLine.Col - 1);

	UHLL_FlyBackwardState* CurState = NewObject<UHLL_FlyBackwardState>();
	this->Get()->ChangeState(CurState);
	CurState->PlayMove(TargetNewLine);
}

void UHLL_FlyBackwardState::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_BackwardFly),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_BackwardFlyLow)
	);

	this->CurTimeLine = UTimeLineClass::MakeTimeLineClass();
}

void UHLL_FlyBackwardState::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->CurTimeLine))
	{
		this->CurTimeLine->Tick(DeltaTime);
	}
}

void UHLL_FlyBackwardState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3)
	{
		if (AttackType == EFlyItemAttackType::Bomb)
		{
			_HurtValue = _HurtValue * 0.2f;
		}
	}
}

void UHLL_FlyBackwardState::PlayMove(FLine NewLine)
{
	this->CurLine = NewLine;

	this->Location = this->Get()->GetActorLocation();
	this->TargetLocation = this->GetMesheLocation(NewLine.Row, NewLine.Col);

	this->CurTimeLine->AddCurve(
		this->GetHLL()->BossMoveCurve, this,
		[](UTimeLineClass* timeline, UObject* Obj, float value) {
			UHLL_FlyBackwardState* Cur = Cast<UHLL_FlyBackwardState>(Obj);
			Cur->Get()->SetActorLocation(
				UKismetMathLibrary::VLerp(
					Cur->Location,
					Cur->TargetLocation,
					value
				)
			);
		},
		[](UTimeLineClass* timeline, UObject* Obj) {
			UHLL_FlyBackwardState* Cur = Cast<UHLL_FlyBackwardState>(Obj);
			//切换老鼠线路
			Cur->Get()->GetMouseManager()->ForceChangeLine(
				UGameSystemFunction::GetObjectName(Cur->GetHLL()),
				Cur->Get()->GetMouseLine().Row,
				Cur->CurLine.Row,
				Cur->CurLine.Col
			);
			Cur->Get()->ChangeState(NewObject<UHLL_FlyFalltate>());
		}
		);

	this->Get()->SetInWaterAnimEnable(false);
	this->Get()->ForceSetWaterAnimShow(ELineType::Sky);
	this->CurTimeLine->PlayFromStart();
}

void UHLL_FlyFalltate::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_Fall),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_FallLow)
	);

	//切换类型
	this->GetHLL()->GetMouseManager()->ChangeMouseLineType(
		this->GetHLL(), this->GetHLL()->GetMouseLine().Row,
		ELineType::OnGround, this->GetHLL()->MMeshe, this->GetHLL()->MBody
	);

	//设置老鼠进入网格
	this->Get()->InMapMeshe(
		this->GetMeshe(this->GetHLL()->GetMouseLine().Row,
			this->GetHLL()->GetMouseLine().Col)->GetLineType()
	);
	this->Get()->CloseInWaterTimeLine();
}

void UHLL_FlyFalltate::AnimPlayEnd()
{
	this->Get()->ChangeState(NewObject<UHLL_IdleState>());
}

void UHLL_IdleState::Init()
{
	if (this->GetHLL()->CurStateNum > 1)
	{
		this->GetHLL()->CurStateNum = 0;
	}

	if (this->GetLevel() < 3)
	{
		this->IdleCount = 4;
		this->Get()->DefSpawnMouse(1);
	}
	else {
		this->IdleCount = 2;
		this->Get()->DefSpawnMouse(2);
	}



	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_Idle),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_IdleLow)
	);
}

void UHLL_IdleState::AnimPlayEnd()
{
	this->IdleCount--;
	if (this->IdleCount <= 0)
	{

		if (this->GetLevel() < 3)
		{
			//固定顺序
			if (this->GetHLL()->CurStateNum == 0)
			{
				this->Get()->ChangeState(NewObject<UHLL_FlyState>());
				this->GetHLL()->CurStateNum++;
			}
			else if (this->GetHLL()->CurStateNum == 1)
			{
				if (UGameSystemFunction::GetRange(25, 100))
				{
					this->Get()->ChangeState(NewObject<UHLL_LaserState>());
				}
				else {
					this->Get()->ChangeState(NewObject<UHLL_MissileState>());
				}
				this->GetHLL()->CurStateNum++;
			}
		}
		else {
			if (UGameSystemFunction::GetRange(50, 100))
			{
				//降落打击
				this->Get()->ChangeState(NewObject<UHLL_FlyState>());
			}
			else {
				if (UGameSystemFunction::GetRange(75, 100))
				{
					if (UGameSystemFunction::GetRange(60, 100))
					{
						this->Get()->ChangeState(NewObject<UHLL_MissileState>());
					}
					else {
						this->Get()->ChangeState(NewObject<UHLL_MissileSecondState>());
					}
				}
				else
				{
					this->Get()->ChangeState(NewObject<UHLL_LaserState>());
				}
			}
		}
	}
}

void UHLL_IdleState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3) { _HurtValue = _HurtValue * 0.95f; }
}

void UHLL_MissileState::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_MissileATK),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_MissileATKLow)
	);

	this->Get()->DefSpawnMouse(2);

	this->GetHLL()->OnSpawnMissile(this->GetHLL()->GetActorLocation());
}

void UHLL_MissileState::AnimPlayEnd()
{
	this->Get()->ChangeState(NewObject<UHLL_IdleState>());
}

void UHLL_MissileState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3) { _HurtValue = _HurtValue * 0.95f; }
}

void UHLL_LaserState::Init()
{
	this->IdleCount = 5;

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_LaserATK),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_LaserATKLow)
	);
}

void UHLL_LaserState::AnimPlayEnd()
{
	this->IdleCount--;

	if (this->IdleCount <= 0)
	{

		this->GetHLL()->OnSpawnLaser(this->GetHLL()->GetActorLocation());

		UHLL_IdleState* Cur = NewObject<UHLL_IdleState>();
		this->Get()->ChangeState(Cur);
		Cur->IdleCount = 10;
	}
}

void UHLL_LaserState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	_HurtValue = _HurtValue * 1.1f;
}

void UHLL_MissileSecondState::Init()
{
	UGameSystemFunction::AddGameLog(this->GetHLL(),
		FALD(TEXT("请使用【")) + FAL(TEXT("冰桶炸弹"), TEXT("c")) + FALD(TEXT("】来")) +
		FAL(TEXT("解除"), TEXT("g")) + FALD(TEXT("轰隆隆的全面轰炸！"))
	);

	this->Count = 8;

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_MissileATK),
		UGameSystemFunction::LoadRes(this->GetHLL()->AnimA_MissileATKLow)
	);

	this->Count--;
	this->GetHLL()->OnSpawnMissile(this->GetHLL()->GetActorLocation());
}

void UHLL_MissileSecondState::AnimPlayEnd()
{
	this->Count--;
	if (this->Count <= 0)
	{
		UHLL_IdleState* Cur = NewObject<UHLL_IdleState>();
		this->Get()->ChangeState(Cur);
	}
	else {
		this->Get()->DefSpawnMouse(1);
		this->GetHLL()->OnSpawnMissile(this->GetHLL()->GetActorLocation());
	}
}

void UHLL_MissileSecondState::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	Super::ExecuteBuff(BuffTag, CurBuffTime);

	if (BuffTag == EGameBuffTag::Freeze)
	{
		this->Count = 0;
	}
}

void UHLL_MissileSecondState::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	if (this->GetLevel() >= 3) { _HurtValue = _HurtValue * 0.95f; }
}
