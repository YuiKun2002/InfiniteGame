// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"
#include "Kismet/KismetMathLibrary.h"


void AMouseActor::MouseInit()
{
	this->bMouseInit = true;

	//初始化血量
	this->M_Proper_HP.M_CurrentHP = this->M_Proper_HP.M_HP;
	//初始化攻击
	this->M_Proper_ATK.M_CurrentATK = this->M_Proper_ATK.M_ATK;
	//初始化速度
	this->M_Proper_State.M_CurrentMoveSpeed = this->M_Proper_State.M_MoveSpeed;
	//初始化更新速度
	this->M_Proper_Condition.M_CurrentPlayRate = 1.f;
	//初始化基础移动方向
	this->fBaseMoveDirection = 1.f;

	//初始化其他数据
	this->bConstMoveTime = 0.5f;
	this->bMoveTime = 0.f;
	this->MouseMoveLineScaleValue = FVector2D(0.f);
	this->bEnableForceMove = false;
	this->bForceMoveOtherLine = false;
}

void AMouseActor::MouseTick(const float& DeltaTime) {}

AMouseActor::AMouseActor()
{
	this->M_MousePosition = CreateDefaultSubobject<USceneComponent>("MouseTargetPositionComp");
	//设置依附
	this->M_MousePosition->SetupAttachment(this->GetPointComponent(), TEXT("MouseTargetLocationComp"));
}

UMouseManagerComponent* AMouseActor::GetMouseManager()
{
	return this->M_MouseManager;
}

void AMouseActor::AlienDeadAnimationCompelet(UTrackEntry* Track)
{
	this->Destroy();
}

void AMouseActor::UpdateColor()
{
	if (!this->M_Proper_Condition.M_bColor)
	{
		this->M_Proper_Condition.M_bColor = true;
		this->M_Proper_Condition.M_bHurtColorTime = 0.2f;
		this->M_Proper_State.CurOpacity = 0.4f;

		/*this->SetFlipbookColor(FVector(
			this->GetMyActor()->GetSpriteColor().R,
			this->GetMyActor()->GetSpriteColor().G,
			this->GetMyActor()->GetSpriteColor().B
		), this->M_Proper_State.CurOpacity);*/

		this->SetSpineRenderColor(
			FLinearColor(
				this->GetSpineRenderColor().R,
				this->GetSpineRenderColor().G,
				this->GetSpineRenderColor().B,
				this->M_Proper_State.CurOpacity
			)
		);
	}
}

void AMouseActor::ParseBuff_Information(const FItem_Buff& _Buff)
{
	if (_Buff.M_Buffers.Num())
	{
		FGameBuffInfor Infor;
		//转换buff
		for (const auto& CurInfor : _Buff.M_Buffers)
		{
			switch (CurInfor.Key)
			{
			case Buff_Infor::E_SlowDown:
				Infor.CurBuffs.Emplace(EGameBuffTag::SlowDown, CurInfor.Value);
				break;
			case Buff_Infor::E_Freeze:
				Infor.CurBuffs.Emplace(EGameBuffTag::Freeze, CurInfor.Value);
				break;
			case Buff_Infor::E_Solidification:
				Infor.CurBuffs.Emplace(EGameBuffTag::Solidification, CurInfor.Value);
				break;
			case Buff_Infor::E_Accelerate:
				Infor.CurBuffs.Emplace(EGameBuffTag::Accelerate, CurInfor.Value);
				break;
			case Buff_Infor::Burn:
				Infor.CurBuffs.Emplace(EGameBuffTag::Burn, CurInfor.Value);
				break;
			default:
				break;
			}
		}
		//添加buff
		this->M_Buff->AddBuffInfor(Infor);
	}
}

void AMouseActor::MouseKill()
{
	if (!this->bMouseInit)
	{
		return;
	}

	//将当前老鼠从线路管理器中移除
	UMouseManagerComponent* MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;

	//生成死亡个数
	UResourceManagerComponent::ResourceAddKillMouse(this->MouseObjName);

	if (IsValid(MouseManager))
	{
		//生成专属掉落物品
		if (this->CurMouseLevelItems.Num())
		{
			//立刻生成
			for (const FString& CurItem : this->CurMouseLevelItems)
			{
				UResourceManagerComponent::SpawnItemByName(CurItem, this->GetActorLocation());
			}
		}

		//生成回合掉落物
		UResourceManagerComponent::SpawnItem(this->GetActorLocation());

		//生成金币资源
		UResourceManagerComponent::SpawnItemCoin(this->GetActorLocation());

		//生成经验值
		UResourceManagerComponent::ResourceAddEx(this->M_Proper_State.M_CurrentMouseEx);

		//添加得分
		if (this->GetMouseBaseType() == EMouseBaseType::Boss)
		{
			UResourceManagerComponent::ResourceAddScore(UGameSystemFunction::GetRandomRange(95, 100));
		}
		else if (this->GetMouseBaseType() == EMouseBaseType::Other)
		{
			UResourceManagerComponent::ResourceAddScore(UGameSystemFunction::GetRandomRange(1, 5));
		}
		else {
			UResourceManagerComponent::ResourceAddScore(UGameSystemFunction::GetRandomRange(5, 10));
		}

		if (this->CurMouseDeathForceGameWin)
		{
			MouseManager->ForceNextEndRound();
		}
		else {
			MouseManager->RemoveMouse(this);
		}
	}
	else {
		this->Destroy();
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			FString(UGameSystemFunction::GetObjectName(this) + TEXT("死亡,") +
				FString::FromInt(this->GetTotalHP()) + TEXT("总血量")
			)
		);
	}
}

bool AMouseActor::UpdateHP(float _HurtValue)
{
	this->UpdateColor();

	//血量更新
	if (_HurtValue < 0.f)
	{
		this->M_Proper_HP.M_CurrentHP += _HurtValue * -1.f;
	}
	else
	{
		//计算伤害
		this->M_Proper_HP.M_CurrentHP -= _HurtValue - (_HurtValue * this->M_Proper_HP.M_IgnoreHurtValue);
	}

	if (this->M_Proper_HP.M_CurrentHP >= this->GetTotalHP())
	{
		this->M_Proper_HP.M_CurrentHP = this->GetTotalHP();
	}

	if (this->M_Proper_HP.M_CurrentHP <= 0.f)
	{
		this->M_Proper_HP.M_CurrentHP = 0.f;

		//触发老鼠死亡
		this->SetMouseDeath(true);

		return true;
	}

	//结束受攻击状态(更新血量的函数一般在受到伤害时才会触发)
	this->SetbIsHurt(false);

	return false;
}

void AMouseActor::SetHP(float HP)
{
	this->M_Proper_HP.M_HP = HP;
	this->M_Proper_HP.M_CurrentHP = HP;
}

float AMouseActor::GetCurrentHP()
{
	return this->M_Proper_HP.M_CurrentHP;
}

float AMouseActor::GetTotalHP()
{
	return this->M_Proper_HP.M_HP;
}

float AMouseActor::GetCurrentMoveSpeed()
{
	return this->M_Proper_State.M_CurrentMoveSpeed;
}

void AMouseActor::SetCurrentMoveSpeed(const float _Value)
{
	this->M_Proper_State.M_CurrentMoveSpeed = _Value;
}

float AMouseActor::GetMoveSpeed()
{
	return this->M_Proper_State.M_MoveSpeed;
}

void AMouseActor::SetMoveSpeed(const float _Value)
{
	this->M_Proper_State.M_MoveSpeed = _Value;
	this->SetCurrentMoveSpeed(_Value);
}

float AMouseActor::GetCurrentATK()
{
	return this->M_Proper_ATK.M_CurrentATK;
}

float AMouseActor::GetATK()
{
	return this->M_Proper_ATK.M_ATK;
}

void AMouseActor::SetMouseATK(float _Value)
{
	this->M_Proper_ATK.M_CurrentATK = _Value;
	this->M_Proper_ATK.M_ATK = _Value;
}

bool AMouseActor::GetbIsHurt()
{
	return this->M_Proper_ATK.IsHurt;
}

void AMouseActor::SetbIsHurt(bool _Value)
{
	this->M_Proper_ATK.IsHurt = _Value;

	if (_Value)
	{
		//第一次受到攻击时
		this->BeAttakedBegin();
	}
}

float AMouseActor::GetHurtValue()
{
	return this->M_Proper_ATK.HurtValue;
}

void AMouseActor::SetHurtValue(float _Value)
{
	this->M_Proper_ATK.HurtValue = _Value;
}

void AMouseActor::SetIgnoreHurtValue(float Value)
{
	//范围限制
	if (Value > 1.f)
	{
		Value = 1.f;
	}
	else if (Value < 0.f) {
		Value = 0.f;
	}

	this->M_Proper_HP.M_IgnoreHurtValue = Value;
}

bool AMouseActor::GetbIsAttack() const
{
	return this->M_Proper_ATK.IsAttack;
}

void AMouseActor::SetbIsAttack(bool _Value)
{
	this->M_Proper_ATK.IsAttack = _Value;

	if (_Value)
	{
		//开始攻击
		this->AttackedBegin();
	}
	else
	{
		//攻击结束
		this->AttackedEnd();
	}
}

bool AMouseActor::MoveStart()
{
	if (!this->M_Proper_State.M_IsMove)
	{
		this->M_Proper_State.M_IsMove = true;

		//开始移动时调用一次
		this->MoveingBegin();

		return true;
	}
	return false;
}

bool AMouseActor::MoveStop()
{
	if (this->M_Proper_State.M_IsMove)
	{
		this->M_Proper_State.M_IsMove = false;

		//结束移动时调用一次
		this->MoveingStop();

		return true;
	}
	return false;
}

bool AMouseActor::GetbIsMove()
{
	return M_Proper_State.M_IsMove;
}

void AMouseActor::SetMouseLineType(ELineType _Type)
{
	this->M_Proper_Condition.M_EMouseType = _Type;
}

ELineType AMouseActor::GetMouseLineType()
{
	return this->M_Proper_Condition.M_EMouseType;
}

FLine& AMouseActor::GetMouseLine()
{
	return this->M_Proper_Condition.M_Line;
}

void AMouseActor::SetMouseLine(FLine _Line)
{
	this->M_Proper_Condition.M_Line = _Line;
}

USceneComponent* const AMouseActor::GetMouseLocation()
{
	return this->M_MousePosition;
}

void AMouseActor::SetMouseLocation(USceneComponent* _Position)
{
	this->M_MousePosition = _Position;
}

bool AMouseActor::GetMouseIsDeath()
{
	return this->M_Proper_State.MouseDeath;
}

void AMouseActor::SetMouseDeath(bool _Value)
{
	if (!this->bMouseInit)
	{
		return;
	}

	if (this->M_Proper_State.IsDeath || this->M_Proper_State.bPlayDeathAnim)
	{
		return;
	}

	this->M_Proper_State.IsDeath = _Value;
	this->M_Proper_State.MouseDeath = _Value;
	this->M_Proper_State.bPlayDeathAnim = _Value;
	this->M_Proper_HP.M_CurrentHP = 0.f;

	if (_Value)
	{
		//停止移动
		this->MoveStop();
		//移除老鼠
		this->MouseKill();

		/*Spine Tag 老鼠死亡 Spine标记*/

		//设置播放结束销毁
		//this->SetAnimationPlayEndDestroy();
		//老鼠死亡
		this->MouseDeathed();
	}
}

int32 AMouseActor::GetMouseRound()
{
	return this->M_Proper_Condition.M_AttachRound;
}

void AMouseActor::SetMouseRound(int32 _Value)
{
	this->M_Proper_Condition.M_AttachRound = _Value;
}

ACardActor* AMouseActor::GetCurrentAttackCard()
{
	return this->M_CurrentAttackCardActor;
}

bool AMouseActor::GetbAttackState() const
{
	//如果老鼠处于攻击状态
	if (!this->M_Buff->GetConstBuff() && this->GetbIsAttack())
	{
		return true;
	}
	return false;
}

ACardActor* AMouseActor::SetCurrentAttackCard(ACardActor* _Card)
{
	this->M_CurrentAttackCardActor = _Card;
	return _Card;
}

void AMouseActor::SetHitSelfFlyActor(AFlyItemActor* _HitSelfFlyActor)
{
	this->M_HitSelfFlyActor = _HitSelfFlyActor;
}

FVector AMouseActor::GetMouseTargetPosition()
{
	return 	this->M_MousePosition->GetComponentLocation();
}

FString AMouseActor::GetPath(const FString& _SourcePath, const FString& _Name)
{
	return FString(_SourcePath + _Name + TEXT(".") + _Name + TEXT("'"));
}

EMouseTag AMouseActor::GetMouseTag()
{
	return this->M_Proper_State.M_MouseTag;
}

EMouseBaseType AMouseActor::GetMouseBaseType()
{
	return EMouseBaseType::Normal;
}

bool AMouseActor::GetMouseMoveOtherLine() const
{
	return this->bForceMoveOtherLine && this->bEnableForceMove;
}

float AMouseActor::GetMouseBaseMoveDirection()
{
	return this->fBaseMoveDirection;
}

float AMouseActor::GetIgnoreHurtValue() const
{
	return this->M_Proper_HP.M_IgnoreHurtValue;
}

float AMouseActor::GetColorOpacity() const
{
	return this->M_Proper_State.CurOpacity;
}

UTrackEntry* AMouseActor::GetTrackEntry()
{
	return this->CurAnimTrackEntry;
}

void AMouseActor::SetMouseTag(EMouseTag NewTag)
{
	this->M_Proper_State.M_MouseTag = NewTag;
}

void AMouseActor::SetMaterial(UMaterialInstance* NewMaterial)
{
	this->GetRenderComponent()->SetMaterial(0, NewMaterial);

	if (
		this->M_Proper_Condition.M_EMouseType == ELineType::OnGround
		&&
		this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater
		)
	{
		this->GetRenderComponent()->
			SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), 0.2f);
		this->GetRenderComponent()->
			SetScalarParameterValueOnMaterials(FName(TEXT("Range")), this->MouseInWaterRate);
	}
	else {
		this->GetRenderComponent()->
			SetScalarParameterValueOnMaterials(FName(TEXT("Range")), 0.f);
	}
}

void AMouseActor::SetMouseInWaterRate(float Rate)
{
	this->GetRenderComponent()->
		SetScalarParameterValueOnMaterials(FName(TEXT("Range")), Rate);
}

void AMouseActor::SetMouseBaseMoveDirection(bool bMoveFront)
{
	if (bMoveFront)
	{
		this->fBaseMoveDirection = 1.f;
	}
	else {
		this->fBaseMoveDirection = -1.f;
	}
}

void AMouseActor::SetEx(const int32& _Ex)
{
	this->M_Proper_State.M_CurrentMouseEx = _Ex;
}

void AMouseActor::SetSpeedReset()
{
	this->SetCurrentMoveSpeed(this->GetMoveSpeed());
}

void AMouseActor::SetCurMouseDeathForceGameWin(bool Enable)
{
	this->CurMouseDeathForceGameWin = Enable;
}

void AMouseActor::SetMouseLevelItems(const TArray<FString>& Items)
{
	this->CurMouseLevelItems = Items;
}

void AMouseActor::SetMouseCollisionType(UPrimitiveComponent* _Collison, UPrimitiveComponent* _MouseBodyOverlap, ECollisionChannel _MouseType)
{
	if (_Collison && _MouseBodyOverlap)
	{
		_Collison->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		_Collison->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		_Collison->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		_Collison->SetCollisionResponseToChannel(_MouseType, ECollisionResponse::ECR_Block);
		_Collison->SetCollisionObjectType(ECC_Pawn);

		_MouseBodyOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		_MouseBodyOverlap->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		_MouseBodyOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		_MouseBodyOverlap->SetCollisionObjectType(ECC_Pawn);
	}
}

void AMouseActor::SetTrackEntry(UTrackEntry* NewTrackEntry)
{
	if (IsValid(this->CurAnimTrackEntry))
	{
		this->CurAnimTrackEntry->AnimationComplete.Clear();
		this->CurAnimTrackEntry->AnimationComplete.RemoveAll(this);
	}

	this->CurAnimTrackEntry = NewTrackEntry;
}

void AMouseActor::BPInMapMeshe(ELineType CurLineType)
{
	this->InMapMeshe(CurLineType);
}

void AMouseActor::ClosedBoxComponent(UPrimitiveComponent* _Box_Comp)
{
	if (IsValid(_Box_Comp))
	{
		_Box_Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMouseActor::EnableBoxComponent(UPrimitiveComponent* _Box_Comp)
{
	if (IsValid(_Box_Comp))
	{
		_Box_Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

bool AMouseActor::AttackCard()
{
	//当前卡片无效
	if (!IsValid(this->GetCurrentAttackCard()))
	{
		return false;
	}

	//当前卡片有效
	if (IsValid(this->GetCurrentAttackCard()) && this->GetCurrentAttackCard()->GetCurrentHP() > 0.f)
	{
		//设置当前攻击该卡片的老鼠
		this->GetCurrentAttackCard()->SetCurrentAttackSelfMouse(this);
		//播放【啃】BGM
		UFVMGameInstance::PlayBGM_S_Static(TEXT("Eat"), TEXT("ItemAudio"));
		//当卡片死亡时
		if (this->GetCurrentAttackCard()->UpdateCardState(this->GetCurrentATK(), 0.f))
		{
			//销毁卡片
			this->GetCurrentAttackCard()->KillCard();
			//卡片摧毁
			UResourceManagerComponent::ResourceAddBadCard();
			return false;
		}

		//调用卡片被攻击的函数
		this->GetCurrentAttackCard()->CardBeHurt(this);

		//卡片没有死亡 //还有目标继续攻击
		return true;
	}

	return true;
}

void AMouseActor::SetMouseMoveOtherLine(FLine NewLine)
{
	this->SetMouseMoveOtherLineFunc(NewLine);
}

void AMouseActor::ForceSetWaterAnimShow(ELineType CurLineType)
{
	if (CurLineType == ELineType::OnWater && this->M_Proper_Condition.M_CurrentInType != ELineType::OnWater)
	{
		if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnGround)
		{
			//显示入水动画
			//this->InWaterAnim->SetHiddenInGame(false);
			//this->InWaterAnim->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 1.f));


			for (UWaterSceneComponent*& Water : this->WaterComps)
			{
				Water->SpawnWater();
			}

			//设置透明度
			//this->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("Range")),
			//	this->MouseInWaterRate);
		}
	}
	else {
		//陆地老鼠上岸
		if (CurLineType != ELineType::OnWater && this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
		{
			//显示入水动画
			//this->InWaterAnim->SetHiddenInGame(true);
			//this->InWaterAnim->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 0.f));

			for (UWaterSceneComponent*& Water : this->WaterComps)
			{
				Water->End();
			}

			//设置透明度
			this->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("Range")), 0);
		}
	}

	//赋值当前网格类型
	this->M_Proper_Condition.M_CurrentInType = CurLineType;
}

void AMouseActor::SetInWaterAnimEnable(bool Value)
{
	this->bInWaterAnimState = Value;
}

void AMouseActor::DisEnableForceMove()
{
	this->bEnableForceMove = false;
}

void AMouseActor::UpdateMove(const float& DeltaTime)
{
	this->UpdateMoveFunc(DeltaTime);
}

void AMouseActor::CloseInWaterTimeLine()
{
	if (IsValid(this->InWaterTimeLine))
	{
		this->InWaterTimeLine->End();
	}
}

void AMouseActor::AddAttackLine(
	const FVector& _BeginOffset,
	const FVector& _EndOffset,
	FColor _Color,
	ECollisionChannel _ECollisionChannel,
	float DeltaTime,
	bool DirectionFront
)
{
	//如果强行移动，则不会进行攻击
	if (this->bForceMoveOtherLine && this->bEnableForceMove)
	{
		return;
	}

	//添加射线
	FHitResult Hit;

	if (UFVMGameInstance::GetDebug())
	{
		DrawDebugLine(this->GetWorld(),
			this->GetActorLocation() + _BeginOffset,
			this->GetActorLocation() + _EndOffset,
			_Color, false, 3.f);
	}

	if (this->GetWorld()->LineTraceSingleByChannel(Hit,
		this->GetActorLocation() + _BeginOffset,
		this->GetActorLocation() + _EndOffset,
		_ECollisionChannel))
	{
		ACardActor* CurCard = Cast<ACardActor>(Hit.GetActor());
		if (IsValid(CurCard))
		{
			this->SetCurrentAttackCard(CurCard);
			this->SetbIsAttack(true);
			this->MoveStop();
		}
		else {

		}
	}
	else {

		if (!this->GetbIsAttack())
		{
			return;
		}

		this->SetCurrentAttackCard(nullptr);

		this->SetbIsAttack(false);

		if (this->GetMouseIsDeath())
		{
			this->MoveStop();
		}
		else {
			this->MoveStart();
		}

	}
}

void AMouseActor::AddAttackLineFunc(const ECollisionChannel& CollisionType, const float& DeltaTime, bool DirectionFront)
{
	if (this->fAttackLineDelay > 0.f)
	{
		this->fAttackLineDelay -= DeltaTime;
	}
	else {
		this->fAttackLineDelay = 0.05f;
		if (DirectionFront)
		{
			if (
				this->M_Proper_Condition.M_EMouseType == ELineType::OnGround
				&&
				this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater
				)
			{
				this->AddAttackLine(FVector(0.f, 0.f, this->fMouseInWaterZ + 15.f),
					FVector(0.f, 15.f, this->fMouseInWaterZ + 15.f),
					FColor::Red, CollisionType, DeltaTime, DirectionFront);

				return;
			}

			this->AddAttackLine(FVector::ZeroVector,
				FVector(0.f, 15.f, 0.f),
				FColor::Red, CollisionType, DeltaTime, DirectionFront);
		}
		else {
			if (
				this->M_Proper_Condition.M_EMouseType == ELineType::OnGround
				&&
				this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater
				)
			{
				this->AddAttackLine(FVector(0.f, 0.f, this->fMouseInWaterZ + 15.f),
					FVector(0.f, -15.f, this->fMouseInWaterZ + 15.f),
					FColor::Red, CollisionType, DeltaTime, DirectionFront);

				return;
			}

			this->AddAttackLine(FVector::ZeroVector,
				FVector(0.f, -15.f, 0.f),
				FColor::Red, CollisionType, DeltaTime, DirectionFront);
		}
	}
}

void AMouseActor::AddAttackCardUpdate()
{
	//如果老鼠处于攻击状态
	if (this->GetbAttackState())
	{
		//攻击防御卡
		if (!this->AttackCard())
		{
			//取消攻击
			this->SetbIsAttack(false);
		}
	}
}

void AMouseActor::AddWaterComponent(UWaterSceneComponent* WaterComp)
{
	this->WaterComps.Emplace(WaterComp);
}

void AMouseActor::SetMouseMoveOtherLineFunc(FLine NewLine)
{
	this->bForceMoveOtherLine = true;
	this->SetbIsAttack(false);
	this->M_CurrentAttackCardActor = nullptr;
	//初始化移动时间
	this->bMoveTime = 0.f;
	//初始化老鼠的位置
	this->MouseMoveLineScaleValue.X = this->GetActorLocation().Z;
	//初始化老鼠位移的位置
	this->MouseMoveLineScaleValue.Y =
		AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMesh(NewLine.Row, 0)->
		GetActorLocation().Z - 15.f;
	//切换线路
	this->GetMouseManager()->ChangeLine(UKismetSystemLibrary::GetObjectName(this),
		this->GetMouseLine().Row, NewLine.Row);
	//重新设置新路线
	this->SetMouseLine(NewLine);
	//设置渲染层级
	this->SetRenderLayer(
		AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMesh(
			NewLine.Row, NewLine.Col
		)->GetTranslucency()
	);
}

void AMouseActor::UpdateMoveFunc(const float& DeltaTime)
{
	if (!this->bEnableForceMove)
	{
		this->bEnableForceMove = true;
	}

	if (this->bForceMoveOtherLine)
	{
		if (this->bMoveTime < this->bConstMoveTime)
		{
			this->bMoveTime += DeltaTime;

			float InWaterZ = 0.f;
			//判断当前网格位置
			if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
			{
				InWaterZ = this->fMouseInWaterZ;
			}
			float TargetLocationZ = UKismetMathLibrary::Lerp(this->MouseMoveLineScaleValue.X, this->MouseMoveLineScaleValue.Y - InWaterZ, this->bMoveTime / this->bConstMoveTime);
			this->AddActorLocalOffset(FVector(0.f, (DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed) * this->fBaseMoveDirection, 0.f));
			FVector CurLoca = this->GetActorLocation();
			this->SetActorLocation(FVector(CurLoca.X, CurLoca.Y, TargetLocationZ));
		}
		else {
			this->bMoveTime = 0.f;
			this->bForceMoveOtherLine = false;
			this->AddActorLocalOffset(FVector(0.f, (DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed) * this->fBaseMoveDirection, 0.f));
			FVector CurLoca = this->GetActorLocation();
			//判断当前网格位置
			if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
			{
				this->SetActorLocation(FVector(CurLoca.X, CurLoca.Y, this->MouseMoveLineScaleValue.Y - this->fMouseInWaterZ));
				this->ForceSetWaterAnimShow(ELineType::OnWater);
			}
			else {
				this->SetActorLocation(FVector(CurLoca.X, CurLoca.Y, this->MouseMoveLineScaleValue.Y));
				this->ForceSetWaterAnimShow(ELineType::OnGround);
			}
		}
	}
	else {
		this->AddActorLocalOffset(FVector(0.f, (DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed) * this->fBaseMoveDirection, 0.f));
	}
}

void AMouseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVM_VS_GamePause())
	{
		return;
	}

	//根据当前buff的状态来决定更新速度
	this->MouseTick(DeltaTime * this->M_Buff->GetTickRate());

	//如果允许移动
	if (this->GetbIsMove())
	{
		if (this->GetCurrentHP() <= 0.f)
		{
			return;
		}

		if (!this->M_Buff->GetConstBuff())
		{
			this->MoveingUpdate(DeltaTime * this->M_Buff->GetTickRate());

			for (UWaterSceneComponent*& Water : this->WaterComps)
			{
				Water->Move(DeltaTime * this->M_Buff->GetTickRate());
			}
		}
	}

	//如果buff存在更新buff
	if (this->M_Buff->GetBuffExist())
	{
		this->M_Buff->UpdateBuff(DeltaTime);
	}

	//被攻击恢复颜色（恢复默认颜色）
	if (this->M_Proper_Condition.M_bColor)
	{
		if (this->M_Proper_Condition.M_bHurtColorTime > 0.f)
		{
			this->M_Proper_Condition.M_bHurtColorTime -= DeltaTime;

			if (this->M_Proper_Condition.M_bHurtColorTime <= 0.f)
			{
				this->M_Proper_Condition.M_bColor = false;
				this->M_Proper_State.CurOpacity = 1.f;
				this->SetSpineRenderColor(
					FLinearColor(
						this->GetSpineRenderColor().R,
						this->GetSpineRenderColor().G,
						this->GetSpineRenderColor().B,
						this->M_Proper_State.CurOpacity
					)
				);
			}
		}
	}

	if (IsValid(this->InWaterTimeLine))
	{
		if (this->GetCurrentHP() <= 0.f)
		{
			return;
		}

		this->InWaterTimeLine->Tick(DeltaTime);
	}
}

void AMouseActor::BeginPlay()
{
	Super::BeginPlay();

	//获取老鼠管理器
	this->M_MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
	if (!this->M_MouseManager)
	{
		this->SetActorTickEnabled(false);
	}

	//加载动画名称
	//UGameSystemFunction::GetAssetCategoryName(this->M_DefAnim_Anim.WalkAnimRes);

	//设置材质
	/*this->GetRenderComponent()->SetMaterial(0, LoadObject<UMaterialInstance>(this,
		TEXT("MaterialInstanceConstant'/Game/Resource/BP/Martials/Mouse/MI_MouseRender.MI_MouseRender'")
	));*/

	//生成buff
	this->M_Buff = UGameBuff::MakeGameBuff(this, EGameBuffCharTag::Mouse);
	this->M_Buff->OnExecuteBuffDelegate.BindUObject(this, &AMouseActor::ExecuteBuff);
	this->M_Buff->OnExitBuffDelegate.BindUObject(this, &AMouseActor::ExitBuff);
	this->M_Buff->ClearBuffs();

	this->M_Proper_HP = FMouseProperty_HP();
	this->M_Proper_ATK = FMouseProperty_ATK();
	this->M_Proper_State = FMouseProperty_State();
	this->M_Proper_Condition = FMouseProperty_Condition();

	//移动
	this->MoveStart();

	//绑定死亡动画播放结束
	//this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	//设置老鼠播放死亡动画
	//this->M_Proper_State.bPlayDeathAnim = false;
	//允许移动
	//this->MoveStart();


	this->bForceMoveOtherLine = false;
	this->bEnableForceMove = false;
}

//移动(每0.02ms自动调用(允许移动的情况下))
void AMouseActor::MoveingUpdate(float DeltaTime)
{
}

//停止移动(每0.02ms自动调用(不能移动的情况下))
void AMouseActor::MoveingStop() {
}

//开始移动(当设置开始移动时调用一次)
void AMouseActor::MoveingBegin()
{
}

//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
void AMouseActor::AttackedBegin()
{
}

void AMouseActor::AttackedEnd()
{
}

//老鼠受到攻击((当设置isHurt为true时)调用一次)
void AMouseActor::BeAttakedBegin()
{
}

//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
bool AMouseActor::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	return true;
}

//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)【如果在此处调用了 Destroy()函数，将不会执行MouseKill(),当前老鼠死亡将不会记录到老鼠生成器的个数中 】
void AMouseActor::MouseDeathed()
{
	//绑定死亡动画播放结束
	//this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();

	/*Spine Tag 老鼠死亡 Spine标记*/

	//释放当前的所有攻击目标
	this->SetCurrentAttackCard(nullptr);
	//取消攻击
	this->M_Proper_ATK.IsAttack = false;
	//解除所有buff
	this->M_Buff->EndBuffs();
	//取消移动
	this->MoveStop();
}


void AMouseActor::InMapMeshe(ELineType CurLineType)
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (this->M_Proper_Condition.M_CurrentInType == CurLineType)
	{
		return;
	}

	if (!this->bInWaterAnimState)
	{
		this->M_Proper_Condition.M_CurrentInType = CurLineType;
	}

	//如果出现强制移动
	if (this->bForceMoveOtherLine)
	{
		this->ForceSetWaterAnimShow(CurLineType);
		return;
	}

	//陆地老鼠入水
	if (CurLineType == ELineType::OnWater && this->M_Proper_Condition.M_CurrentInType != ELineType::OnWater)
	{
		if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnGround)
		{
			//显示入水动画
			//this->InWaterAnim->SetHiddenInGame(false);
			//this->InWaterAnim->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 0.f));

			for (UWaterSceneComponent*& Water : this->WaterComps)
			{
				Water->SpawnWater();
			}

			this->M_Proper_Condition.M_CurrentInType = ELineType::OnWater;
			//得到z轴位置
			this->fCurInWaterZ = this->GetActorLocation().Z;
			//陆地老鼠下水
			this->InWaterTimeLine = UTimeLineClass::MakeTimeLineClass();
			this->InWaterTimeLine->AddCurve(
				TSoftObjectPtr<UCurveFloat>(FSoftObjectPath(
					TEXT("CurveFloat'/Game/Resource/BP/GameStart/Item/Mouse/Curve/MouseInWater.MouseInWater'"))),
				this,
				[](UTimeLineClass* TimeLine, UObject* CurMouse, float time) {
					//位移坐标
					float TargetZ = UKismetMathLibrary::Lerp(
						Cast<AMouseActor>(CurMouse)->fCurInWaterZ, Cast<AMouseActor>(CurMouse)->fCurInWaterZ - Cast<AMouseActor>(CurMouse)->fMouseInWaterZ, time);
					FVector Location = Cast<AMouseActor>(CurMouse)->GetActorLocation();
					Location.Z = TargetZ;
					Cast<AMouseActor>(CurMouse)->SetActorLocation(Location);

					//Cast<AMouseActor>(CurMouse)->InWaterAnim->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, time));

					Cast<AMouseActor>(CurMouse)->GetRenderComponent()->
						SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), 0.2f);

					float Value = UKismetMathLibrary::Lerp(0.f, Cast<AMouseActor>(CurMouse)->MouseInWaterRate, time);
					Cast<AMouseActor>(CurMouse)->GetRenderComponent()->
						SetScalarParameterValueOnMaterials(FName(TEXT("Range")), Value);

				}, [](UTimeLineClass* TimeLine, UObject* CurMouse) {

					AMouseActor* Cur = Cast<AMouseActor>(CurMouse);
					if (TimeLine->GetStart())
					{
						FVector Location = Cast<AMouseActor>(CurMouse)->GetActorLocation();
						Location.Z = Cur->fCurInWaterZ - Cur->fMouseInWaterZ;
						Cur->SetActorLocation(Location);
						Cur->GetRenderComponent()->
							SetScalarParameterValueOnMaterials(FName(TEXT("Range")), Cur->MouseInWaterRate);
						//Cast<AMouseActor>(CurMouse)->InWaterAnim->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 1));
						//Cast<AMouseActor>(CurMouse)->InWaterAnim->SetHiddenInGame(false);

						for (UWaterSceneComponent*& Water : Cast<AMouseActor>(CurMouse)->WaterComps)
						{
							Water->SpawnWater();
						}
					}
					else {
						FVector Location = Cast<AMouseActor>(CurMouse)->GetActorLocation();
						Location.Z = Cur->fCurInWaterZ + Cur->fMouseInWaterZ;
						Cur->SetActorLocation(Location);
						Cur->GetRenderComponent()->
							SetScalarParameterValueOnMaterials(FName(TEXT("Range")), 0.f);
						//Cast<AMouseActor>(CurMouse)->InWaterAnim->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 0));
						//Cast<AMouseActor>(CurMouse)->InWaterAnim->SetHiddenInGame(true);

						for (UWaterSceneComponent*& Water : Cast<AMouseActor>(CurMouse)->WaterComps)
						{
							Water->End();
						}
					}
					}
					);
			this->InWaterTimeLine->PlayFromStart();
		}
	}
	else {
		//陆地老鼠上岸
		if (CurLineType != ELineType::OnWater && this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
		{

			//this->InWaterAnim->SetHiddenInGame(true);

			for (UWaterSceneComponent*& Water : this->WaterComps)
			{
				Water->End();
			}

			this->M_Proper_Condition.M_CurrentInType = CurLineType;

			if (IsValid(this->InWaterTimeLine))
			{
				this->InWaterTimeLine->ReverseFromEnd();
			}
		}
	}

	//赋值当前网格类型
	this->M_Proper_Condition.M_CurrentInType = CurLineType;
}

void AMouseActor::SetRenderLayer(int32 _Layer)
{
	Super::SetRenderLayer(_Layer + 19);

	//this->InWaterAnim->SetTranslucency(_Layer + 20);

	for (UWaterSceneComponent*& Water : this->WaterComps)
	{
		Water->RenderLayer(_Layer + 19);
	}
}

void AMouseActor::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (UFVMGameInstance::GetDebug())
	{
		this->M_Buff->DebugLog(UGameSystemFunction::GetObjectName(this), BuffTag, true);
	}
}

void AMouseActor::ExitBuff(EGameBuffTag BuffTag)
{
	if (UFVMGameInstance::GetDebug())
	{
		this->M_Buff->DebugLog(UGameSystemFunction::GetObjectName(this), BuffTag, false);
	}
}
