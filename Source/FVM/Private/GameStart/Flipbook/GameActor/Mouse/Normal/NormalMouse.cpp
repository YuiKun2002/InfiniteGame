// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalMouse.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "SpineSkeletonAnimationComponent.h"
#include "GameStart/VS/MapMeshe.h"

#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>

ANormalMouse::ANormalMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->BodyComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetPointComponent());
	this->BodyComp->SetupAttachment(this->GetPointComponent());
}

void ANormalMouse::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞网格位置
	this->MesheComp->SetBoxExtent(FVector(20.f, 20.f, 20.f), true);
	this->MesheComp->AddLocalOffset(FVector(0.f, 0.f, 17.f));
	this->BodyComp->AddLocalOffset(FVector(0.f, 0.f, 17.f));

	if (UFVMGameInstance::GetDebug())
	{
		this->MesheComp->SetHiddenInGame(false);
		this->BodyComp->SetHiddenInGame(false);
	}

	//绑定动画播放结束的委托
	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ANormalMouse::OnAnimationPlayEnd);
	//初始化状态管理器
	this->Manager = UNormalMouseStateManager::MakeNormalMouseStateManager(this);

	switch (this->M_NormalMouseTypeMode)
	{
	case ENormalMouseTypeMode::Def:this->Manager->ChangeState(NewObject<UMouseStateDef>());
		break;
	case ENormalMouseTypeMode::Add:this->Manager->ChangeState(NewObject<UMouseStateModAdd>());
		break;
	case ENormalMouseTypeMode::AddDefence:this->Manager->ChangeState(NewObject<UMouseStateAddDefence>());
		break;
	case ENormalMouseTypeMode::Bug:this->Manager->ChangeState(NewObject<UMouseStateBug>());
		break;
	default:
		this->Manager->ChangeState(NewObject<UMouseStateDef>());
		break;
	}
}

void ANormalMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			this->Manager->GetState()->MoveingUpdate(DeltaTime);
		}
	}
}

bool ANormalMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			return this->Manager->GetState()->BeHit(CurHitMouseObj, _HurtValue, AttackType);
		}
	}

	return true;
}


void ANormalMouse::AttackedBegin()
{

	Super::AttackedBegin();

	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			this->Manager->GetState()->AttackedBegin();
		}
	}
}

void ANormalMouse::AttackedEnd()
{
	Super::AttackedEnd();

	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			this->Manager->GetState()->AttackedEnd();
		}
	}
}

void ANormalMouse::MoveingBegin()
{
	Super::MoveingBegin();

	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			this->Manager->GetState()->MoveingBegin();
		}
	}
}

void ANormalMouse::MouseDeathed()
{
	//关闭碰撞
	this->ClosedBoxComponent(this->MesheComp);
	//this->ClosedBoxComponent(this->BodyComp);

	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{

		if (IsValid(this->Manager))
		{
			if (IsValid(this->Manager->GetState()))
			{
				this->Manager->GetState()->MouseDeathed();
			}
		}
	}

	this->Manager->ChangeState(nullptr);
}

void ANormalMouse::InMapMeshe(ELineType CurLineType)
{
	if (this->GetMouseLineType() != ELineType::OnGround)
	{
		this->M_Proper_Condition.M_CurrentInType = CurLineType;
	}
	else {
		Super::InMapMeshe(CurLineType);
	}
}

void ANormalMouse::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			this->Manager->GetState()->ExecuteBuff(BuffTag, CurBuffTime);
		}
	}

	if (CurBuffTime > 0.f)
	{
		Super::ExecuteBuff(BuffTag, CurBuffTime);
	}
}

void ANormalMouse::ExitBuff(EGameBuffTag BuffTag)
{
	Super::ExitBuff(BuffTag);
}

void ANormalMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANormalMouse::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			this->Manager->GetState()->MouseTick(DeltaTime);
		}
	}
}

void ANormalMouse::OnAnimationPlayEnd()
{
	if (IsValid(this->Manager))
	{
		if (IsValid(this->Manager->GetState()))
		{
			this->Manager->GetState()->OnAnimationPlayEnd();
		}
	}
}

UNormalMouseDefenceStateManager* UNormalMouseDefenceStateManager::MakeNormalMouseDefenceStateManager(class ANormalMouse* CurMouse)
{
	UNormalMouseDefenceStateManager* Cur = NewObject<UNormalMouseDefenceStateManager>();
	Cur->NormalMouse = CurMouse;
	Cur->UpdateState();
	return Cur;
}

void UNormalMouseDefenceStateManager::UpdateState(const float& HurtValue, const EFlyItemAttackType& AttackType)
{
	//基础的数据
	bool bAttack = this->NormalMouse->GetbIsAttack();
	float Total = this->NormalMouse->GetTotalHP();
	float CurHP = this->NormalMouse->GetCurrentHP();
	float DefenHP = this->NormalMouse->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP;
	float DefenCurHP = this->NormalMouse->M_NormalDefenceMouseResourceStruct.DefenceItemCurHP;
	FNormalDefenceMouseResourceStruct* Cur = &this->NormalMouse->M_NormalDefenceMouseResourceStruct;

	if (AttackType != EFlyItemAttackType::Def)
	{
		if (!Cur->bItemRemove)
		{
			bool DefenceLow = DefenCurHP <= DefenHP * 0.5 ? true : false;
			//遇到穿透或者投掷【不削弱防具】
			if (CurHP > Total * 0.5)
			{
				this->SetAnim(bAttack, true, DefenceLow ? Cur->TakeLowAtk : Cur->TakeAtk, nullptr, DefenceLow ? Cur->TakeLow : Cur->Take, nullptr);
			}
			else {
				this->SetAnim(bAttack, true, DefenceLow ? Cur->TakeCharAllLowAtk : Cur->TakeCharLowAtk, nullptr, DefenceLow ? Cur->TakeAllLow : Cur->TakeCharLow, nullptr);
			}
		}
		else {
			if (CurHP > Total * 0.5)
			{
				this->SetAnim(bAttack, false, nullptr, Cur->CharAtk, nullptr, Cur->CharDef);
			}
			else {
				this->SetAnim(bAttack, false, nullptr, Cur->CharLowAtk, nullptr, Cur->CharLow);
			}
		}

	}
	else {
		//防具存在优先削弱防具
		if (!Cur->bItemRemove)
		{
			//触发防具掉落
			if (DefenCurHP <= 0.f)
			{
				//防具掉落
				Cur->bItemRemove = true;

				//播放单次动画【在没有进行攻击的时候才会触发掉落动画】
				if (CurHP > Total * 0.5)
				{
					if (!bAttack)
					{
						/*this->NormalMouse->SetPlayAnimationOnce(
							UGameSystemFunction::LoadRes(Cur->TakeFallChar),
							UGameSystemFunction::LoadRes(Cur->CharDef));*/

						this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);
						this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);

						if (Cur->bItemRemoveAddtionSpeed)
						{
							this->NormalMouse->MoveStop();
							Cast<UMouseStateBase>(this->NormalMouse->Manager->GetState())->M_bEnableAttakLine = false;
						}

						return;
					}
				}
				else {
					if (!bAttack)
					{
						/*this->NormalMouse->SetPlayAnimationOnce(
							UGameSystemFunction::LoadRes(Cur->TakeFallCharLow),
							UGameSystemFunction::LoadRes(Cur->CharLow));*/

						this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);
						this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);

						if (Cur->bItemRemoveAddtionSpeed)
						{
							this->NormalMouse->MoveStop();

							Cast<UMouseStateBase>(this->NormalMouse->Manager->GetState())->M_bEnableAttakLine = false;
						}

						return;
					}
				}
			}

			//掉落【触发了攻击】
			if (Cur->bItemRemove && bAttack)
			{
				//无防具
				if (CurHP > Total * 0.5)
				{
					this->SetAnim(true, false, nullptr, Cur->CharAtk, nullptr, nullptr);
				}
				else {
					this->SetAnim(true, false, nullptr, Cur->CharLowAtk, nullptr, nullptr);
				}
			}
			else {

				bool DefenceLow = DefenCurHP <= DefenHP * 0.5 ? true : false;
				//遇到穿透或者投掷【不削弱防具】
				if (CurHP > Total * 0.5)
				{
					this->SetAnim(bAttack, true, DefenceLow ? Cur->TakeLowAtk : Cur->TakeAtk, nullptr, DefenceLow ? Cur->TakeLow : Cur->Take, nullptr);
				}
				else {
					this->SetAnim(bAttack, true, DefenceLow ? Cur->TakeCharAllLowAtk : Cur->TakeCharLowAtk, nullptr, DefenceLow ? Cur->TakeAllLow : Cur->TakeCharLow, nullptr);
				}

				////防具未掉落，更新带有防具的动画
				//if (DefenCurHP > DefenHP * 0.5)
				//{
				//	this->SetAnim(bAttack, !Cur->bItemRemove, Cur->TakeAtk, nullptr, Cur->Take, nullptr);
				//}
				//else if (DefenCurHP > 0.f && DefenCurHP <= DefenHP * 0.5) {
				//	this->SetAnim(bAttack, !Cur->bItemRemove, Cur->TakeLowAtk, nullptr, Cur->TakeLow, nullptr);
				//}
			}
		}
		else {
			//无防具
			if (CurHP > Total * 0.5)
			{
				this->SetAnim(bAttack, false, nullptr, Cur->CharAtk, nullptr, Cur->CharDef);
			}
			else {
				this->SetAnim(bAttack, false, nullptr, Cur->CharLowAtk, nullptr, Cur->CharLow);
			}
		}
	}
}

void UNormalMouseDefenceStateManager::UpdateState()
{
	this->UpdateState(0, EFlyItemAttackType::Def);
}

void UNormalMouseDefenceStateManager::SetAnim(const bool& bAtk, const bool& bTake, TSoftObjectPtr<UPaperFlipbook> Anim1, TSoftObjectPtr<UPaperFlipbook> Anim2, TSoftObjectPtr<UPaperFlipbook> Anim3, TSoftObjectPtr<UPaperFlipbook> Anim4)
{
	if (bAtk)
	{
		//拿
		if (bTake)
		{
			//攻击拿[遇到攻击状态，出现掉落强行播放攻击动画]
			//this->NormalMouse->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim1), true);

			this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);
		}
		else {
			//攻击无
			//this->NormalMouse->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim2), true);

			this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);
		}
	}
	else {
		//拿走
		if (bTake)
		{
			//攻击拿[没有攻击，遇到掉落]
			//this->NormalMouse->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim3));

			this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);
		}
		else {
			//攻击无
			//this->NormalMouse->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim4));

			this->NormalMouse->SetAnimation(0, TEXT("SpineTag"), true);
		}
	}
}

UNormalMouseStateManager* UNormalMouseStateManager::MakeNormalMouseStateManager(ANormalMouse* NewMouseActor)
{
	UNormalMouseStateManager* NewManager = NewObject<UNormalMouseStateManager>();
	NewManager->MouseActor = NewMouseActor;
	return NewManager;
}

void UNormalMouseStateManager::ChangeState(class UNormalMouseStateBase* NewState)
{
	this->MouseState = nullptr;
	if (IsValid(NewState))
	{
		this->MouseState = NewState;
		this->MouseState->CurManager = this;
		this->MouseState->Init();
	}
}

UNormalMouseStateBase* UNormalMouseStateManager::GetState()
{
	return this->MouseState;
}

void UNormalMouseStateBase::Init() {}
void UNormalMouseStateBase::MouseTick(const float& DeltaTime) {}
void UNormalMouseStateBase::MoveingUpdate(float DeltaTime) {}
bool UNormalMouseStateBase::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) { return true; }
void UNormalMouseStateBase::AttackedBegin() {}
void UNormalMouseStateBase::AttackedEnd() {}
void UNormalMouseStateBase::MoveingBegin() {}
void UNormalMouseStateBase::MouseDeathed() {}
void UNormalMouseStateBase::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) {}
void UNormalMouseStateBase::OnAnimationPlayEnd() {}

ANormalMouse* UNormalMouseStateBase::Get()
{
	return this->CurManager->MouseActor;
}

void UMouseStateBase::Init()
{
	this->M_bEnableAttakLine = true;

	if (this->Get()->M_DefAnim_Anim.WalkAnimRes.Get())
	{
		//播放Walk动画
		this->TrackEntry = this->Get()->SetAnimation(
			this->TrackEntry,
			0,
			this->Get()->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName()
		);

		this->State = 0;
	}
}

void UMouseStateBase::MouseTick(const float& DeltaTime)
{
	if (this->M_bEnableAttakLine)
	{
		this->Get()->AddAttackLineFunc(
			UGameSystemFunction::GetCardCollisionBoxType(this->Get()->M_ECardCollisionType), DeltaTime
		);
	}
}

void UMouseStateBase::MoveingUpdate(float DeltaTime)
{
	this->Get()->UpdateMove(DeltaTime);
}

bool UMouseStateBase::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	//如果血量已经为0，则直接退出
	if (this->Get()->GetCurrentHP() <= 0.f)
	{
		return false;
	}

	if (this->Get()->GetCurrentHP() - _HurtValue <= 0)
	{
		//判断是否被炸死
		if (AttackType == EFlyItemAttackType::Bomb)
		{
			this->Get()->M_MouseDeathByBomb = true;
		}
	}

	//更新血量
	return !this->Get()->UpdateHP(_HurtValue);
}

void UMouseStateBase::OnAnimationPlayEnd()
{
	//如果老鼠处于攻击状态
	if (this->Get()->GetbIsAttack())
	{
		//攻击防御卡
		if (!this->Get()->AttackCard())
		{
			//取消攻击
			this->Get()->SetbIsAttack(false);
		}
	}
}

void UMouseStateDef::Init()
{
	Super::Init();

	this->M_bEnableAttakLine = true;
	this->Get()->MoveStart();
	this->ModeDefState();
}

bool UMouseStateDef::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		this->ModeDefState();
		return true;
	}

	return false;
}

void UMouseStateDef::AttackedBegin()
{
	this->ModeDefState();
}

void UMouseStateDef::AttackedEnd()
{
	this->ModeDefState();
}

void UMouseStateDef::MoveingBegin()
{
	this->ModeDefState();
}

void UMouseStateDef::MouseDeathed()
{
	if (this->State != 4)
	{
		this->TrackEntry = this->Get()->SetAnimation(
			this->TrackEntry,
			0,
			this->Get()->M_DefAnim_Anim.DeadAnimRes.GetDefaultObject()->GetCategoryName()
		);
		this->TrackEntry->AnimationComplete.AddDynamic(this->Get(), &AMouseActor::AlienDeadAnimationCompelet);
		this->State = 4;
	}
}

void UMouseStateDef::OnAnimationPlayEnd()
{
	Super::OnAnimationPlayEnd();

	//更新动画
	this->ModeDefState();
}

void UMouseStateDef::OnAnimationComplet(class UTrackEntry* Track)
{
	this->OnAnimationPlayEnd();
}

void UMouseStateDef::ModeDefState()
{
	//更新状态[当生命值大于总生命值的40%]
	if (this->Get()->GetCurrentHP() > this->Get()->GetTotalHP() * 0.4f)
	{
		if (this->Get()->GetbIsAttack())
		{
			if (this->State != 1)
			{
				//播放攻击动画
				this->TrackEntry = this->Get()->SetAnimation(
					this->TrackEntry,
					0,
					this->Get()->M_DefAnim_Anim.AttackAnimRes.GetDefaultObject()->GetCategoryName()
				);

				this->TrackEntry->AnimationComplete.AddDynamic(this, &UMouseStateDef::OnAnimationComplet);

				this->State = 1;
			}
		}
		else {
			if (this->State != 0)
			{
				this->TrackEntry = this->Get()->SetAnimation(
					this->TrackEntry,
					0,
					this->Get()->M_DefAnim_Anim.WalkAnimRes.GetDefaultObject()->GetCategoryName()
				);

				this->State = 0;
			}
		}
	}//[当生命值小于等于总生命值的40% && 老鼠生命值大于0]
	else if (this->Get()->GetCurrentHP() <= this->Get()->GetTotalHP() * 0.4f && this->Get()->GetCurrentHP() > 0.f)
	{
		if (this->Get()->GetbIsAttack())
		{
			if (this->State != 3)
			{
				//播放攻击动画
				this->TrackEntry = this->Get()->SetAnimation(
					this->TrackEntry,
					0,
					this->Get()->M_DefAnim_Anim.AttackAnimDamageRes.GetDefaultObject()->GetCategoryName()
				);

				this->TrackEntry->AnimationComplete.AddDynamic(this, &UMouseStateDef::OnAnimationComplet);

				this->State = 3;
			}
		}
		else {
			if (this->State != 2)
			{
				this->TrackEntry = this->Get()->SetAnimation(
					this->TrackEntry,
					0,
					this->Get()->M_DefAnim_Anim.WalkAnimDamageRes.GetDefaultObject()->GetCategoryName()
				);

				this->State = 2;
			}
		}
	}
}

void UMouseStateModAdd::Init()
{
	this->ModeAddState();
}

bool UMouseStateModAdd::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		this->ModeAddState();
		return true;
	}
	return false;
}

void UMouseStateModAdd::AttackedBegin()
{
	this->ModeAddState();
}

void UMouseStateModAdd::AttackedEnd()
{
	this->ModeAddState();
}

void UMouseStateModAdd::MoveingBegin()
{
	this->ModeAddState();
}

void UMouseStateModAdd::MouseDeathed()
{
	/*this->Get()->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->Get()->M_NormalAddMouseResourceStruct.M_NomalDeathAnim), true);*/

	this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
}

void UMouseStateModAdd::OnAnimationPlayEnd()
{
	Super::OnAnimationPlayEnd();

	this->ModeAddState();
}

void UMouseStateModAdd::ModeAddState()
{
	bool bAtk = this->Get()->GetbIsAttack();
	float Cur = this->Get()->GetCurrentHP();
	float Tot = this->Get()->GetTotalHP();

	//有头盔行走
	if (Cur > Tot * 0.8f)
	{
		this->Get()->M_NormalAddMouseResourceStruct.bHeadRemove = false;

		if (bAtk)
		{
			/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
				this->Get()->M_NormalAddMouseResourceStruct.M_NomalHeadATKAnim), true);*/

			this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
		}
		else {
			/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
				this->Get()->M_NormalAddMouseResourceStruct.M_NomalHeadAnim));*/

			this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
		}

		return;
	}
	else if (Cur <= Tot * 0.8f && Cur > Tot * 0.6f)
	{
		this->Get()->M_NormalAddMouseResourceStruct.bHeadRemove = false;

		//有头盔[破损]行走
		if (bAtk)
		{
			/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
				this->Get()->M_NormalAddMouseResourceStruct.M_NomalHeadATKLowAnim), true);*/

			this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
		}
		else {
			/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
				this->Get()->M_NormalAddMouseResourceStruct.M_NomalHeadLowAnim));*/

			this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
		}
	}
	else if (Cur <= Tot * 0.6f && Cur > Tot * 0.2f)
	{
		if (bAtk)
		{
			if (this->Get()->M_NormalAddMouseResourceStruct.bHeadRemove)
			{
				/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
					this->Get()->M_NormalAddMouseResourceStruct.M_NomalATKAnim), true);*/

				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
			}
			else {
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->M_NormalAddMouseResourceStruct.M_NomalAnim));

				//播放攻击状态下的头盔掉落
				this->Get()->M_NormalAddMouseResourceStruct.bHeadRemove = true;
				/*this->Get()->SetPlayAnimationOnce(
					UGameSystemFunction::LoadRes(
						this->Get()->M_NormalAddMouseResourceStruct.M_NomalHeadATKRemoveAnim),
					UGameSystemFunction::LoadRes(
						this->Get()->M_NormalAddMouseResourceStruct.M_NomalATKAnim)
				);*/

				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);

				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
			}
		}
		else {
			if (this->Get()->M_NormalAddMouseResourceStruct.bHeadRemove)
			{
				/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
					this->Get()->M_NormalAddMouseResourceStruct.M_NomalAnim));*/

				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
			}
			else {
				//播放没有攻击时的头盔掉落
				this->Get()->M_NormalAddMouseResourceStruct.bHeadRemove = true;
				/*this->Get()->SetPlayAnimationOnce(
					UGameSystemFunction::LoadRes(this->Get()->M_NormalAddMouseResourceStruct.M_NomalHeadRemoveAnim),
					UGameSystemFunction::LoadRes(this->Get()->M_NormalAddMouseResourceStruct.M_NomalAnim)
				);*/

				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
			}
		}
	}
	else {
		if (Cur > 0.f)
		{
			if (bAtk)
			{
				/*this->Get()->SetPlayAnimation(
					UGameSystemFunction::LoadRes(
						this->Get()->M_NormalAddMouseResourceStruct.M_NomalATKLowAnim), true);*/

				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
			}
			else {
				/*this->Get()->SetPlayAnimation(
					UGameSystemFunction::LoadRes(
						this->Get()->M_NormalAddMouseResourceStruct.M_NomalLowAnim));*/

				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
				this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
			}
		}
	}
}

void UMouseStateAddDefence::Init()
{
	//初始化
	this->Get()->M_NormalDefenceMouseResourceStruct.bItemRemove = false;

	this->Get()->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP =
		this->Get()->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP + this->Get()->GetTotalHP() * 0.5f;

	this->Get()->M_NormalDefenceMouseResourceStruct.DefenceItemCurHP =
		this->Get()->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP;

	this->Get()->M_NormalDefenceMouseResourceStruct.Manager =
		UNormalMouseDefenceStateManager::MakeNormalMouseDefenceStateManager(this->Get());
}

void UMouseStateAddDefence::MoveingUpdate(float DeltaTime)
{
	if (this->Get()->M_NormalDefenceMouseResourceStruct.bItemRemoveAddtionSpeed)
	{
		this->Get()->UpdateMove(DeltaTime * this->Get()->M_NormalDefenceMouseResourceStruct.fAddtionSpeedRate);
	}
	else {
		this->Get()->UpdateMove(DeltaTime);
	}
}

bool UMouseStateAddDefence::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	//默认攻击
	if (AttackType == EFlyItemAttackType::Def)
	{
		//更新防具血量
		if (!this->Get()->M_NormalDefenceMouseResourceStruct.bItemRemove)
		{
			this->Get()->M_NormalDefenceMouseResourceStruct.DefenceItemCurHP -= _HurtValue;
		}
		else {
			if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
			{
				return false;
			}
		}
	}
	else {
		if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
		{
			return false;
		}
	}

	this->Get()->M_NormalDefenceMouseResourceStruct.Manager->UpdateState();
	return true;
}

void UMouseStateAddDefence::AttackedBegin()
{
	this->Get()->M_NormalDefenceMouseResourceStruct.Manager->UpdateState();
}

void UMouseStateAddDefence::AttackedEnd()
{
	this->Get()->M_NormalDefenceMouseResourceStruct.Manager->UpdateState();
}

void UMouseStateAddDefence::MoveingBegin()
{
	this->Get()->M_NormalDefenceMouseResourceStruct.Manager->UpdateState();
}

void UMouseStateAddDefence::MouseDeathed()
{
	/*this->Get()->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->Get()->M_NormalDefenceMouseResourceStruct.Death), true);*/

	this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
}

void UMouseStateAddDefence::OnAnimationPlayEnd()
{
	Super::OnAnimationPlayEnd();

	//如果没有攻击，但是停止了移动，表示掉落的防具【如果是冻结、凝固buff，动画根本不会播放也不会出现播放完成的情况】
	if (!this->Get()->GetbIsAttack() && !this->Get()->GetbIsMove())
	{
		this->Get()->MoveStart();
		this->M_bEnableAttakLine = true;
	}
	this->Get()->M_NormalDefenceMouseResourceStruct.Manager->UpdateState();
}

void UMouseStateBug::Init()
{
	/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
		this->Get()->M_NormalBugMouseResourceStruct.MDef), true);*/

	this->Get()->SetAnimation(0, TEXT("SpineTag"), true);

	this->ATKCount = this->Get()->M_NormalBugMouseResourceStruct.AttackMaxCount;
}

void UMouseStateBug::AttackedBegin()
{
	/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
		this->Get()->M_NormalBugMouseResourceStruct.MATK), true);*/

	this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
}

void UMouseStateBug::AttackedEnd()
{
	/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
		this->Get()->M_NormalBugMouseResourceStruct.MDef), true);*/

	this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
}

void UMouseStateBug::MouseDeathed()
{
	/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
		this->Get()->M_NormalBugMouseResourceStruct.MDeath), true);*/

	this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
}

void UMouseStateBug::OnAnimationPlayEnd()
{
	//获取是否在攻击
	bool bATK = this->Get()->GetbIsAttack();

	Super::OnAnimationPlayEnd();

	//如果在攻击则减少攻击次数
	if (bATK)
	{
		if (this->ATKCount > 0)
		{
			this->ATKCount--;
			if (this->ATKCount <= 0)
			{
				this->Get()->SetMouseDeath(true);
			}
		}
		else {
			this->Get()->SetMouseDeath(true);
		}
	}
	else {
		/*this->Get()->SetPlayAnimation(UGameSystemFunction::LoadRes(
			this->Get()->M_NormalBugMouseResourceStruct.MDef), true);*/

		this->Get()->SetAnimation(0, TEXT("SpineTag"), true);
	}
}

void UMouseStateBug::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	//免疫减速，冻结，凝固
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
}
