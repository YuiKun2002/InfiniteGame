// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/RepairMouse.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/MapMeshe.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>

ARepairMouse::ARepairMouse()
{
	this->MMesheComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ReMouseMesheComponent"));
	this->MBodyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ReMouseBodyComponent"));

	//设置依附
	this->MMesheComponent->SetupAttachment(this->GetRootComponent());
	this->MBodyComponent->SetupAttachment(this->MMesheComponent);
}

void ARepairMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MMesheComponent, this->MBodyComponent);

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ARepairMouse::OnAnimationPlayEnd);
}

void ARepairMouse::MouseInit()
{
	Super::MouseInit();

	this->M_bEnableAttakLine = true;


	//初始化
	this->M_NormalDefenceMouseResourceStruct.bItemUse = false;
	this->M_NormalDefenceMouseResourceStruct.bItemRemove = false;
	this->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP = this->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP + this->GetTotalHP() * 0.5f;
	this->M_NormalDefenceMouseResourceStruct.DefenceItemCurHP = this->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP;
	//this->Manager = URepairMouseStateManager::MakeRepairMouseStateManager(this);


	//开始移动
	this->MoveStart();
}

void ARepairMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARepairMouse::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->M_bEnableAttakLine)
	{
		this->AddAttackLineFunc(UGameSystemFunction::GetCardCollisionBoxType(this->M_ECardCollisionType), DeltaTime);
	}
}

void ARepairMouse::OnAnimationPlayEnd()
{
	if (this->GetCurrentHP() > 0.f)
	{
		if (this->bSecond)
		{
			//扶梯播放完毕，创建扶梯
			this->bSecond = false;
			this->M_bEnableAttakLine = true;

			ACardActor* CurCard = this->GetCurrentAttackCard();
			if (IsValid(CurCard) && CurCard->GetCurrentHP() > 0.f)
			{
				//判断当前卡片是否存在，如果不存在则不需要创建扶梯
				if (!this->RepairMouseEscalator.Get())
				{
					this->RepairMouseEscalator.LoadSynchronous();
				}

				ARepairMouseEscalator* Cur = this->GetWorld()->SpawnActor<ARepairMouseEscalator>(this->RepairMouseEscalator.Get());
				if (IsValid(Cur))
				{
					Cur->SetActorLocation(this->GetActorLocation());
					Cur->Init(this->bState, this->GetTranslucentSortPriority() - 2, CurCard->GetActorLocation(), CurCard);
				}
			}



			//开始移动
			this->MoveStart();
		}

		//如果老鼠处于攻击状态
		if (this->GetbIsAttack())
		{
			//攻击防御卡
			if (!this->AttackCard())
			{
				//取消攻击
				this->SetbIsAttack(false);
			}
		}

		this->UpdateState();

	}
}

void ARepairMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->UpdateMove(DeltaTime);
}

bool ARepairMouse::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	//默认攻击
	if (AttackType == EFlyItemAttackType::Def)
	{
		//更新防具血量
		if (!this->M_NormalDefenceMouseResourceStruct.bItemRemove)
		{
			this->M_NormalDefenceMouseResourceStruct.DefenceItemCurHP -= _HurtValue;
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

	if (this->GetCurrentHP() > 0.f)
	{
		if (!this->bSecond && !this->bUseFirst)
		{
			this->UpdateState(_HurtValue, AttackType);
		}
	}

	return true;
}

void ARepairMouse::AttackedBegin()
{
	Super::AttackedBegin();

	//如果已经使用了扶梯道具
	if (this->M_NormalDefenceMouseResourceStruct.bItemUse)
	{

		this->UpdateState();

	}
	else {
		//获取当前卡片类型
		ACardActor* CurCard = this->GetCurrentAttackCard();
		if (IsValid(CurCard))
		{
			if (
				CurCard->GetCardData().CardCategoryTag == ECardCategoryTag::HighBlock
				||
				CurCard->GetCardData().CardCategoryTag == ECardCategoryTag::LowBlock
				)
			{
				float Total = this->GetTotalHP();
				float CurHP = this->GetCurrentHP();
				float DefenHP = this->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP;
				float DefenCurHP = this->M_NormalDefenceMouseResourceStruct.DefenceItemCurHP;
				bool bHiDefence = DefenCurHP > DefenHP * 0.5f;
				this->bState = bHiDefence;
				if (CurHP > Total * 0.5f)
				{
					if (bHiDefence)
					{
						this->SetPlayAnimation(
							UGameSystemFunction::LoadRes(this->M_NormalDefenceMouseResourceStruct.TakeUse), true);
					}
					else {
						this->SetPlayAnimation(
							UGameSystemFunction::LoadRes(this->M_NormalDefenceMouseResourceStruct.TakeLowUse), true);
					}
				}
				else {
					if (bHiDefence)
					{
						this->SetPlayAnimation(
							UGameSystemFunction::LoadRes(this->M_NormalDefenceMouseResourceStruct.TakeCharLowUse), true);
					}
					else {
						this->SetPlayAnimation(
							UGameSystemFunction::LoadRes(this->M_NormalDefenceMouseResourceStruct.TakeAllLowUse), true);

					}
				}
				//取消攻击
				this->bUseFirst = true;
				this->M_NormalDefenceMouseResourceStruct.bItemUse = true;
				this->M_NormalDefenceMouseResourceStruct.bItemRemove = true;
				this->SetbIsAttack(false);
				return;
			}
		}



		this->UpdateState();


	}
}

void ARepairMouse::AttackedEnd()
{
	if (this->bUseFirst)
	{
		this->bUseFirst = false;
		this->bSecond = true;
		this->M_bEnableAttakLine = false;
	}
	else {
		Super::AttackedEnd();
		this->UpdateState();
	}
}

void ARepairMouse::MoveingBegin()
{
	Super::MoveingBegin();

	this->UpdateState();
}

void ARepairMouse::MouseDeathed()
{
	this->M_bEnableAttakLine = false;

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->M_NormalDefenceMouseResourceStruct.Death), true);
	}

	//关闭碰撞
	this->ClosedBoxComponent(this->MMesheComponent);
	this->ClosedBoxComponent(this->MBodyComponent);
}

void ARepairMouse::UpdateState(const float& HurtValue, const EFlyItemAttackType& AttackType)
{
	if (IsValid(this))
	{
		if (this->GetCurrentHP() <= 0.f)
		{
			return;
		}
	}
	else {
		return;
	}

	//基础的数据
	bool bAttack = this->GetbIsAttack();
	float Total = this->GetTotalHP();
	float CurHP = this->GetCurrentHP();
	float DefenHP = this->M_NormalDefenceMouseResourceStruct.DefenceItemBaseHP;
	float DefenCurHP = this->M_NormalDefenceMouseResourceStruct.DefenceItemCurHP;
	FRepairMouseResourceStruct* Cur = &this->M_NormalDefenceMouseResourceStruct;

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
						this->SetPlayAnimationOnce(
							UGameSystemFunction::LoadRes(Cur->TakeFallChar),
							UGameSystemFunction::LoadRes(Cur->CharDef));
						return;
					}
				}
				else {
					if (!bAttack)
					{
						this->SetPlayAnimationOnce(
							UGameSystemFunction::LoadRes(Cur->TakeFallCharLow),
							UGameSystemFunction::LoadRes(Cur->CharLow));

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

void ARepairMouse::UpdateState()
{
	this->UpdateState(0, EFlyItemAttackType::Def);
}

void ARepairMouse::SetAnim(const bool& bAtk, const bool& bTake, TSoftObjectPtr<UPaperFlipbook> Anim1, TSoftObjectPtr<UPaperFlipbook> Anim2, TSoftObjectPtr<UPaperFlipbook> Anim3, TSoftObjectPtr<UPaperFlipbook> Anim4)
{
	if (!IsValid(this))
	{
		return;
	}

	if (bAtk)
	{
		//拿
		if (bTake)
		{
			//攻击拿[遇到攻击状态，出现掉落强行播放攻击动画]
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim1), true);
		}
		else {
			//攻击无
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim2), true);
		}
	}
	else {
		//拿走
		if (bTake)
		{
			//攻击拿[没有攻击，遇到掉落]
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim3));
		}
		else {
			//攻击无
			this->SetPlayAnimation(UGameSystemFunction::LoadRes(Anim4));
		}
	}
}
