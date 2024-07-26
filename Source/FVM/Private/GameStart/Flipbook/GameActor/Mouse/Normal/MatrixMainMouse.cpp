// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MatrixMainMouse.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/MatrixChildMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/MapMeshe.h"

void UMatrixGroup::Setup(const EMatrixGroupTag& Tag, AMatrixGroupMouse* NewMouse)
{
	AMatrixGroupMouse** Cur = this->GroupMouse.Find(Tag);
	if (Cur && IsValid(*Cur) && (*Cur)->GetCurrentHP() > 0.f)
	{
		return;
	}
	else {
		this->GroupMouse.Emplace(Tag, NewMouse);
	}
}

bool UMatrixGroup::GetNeedCreate(TArray<EMatrixGroupTag>& OutTag)
{
	//查询首领
	AMatrixGroupMouse** Cur = this->GroupMouse.Find(EMatrixGroupTag::Main);
	if (Cur && IsValid(*Cur) && (*Cur)->GetCurrentHP() > 0.f)
	{
		//添加标签
		OutTag.Append({ EMatrixGroupTag::Top,EMatrixGroupTag::Bottom,EMatrixGroupTag::Left,EMatrixGroupTag::Right });

		int32 DCount = 0;
		if ((*Cur)->GetMouseLine().Row == 0)
		{
			OutTag.Remove(EMatrixGroupTag::Top);
			DCount = 1;
		}

		if ((*Cur)->GetMouseLine().Row == (*Cur)->GetMouseManager()->GetLineCount() - 1)
		{
			OutTag.Remove(EMatrixGroupTag::Bottom);
			DCount = 1;
		}

		int32 ValidMouseCount = 0;
		for (auto CurMouse : this->GroupMouse)
		{
			if (IsValid(CurMouse.Value) && CurMouse.Value->GetCurrentHP() > 0.f)
			{
				//已经存在的老鼠，增加数量，标签移除对应标签
				ValidMouseCount += 1;
				OutTag.Remove(CurMouse.Key);
			}
		}
		return ValidMouseCount == (5 - DCount) ? false : true;
	}
	return false;
}

bool UMatrixGroup::GetMove()
{
	for (const auto& CurMouse : this->GroupMouse)
	{
		if (IsValid(CurMouse.Value) && CurMouse.Value->GetCurrentHP() > 0.f)
		{
			if (!CurMouse.Value->M_Buff->GetConstBuff())
			{
				//与Buff无关，其中一只老鼠因为Buff被限制，其他老鼠则应该忽略
				if (!CurMouse.Value->GetbIsMove())
				{
					//UE_LOG(LogTemp, Error, TEXT("%s"), *FString(UKismetSystemLibrary::GetObjectName(CurMouse.Value) + TEXT("：无法移动")));
					this->bMove = false;
					return false;
				}
			}
		}
	}

	//如果首领没有死亡，才会判断是否创建完毕
	if (!this->bMainMouseDeathed)
	{
		if (!this->GetbCreate())
		{
			return false;
		}
	}

	this->bMove = true;
	return true;
}

void UMatrixGroup::UpdateState(AMatrixGroupMouse* CallObj)
{
	for (const auto& CurMouse : this->GroupMouse)
	{
		if (IsValid(CurMouse.Value) && CurMouse.Value->GetCurrentHP() > 0.f)
		{
			if (CurMouse.Value != CallObj)
			{
				if (!CurMouse.Value->M_Buff->GetConstBuff())
				{
					CurMouse.Value->UpdateAnim();
				}
			}
		}
	}
}

void UMatrixGroup::MoveStop()
{
	this->bMove = false;
}

const bool& UMatrixGroup::GetbMove()
{
	return this->bMove;
}

void UMatrixGroup::SetAtk(bool bCurAtk)
{
	this->bAtk = bCurAtk;
}

bool UMatrixGroup::GetAtk()
{
	for (const auto& CurMouse : this->GroupMouse)
	{
		if (IsValid(CurMouse.Value) && CurMouse.Value->GetCurrentHP() > 0.f)
		{
			//有一只老鼠在攻击都是true
			if (CurMouse.Value->GetbIsAttack())
			{
				return true;
			}
		}
	}
	return false;
}

const bool& UMatrixGroup::GetbAtk()
{
	return this->bAtk;
}

bool UMatrixGroup::GetbCreate()
{
	for (const auto& CurMouse : this->GroupMouse)
	{
		if (IsValid(CurMouse.Value) && CurMouse.Value->GetCurrentHP() > 0.f)
		{
			//如果有一只老鼠没有创建成功，返回false
			if (!CurMouse.Value->GetbCreate())
			{
				return false;
			}
		}
	}

	return true;
}

void UMatrixGroup::DeathMouse(class AMatrixGroupMouse* NewMouse, bool MainMouseDeathed)
{
	//首领是否死亡
	if (!this->bMainMouseDeathed)
	{
		this->bMainMouseDeathed = MainMouseDeathed;
	}

	bool bRe = false;
	for (const auto& CurMouse : this->GroupMouse)
	{
		if (IsValid(CurMouse.Value))
		{
			if (CurMouse.Value == NewMouse)
			{
				this->GroupMouse.Remove(CurMouse.Key);
				bRe = true;
				break;
			}
		}
	}

	if (bRe)
	{
		this->GetMove();
		this->SetAtk(this->GetbAtk());
		this->UpdateState(nullptr);
	}
}

void AMatrixGroupMouse::SetupMatrixGroup(const EMatrixGroupTag& Tag, UMatrixGroup* matrixGroup)
{
	this->MatrixGroup = matrixGroup;
	this->MatrixGroup->Setup(Tag, this);
}

UMatrixGroup* AMatrixGroupMouse::GetMatrixGroup()
{
	return this->MatrixGroup;
}

void AMatrixGroupMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	if (this->GetMatrixGroup())
	{
		if (this->GetMatrixGroup()->GetbMove())
		{
			this->MoveFunc(DeltaTime);
		}

		return;
	}
	else {
		this->MoveFunc(DeltaTime);
	}

	//如果遇到控制buff，则不能行动
	//this->GetBuffResult();
}
void AMatrixGroupMouse::MoveFunc(const float& DeltaTime) {}

void AMatrixGroupMouse::UpdateAnim() {}

void AMatrixGroupMouse::MoveingStop()
{
	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->MoveStop();
	}
}

void AMatrixGroupMouse::MoveingBegin()
{
	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->GetMove();
	}
}

bool AMatrixGroupMouse::GetbCreate()
{
	return this->bCreateMouse;
}

AMatrixMainMouse::AMatrixMainMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AMatrixMainMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bAttack)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime);
	}
}

void AMatrixMainMouse::BeginPlay()
{
	Super::BeginPlay();
	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMatrixMainMouse::AnimPlayEnd);
}

void AMatrixMainMouse::MouseInit()
{
	Super::MouseInit();

	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//初始化
	this->bRun = true;
	this->RunDelay = 0.2f;
	this->RunMaxTime = 2.2f;
	this->bCreateDelay = 10.f;

	//根据等级决定
	if (this->GetTotalHP() > 40.f)
	{
		this->RunMaxTime = 5.f;
		this->bCreateDelay = 4.f;
	}

	this->bCreate = false;
	this->bSpeedBack = false;
	this->bAttack = false;
	this->AttackDelay = 0.1f;
	this->bCreateMouse = true;
	this->UpdateAnim(this->bRun, this->Anim.Run, this->Anim.RunLow);
}

void AMatrixMainMouse::MoveFunc(const float& DeltaTime) {
	//奔跑
	this->UpdateRunCheckCard(DeltaTime);
	//普通移动
	this->UpdateWalk(DeltaTime);
}

void AMatrixMainMouse::AttackedBegin()
{
	Super::AttackedBegin();

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->GetMove();
		this->GetMatrixGroup()->SetAtk(true);
		this->GetMatrixGroup()->UpdateState(nullptr);
	}
	else {
		this->UpdateAttackAnim();
	}
}

void AMatrixMainMouse::AttackedEnd()
{
	Super::AttackedBegin();

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->GetMove();
		this->GetMatrixGroup()->SetAtk(this->GetMatrixGroup()->GetAtk());
		this->GetMatrixGroup()->UpdateState(nullptr);

		if (!this->GetMatrixGroup()->GetbMove())
		{
			this->UpdateAnim(true, this->Anim.Attack, this->Anim.AttackLow);
			return;
		}
		else {
			this->UpdateAnim(true, this->Anim.Walk, this->Anim.WalkLow);
		}
	}
}

void AMatrixMainMouse::MoveingBegin()
{
	Super::MoveingBegin();

	if (this->GetMatrixGroup())
	{
		if (this->GetMatrixGroup()->GetbMove())
		{
			this->UpdateAnim(true, this->Anim.Walk, this->Anim.WalkLow);

			this->GetMatrixGroup()->UpdateState(this);
		}
	}
}

bool AMatrixMainMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);


	//【条件满足】更新跑步动画
	this->UpdateAnim(this->bRun, this->Anim.Run, this->Anim.RunLow);

	if (this->bRun)
	{
		return true;
	}

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->GetMove();
		this->GetMatrixGroup()->SetAtk(this->GetMatrixGroup()->GetAtk());
		this->GetMatrixGroup()->UpdateState(nullptr);

		if (!this->GetMatrixGroup()->GetbMove())
		{
			this->UpdateAnim(true, this->Anim.Attack, this->Anim.AttackLow);
			return true;
		}
	}
	else {
		if (!this->UpdateAttackAnim())
		{
			this->UpdateAnim(true, this->Anim.Walk, this->Anim.WalkLow);
		}
	}

	return true;
}

void AMatrixMainMouse::MouseDeathed()
{
	//关闭碰撞
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//解除绑定
	this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->bAttack = false;

	Super::MouseDeathed();

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->DeathMouse(this);
	}

	if (this->GetMouseDeathByBomb())
	{
		this->PlayBombEffAnim();
	}
	else {
		this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.IdleDeath)
		);
	}
}

void AMatrixMainMouse::AnimPlayEnd()
{
	//【条件满足】更新跑步动画
	this->UpdateAnim(this->bRun, this->Anim.Run, this->Anim.RunLow);

	if (this->bRun)
	{
		return;
	}

	//成员创建完毕
	if (this->CreateChildEnd())
	{
		//其他操作
		if (!this->UpdateAttackAnim())//更新攻击动画
		{
			//如果有小弟没有创建完成，播放攻击动画
			if (this->GetMatrixGroup())
			{
				//动画播放完成
				if (this->GetMatrixGroup()->GetbCreate())
				{
					this->GetMatrixGroup()->GetMove();
					this->GetMatrixGroup()->SetAtk(this->GetMatrixGroup()->GetAtk());
					this->GetMatrixGroup()->UpdateState(nullptr);

					//如果有任何老鼠处于攻击状态
					if (this->GetMatrixGroup()->GetbAtk())
					{
						if (!this->GetMatrixGroup()->GetbMove())
						{
							this->UpdateAnim(true, this->Anim.Attack, this->Anim.AttackLow);
							return;
						}
					}

				}
				else {
					this->GetMatrixGroup()->MoveStop();
					this->GetMatrixGroup()->UpdateState(nullptr);
					this->UpdateAnim(true, this->Anim.Attack, this->Anim.AttackLow);
					return;
				}
			}

			this->UpdateAnim(true, this->Anim.Walk, this->Anim.WalkLow);
			//首领允许移动
			this->MoveStart();
		}
		else {
			if (this->GetbIsAttack())
			{
				//攻击防御卡
				if (!this->AttackCard())
				{
					//取消攻击
					this->SetbIsAttack(false);
					return;
				}
			}
		}
	}
}

void AMatrixMainMouse::CheckMoveFunc(const float& DeltaTime)
{
	if (this->bCreateDelay > 0.f)
	{
		this->bCreateDelay -= DeltaTime;
	}
	else {
		//根据等级决定
		if (this->GetTotalHP() > 40.f)
		{
			this->bCreateDelay = 4.f;
		}
		else {
			this->bCreateDelay = 10.f;
		}

		if (this->GetMatrixGroup())
		{
			TArray<EMatrixGroupTag> CreateTag;
			if (this->GetMatrixGroup()->GetNeedCreate(CreateTag))
			{
				this->bCreate = true;
				this->CreateNewChild();

				return;
			}
		}
	}

	//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

	this->UpdateMove(DeltaTime);
}

void AMatrixMainMouse::UpdateRunCheckCard(const float& DeltaTime)
{
	if (!this->bRun)
	{
		return;
	}

	if (this->RunDelay > 0.f)
	{
		this->RunDelay -= DeltaTime;
	}
	else {
		this->RunDelay = 0.2f;

		FHitResult Ht;
		if (UGameSystemFunction::AddSphereTrace(
			this, Ht,
			FVector(0.f), FVector(0.f), 15.f,
			UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor2)
		))
		{
			ACardActor* CurCard = Cast<ACardActor>(Ht.GetActor());
			if (IsValid(CurCard))
			{
				this->bRun = false;
				this->bCreate = true;
			}
		}
	}

	if (this->RunMaxTime > 0.f)
	{
		this->RunMaxTime -= DeltaTime;
	}
	else {
		this->bRun = false;
		this->bCreate = true;
	}

	if (this->bCreate)
	{
		this->CreateNewChild();
		return;
	}
	else
	{
		this->CheckMoveFunc(DeltaTime);
	}
}

void AMatrixMainMouse::UpdateAnim(bool bState, const TSoftObjectPtr<UPaperFlipbook>& CurAnim, const TSoftObjectPtr<UPaperFlipbook>& CurAnimLow)
{
	float Cur = this->GetCurrentHP();
	if (Cur <= 0.f)
	{
		return;
	}

	//如果在创建的过程中遇到除死亡的任何情况，不更新动画
	if (this->bCreate)
	{
		return;
	}

	if (bState)
	{
		//播放奔跑动画
		if (Cur > this->GetTotalHP() * 0.5)
		{
			this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(CurAnim), true
			);
		}
		else {
			this->SetPlayAnimation(
				UGameSystemFunction::LoadRes(CurAnimLow), true
			);
		}
	}
}

void AMatrixMainMouse::UpdateAnim()
{
	if (this->GetMatrixGroup())
	{
		if (!this->GetMatrixGroup()->GetbMove())
		{
			this->UpdateAnim(true, this->Anim.Attack, this->Anim.AttackLow);
			return;
		}
	}

	if (this->GetbIsAttack())
	{
		this->UpdateAnim(true, this->Anim.Attack, this->Anim.AttackLow);
	}
	else {
		this->UpdateAnim(true, this->Anim.Walk, this->Anim.WalkLow);
	}
}

AMouseActor* AMatrixMainMouse::CreateNewChildMouse(const FVector& Loca, const FLine& NewLine)
{
	float Offset = 0.f;
	if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
	{
		Offset = fMouseInWaterZ;
	}

	//创建小弟
	AMouseActor* NewMouse = UMouseSpawnManager::MakeNewMouseByClass(
		this->GetMouseManager(),
		this->Anim.ChildMouse,
		this->GetActorLocation() + Loca + FVector(0.f, 0.f, Offset), 20.f,
		5.f, 16.f,
		NewLine,
		ELineType::OnGround
	);

	return NewMouse;
}

void AMatrixMainMouse::CreateNewChild()
{
	if (!this->bCreate)
	{
		return;
	}

	//降低移动速度
	if (!this->bSpeedBack)
	{
		this->bSpeedBack = true;
		this->SetMoveSpeed(16.f);
	}

	if (this->GetMatrixGroup())
	{
		this->GetMatrixGroup()->MoveStop();
		this->GetMatrixGroup()->UpdateState(this);
	}

	//关闭攻击
	this->bAttack = false;

	//更新动画
	float Cur = this->GetCurrentHP();
	if (Cur > this->GetTotalHP() * 0.5)
	{
		this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.Create)
		);
	}
	else {
		this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->Anim.CreateLow)
		);
	}
}

bool AMatrixMainMouse::CreateChildEnd()
{
	if (this->bCreate)
	{
		this->bCreate = false;
		this->bAttack = true;
		this->CreateChildMouse();
	}

	return !this->bCreate;
}

void AMatrixMainMouse::CreateChildMouse()
{
	if (!this->GetMatrixGroup())
	{
		this->SetupMatrixGroup(EMatrixGroupTag::Main, NewObject<UMatrixGroup>());
	}

	TArray<EMatrixGroupTag> CreateTag;
	if (this->GetMatrixGroup()->GetNeedCreate(CreateTag))
	{
		auto TCreateChild = [&](const FVector& Loca, const FLine& NewLine)->AMatrixGroupMouse* {
			return Cast<AMatrixGroupMouse>(
				this->CreateNewChildMouse(Loca, NewLine)
				);
		};

		FLine CurLine = this->GetMouseLine();
		for (const EMatrixGroupTag& CurTag : CreateTag)
		{
			switch (CurTag)
			{
			case EMatrixGroupTag::Top:
				TCreateChild(FVector(0.f, 0.f, 65.f),
					FLine(CurLine.Row - 1, 0))->SetupMatrixGroup(CurTag, this->GetMatrixGroup()); break;
			case EMatrixGroupTag::Bottom:
				TCreateChild(FVector(0.f, 0.f, -65.f),
					FLine(CurLine.Row + 1, 0))->SetupMatrixGroup(CurTag, this->GetMatrixGroup()); break;
			case EMatrixGroupTag::Left:
				TCreateChild(FVector(0.f, -80.f, 0.f), CurLine)->SetupMatrixGroup(CurTag, this->GetMatrixGroup()); break;
			case EMatrixGroupTag::Right:
				TCreateChild(FVector(0.f, 80.f, 0.f), CurLine)->SetupMatrixGroup(CurTag, this->GetMatrixGroup()); break;
			default:
				break;
			}
		}
	}
}

void AMatrixMainMouse::UpdateWalk(const float& DeltaTime)
{
	if (this->bRun || this->bCreate || this->GetbIsAttack())
	{
		return;
	}

	this->CheckMoveFunc(DeltaTime);
}

bool AMatrixMainMouse::UpdateAttackAnim()
{
	if (this->GetbIsAttack())
	{
		//更新攻击动画
		this->UpdateAnim(true, this->Anim.Attack, this->Anim.AttackLow);

		return true;
	}
	return false;
}


