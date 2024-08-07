// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_FL2.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicMasterMouse.h"

void UFLB_Create::Init()
{
	/*this->GetA()->SetPlayAnimationOnce(
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_Out),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Create)
	);*/

	this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);

	//切换路线
	this->GetA()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->GetA()),
		this->GetA()->GetMouseLine().Row, 3, this->GetRowAndCol().Col - 1
	);

	//设置位置
	this->GetA()->SetActorLocation(this->GetMesheLocation(3, this->GetRowAndCol().Col - 1));

	//进入网格
	this->GetA()->InMapMeshe(this->GetMeshe(3, this->GetRowAndCol().Col - 1)->GetLineType());
	this->GetA()->CloseInWaterTimeLine();

	this->bInit = false;
}

void UFLB_Create::AnimPlayEnd()
{
	if (!this->bInit)
	{
		this->bInit = true;
		return;
	}

	this->GetA()->ChangeState(NewObject<UFLB_Idle>());
}

void UFLB_Idle::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Idle),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_IdleLow)
	);
}

void UFLB_Idle::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Idle),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_IdleLow)
	);

	_HurtValue = _HurtValue - _HurtValue * 0.15f;
}

void UFLB_Idle::AnimPlayEnd()
{
	if (this->Count > 0)
	{
		this->Count--;
	}
	else {
		//召唤老鼠
		this->GetA()->DefSpawnMouse(this->GetA()->GetMouseLevel());

		//移动目的地
		this->GetA()->ChangeState(NewObject<UFLB_Move>());
	}
}

void UFLB_Move::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Idle),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_IdleLow)
	);

	int32 RanRow = UGameSystemFunction::GetRandomRange(0, this->GetRowAndCol().Row - 1);
	if (this->GetA()->GetMouseLine().Row == RanRow)
	{
		if (RanRow == 0)
		{
			RanRow++;
		}
		else if (RanRow == this->GetRowAndCol().Row - 1)
		{
			RanRow--;
		}
	}

	//切换路线
	this->GetA()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->GetA()), this->GetA()->GetMouseLine().Row,
		RanRow, this->GetRowAndCol().Col - 1);

	//初始化移动位置
	this->CurLocation = this->GetA()->GetActorLocation();
	this->TargetLocation = this->GetMesheLocation(RanRow, this->GetRowAndCol().Col - 1);

	this->MoveTime = UTimeLineClass::MakeTimeLineClass();
	this->MoveTime->AddCurve(
		this->GetA()->MouseMoveCurve, this,
		[](UTimeLineClass* time, UObject* obj, float value)
		{
			(Cast<UFLB_Move>(obj)->GetA())->SetActorLocation(
				UKismetMathLibrary::VLerp(
					Cast<UFLB_Move>(obj)->CurLocation,
					Cast<UFLB_Move>(obj)->TargetLocation,
					value)
			);
		},
		[](UTimeLineClass* time, UObject* obj)
		{
			ELineType CurTemp = (Cast<UFLB_Move>(obj)->GetA())->M_Proper_Condition.M_CurrentInType;
			(Cast<UFLB_Move>(obj)->GetA())->SetInWaterAnimEnable(true);
			(Cast<UFLB_Move>(obj)->GetA())->M_Proper_Condition.M_CurrentInType = ELineType::OnGround;
			(Cast<UFLB_Move>(obj)->GetA())->InMapMeshe(CurTemp);

			//切换技能
			(Cast<UFLB_Move>(obj)->GetA())->ChangeState(NewObject<UFLB_SelectSKill>());
		}
		);
	this->GetA()->SetInWaterAnimEnable(false);
	this->MoveTime->PlayFromStart();
}

void UFLB_Move::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->MoveTime))
	{
		if (this->GetA()->GetCurrentHP() > 0.f)
		{
			this->MoveTime->Tick(DeltaTime);
		}
	}
}

void UFLB_Move::InMapMeshe(ELineType CurLineType)
{
	if (this->GetA()->M_Proper_Condition.M_CurrentInType == CurLineType)
	{
		return;
	}

	this->GetA()->ForceSetWaterAnimShow(CurLineType);
	this->GetA()->M_Proper_Condition.M_CurrentInType = CurLineType;
}

void UFLB_SelectSKill::Init()
{
	Super::Init();

	if (UGameSystemFunction::GetRange(50, 100))
	{
		if (this->Get()->GetCurrentHP() < this->Get()->GetTotalHP() * 0.5f)
		{
			if (UGameSystemFunction::GetRange(35, 100))
			{
				this->GetA()->ChangeState(NewObject<UFLB_ReverseHP>());
			}
			else {
				if (UGameSystemFunction::GetRange(50, 100))
				{
					//判断当前网格是否有角色
					if (AMapMeshe* CurMapMeshe =
						this->GetA()->GetMesheComp()->GetMapMesh(
							this->GetA()->GetMouseLine().Row,
							this->GetA()->GetMesheComp()->GetMapMeshRowAndCol().Col - 1
						)
						)
					{
						if (IsValid(CurMapMeshe->GetUIMapMeshe()))
						{
							if (CurMapMeshe->GetUIMapMeshe()->GetPlayer())
							{
								if (UGameSystemFunction::GetRange(50, 100))
								{
									this->GetA()->ChangeState(NewObject<UFLB_KillCard>());
								}
								else {
									this->GetA()->ChangeState(NewObject<UFLB_CallMouse>());
								}

								return;
							}
						}
					}
					this->GetA()->ChangeState(NewObject<UFLB_CallBug>());
				}
				else {
					this->GetA()->ChangeState(NewObject<UFLB_KillCard>());
				}
			}
		}
		else {
			int32 Ran = UGameSystemFunction::GetRandomRange(0, 2);
			if (Ran == 0)
			{
				//判断当前网格是否有角色
				if (AMapMeshe* CurMapMeshe =
					this->GetA()->GetMesheComp()->GetMapMesh(
						this->GetA()->GetMouseLine().Row,
						this->GetA()->GetMesheComp()->GetMapMeshRowAndCol().Col - 1
					)
					)
				{
					if (IsValid(CurMapMeshe->GetUIMapMeshe()))
					{
						if (CurMapMeshe->GetUIMapMeshe()->GetPlayer())
						{
							if (UGameSystemFunction::GetRange(50, 100))
							{
								this->GetA()->ChangeState(NewObject<UFLB_KillCard>());
							}
							else {
								this->GetA()->ChangeState(NewObject<UFLB_CallMouse>());
							}

							return;
						}
					}
				}
				this->GetA()->ChangeState(NewObject<UFLB_CallBug>());
			}
			else if (Ran == 1)
			{
				this->GetA()->ChangeState(NewObject<UFLB_KillCard>());
			}
			else {
				this->GetA()->ChangeState(NewObject<UFLB_CallMouse>());
			}
		}
	}
	else {
		this->GetA()->ChangeState(NewObject<UFLB_CallMouse>());
	}

}

void UFLB_ReverseHP::Init()
{
	Super::Init();

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use1),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use1Low)
	);
}

void UFLB_ReverseHP::AnimPlayEnd()
{
	if (this->bInit)
	{
		if (this->UseCount > 0)
		{
			this->UseCount--;

			if (this->GetA()->BeHit(nullptr, this->GetA()->GetTotalHP() * 0.04f * -1.f, EFlyItemAttackType::Def))
			{
				this->GetA()->SetbIsHurt(true);
			}

			//生成加血动画
			AMagicMasterHpAddtionBuff* CurAnimObj = this->GetA()->GetWorld()->SpawnActor<AMagicMasterHpAddtionBuff>();
			CurAnimObj->Init(this->GetA(), this->GetA()->AnimA_HP);

			if (this->UseCount <= 0)
			{
				this->GetA()->ChangeState(NewObject<UFLB_Idle>());
			}
		}
		else {
			this->GetA()->ChangeState(NewObject<UFLB_Idle>());
		}
	}
	else {
		this->bInit = true;
	}
}

void UFLB_CallBug::AnimPlayEnd()
{
	AFL_CallBugActor* New = this->GetA()->GetWorld()->SpawnActor<AFL_CallBugActor>();
	New->Init(this);

	this->GetA()->ChangeState(NewObject<UFLB_Idle>());
}

void UFLB_KillCard::Init()
{
	Super::Init();

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use2),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use2Low)
	);
}

void UFLB_KillCard::AnimPlayEnd()
{
	if (!this->bInit)
	{
		this->bInit = true;

		//消灭卡片
		this->KillCard();

		return;
	}

	if (!this->bFinish)
	{
		this->bFinish = true;

		//切换形态
		this->GetA()->ChangeState(NewObject<UFLB_Idle>());
		return;
	}
}

void UFLB_CallMouse::Init()
{
	Super::Init();

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use1),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use1Low)
	);
}

void UFLB_CallMouse::AnimPlayEnd()
{
	if (!this->bInit)
	{
		this->bInit = true;

		//召唤老鼠
		this->SpawnMouse();

		return;
	}

	if (!this->bFinish)
	{
		this->bFinish = true;

		//切换形态
		this->GetA()->ChangeState(NewObject<UFLB_Idle>());
		return;
	}
}
