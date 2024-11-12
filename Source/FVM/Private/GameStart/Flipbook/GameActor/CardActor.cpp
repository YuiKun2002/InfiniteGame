
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "Data/GameBuff.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameStart/VS/GameMapInstance.h"

#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/UI/UI_Card.h"

#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Components/BoxComponent.h>
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include <Components/StaticMeshComponent.h>
#include "GameStart/VS/CardLevelActor.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

//卡片功能组件
#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include "GameStart/VS/GameMapInstance.h"

//玩家背包数据
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"


void ACardActor::SetCardActor(const FItemCard& _CardData)
{
	this->SetCardHP(_CardData.M_CardHP, 0.f);
	this->SetCardType(_CardData.M_ELineType, _CardData.M_CardLayer);
	this->SetCardGrade(this->GetCardGrade(_CardData.ItemName.ToString()));
	this->M_CardProperty.M_CardName = _CardData.ItemName.ToString();
	this->M_CardProperty.M_CardDay = _CardData.M_CardDay;
	this->M_CardProperty.M_CardID = _CardData.M_CardLayer;
	this->M_SourceCardDataBase = _CardData;
}

UUI_Card* const ACardActor::GetCardUI()
{
	return this->M_UUI_Card;
}

void ACardActor::SetCardUI(UUI_Card* _card_ui)
{
	this->M_UUI_Card = _card_ui;
}

void ACardActor::SetFloatModeEnable(bool _Value)
{
	this->M_bFloatMode = _Value;
}

FLine ACardActor::GetLine()
{
	return this->M_CardProperty.M_Line;
}

int32 ACardActor::GetCardID() const
{
	return this->M_CardProperty.M_CardID;
}

const ELineType& ACardActor::GetLineType() const
{
	return this->M_CardProperty.M_ELineType;
}

void ACardActor::SetCardGrade(const int32& _CardGrade)
{
	this->M_SourceCardDataBase.M_CardGrade = _CardGrade;

	if (this->M_SourceCardDataBase.M_CardGrade < 0)
	{
		this->M_SourceCardDataBase.M_CardGrade = 0;
	}
	else if (this->M_SourceCardDataBase.M_CardGrade > 16)
	{
		this->M_SourceCardDataBase.M_CardGrade = 16;
	}

	if (_CardGrade > 3 && _CardGrade < 17)
	{
		if (!IsValid(this->CardLevelActor))
		{
			this->CardLevelActor = this->GetWorld()->SpawnActor<ACardLevelActor>(
				LoadClass<ACardLevelActor>(nullptr,
					TEXT("Blueprint'/Game/Resource/SpineData/卡片等级动画/BP_CardLevel.BP_CardLevel_C'"))
			);

			if (IsValid(this->CardLevelActor))
			{
				FVector Location = this->GetActorLocation();
				FVector PointLocation(
					this->GetPointComponent()->GetRelativeLocation().Y + Location.X,
					this->GetPointComponent()->GetRelativeLocation().X + Location.Y,
					this->GetPointComponent()->GetRelativeLocation().Z + Location.Z
				);
				FVector LevelPointLocation = FVector::ZeroVector;

				FVector Offset = FVector(1.f, 0.f, 0.f);
				if (this->bCustomCardLavelLocation)
				{
					LevelPointLocation = FVector(
						this->CardLevelLocationComp->GetRelativeLocation().Y + PointLocation.X,
						this->CardLevelLocationComp->GetRelativeLocation().X + PointLocation.Y,
						this->CardLevelLocationComp->GetRelativeLocation().Z + PointLocation.Z
					);
				}
				else {
					float Z = (_CardGrade > 9) ? 30.f : 35.f;
					LevelPointLocation = PointLocation;
					LevelPointLocation.Z =
						AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->
						GetMapMeshLocation(
							this->GetLine().Row,
							this->GetLine().Col
						).Z - Z;
				}

				this->CardLevelActor->SetCardLevelLocation(
					LevelPointLocation,
					_CardGrade,
					this->GetRenderLayer(),
					Offset
				);
			}
			else {
				UGameSystemFunction::FVMLog(__FUNCTION__,
					TEXT("卡片等级资源加载失败，请检查资源位置！"));
			}
		}
	}
}

void ACardActor::SetLine(const FLine& _Line)
{
	this->M_CardProperty.M_Line = _Line;
}

void ACardActor::SetCardType(const ELineType& _Type, const int32& _CardID)
{
	this->M_CardProperty.M_ELineType = _Type;
	this->M_CardProperty.M_CardID = _CardID;
}

float ACardActor::GetCurrentHP()
{
	return this->M_FCardActor_HP.M_Life;
}

FString ACardActor::GetCardName()
{
	return this->M_CardProperty.M_CardName;
}

float ACardActor::GetTotalHP()
{
	return this->M_FCardActor_HP.M_TotalLife;
}

bool ACardActor::GetFloatMode() const
{
	return this->M_bFloatMode;
}

const FItemCard& ACardActor::GetCardData()
{
	return this->M_SourceCardDataBase;
}

void ACardActor::SetCardHP(const float& _HP, float _HPUpRate)
{
	this->M_FCardActor_HP.M_TotalLife = _HP;
	this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_TotalLife;
}

void ACardActor::SetCardCurrentHP(const float& _HP)
{
	this->M_FCardActor_HP.M_Life = _HP;
}

bool ACardActor::GetMapDay() const
{
	return this->M_CardProperty.M_Day;
}

bool ACardActor::GetCardDay() const
{
	return this->M_CardProperty.M_CardDay;
}

void ACardActor::CardDayTransform(bool _Day)
{
	this->M_CardProperty.M_CardDay = _Day;
}

void ACardActor::UpdateCardCollsion()
{
	this->GetUIMapMesh()->CloseAllCardsCollision();

	//查询外层卡
	ACardActor* const* Cur1 = this->GetUIMapMesh()->GetCardDatas().Find(1);
	if (Cur1 && IsValid(*Cur1))
	{
		(*Cur1)->SetCollisionEnable(true);
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(
				UGameSystemFunction::GetObjectName(this) +
				TEXT("开启碰撞")
			));
		}
		return;
	}

	//查询内层卡
	ACardActor* const* Cur0 = this->GetUIMapMesh()->GetCardDatas().Find(0);
	if (Cur0 && IsValid(*Cur0))
	{
		(*Cur0)->SetCollisionEnable(true);
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(
				UGameSystemFunction::GetObjectName(this) +
				TEXT("开启碰撞")
			));
		}
		return;
	}

	//没有角色的情况下
	if (!this->GetUIMapMesh()->GetPlayer())
	{
		//查询底层卡
		ACardActor* const* Cur01 = this->GetUIMapMesh()->GetCardDatas().Find(-1);
		if (Cur01 && IsValid(*Cur01))
		{
			(*Cur01)->SetCollisionEnable(true);
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(
					UGameSystemFunction::GetObjectName(this) +
					TEXT("开启碰撞")
				));
			}
			return;
		}
	}
}

bool ACardActor::GamePause()
{
	return this->M_AGameMapInstance->GetGamePause();
}

AMouseActor* ACardActor::GetCurrentMouse()
{
	return this->M_CardProperty.M_CurrentMouse;
}

AMouseActor* ACardActor::GetCurrentAttackSelfMouse()
{
	return this->M_CardProperty.M_CurrentAttackSelfMouse;
}

void ACardActor::GetAttackDefnceRate(float& Rate)
{
	Rate = this->M_FCardActor_HP.M_AttackDefenceRate;
}

void ACardActor::SetCurrentMouse(AMouseActor* _MouseActor)
{
	this->M_CardProperty.M_CurrentMouse = _MouseActor;
}

void ACardActor::SetCurrentAttackSelfMouse(AMouseActor* _MouseActor)
{
	this->M_CardProperty.M_CurrentAttackSelfMouse = _MouseActor;
}

void ACardActor::SetRenderLayer(int32 _Layer)
{
	// 木盘子 -1  0  0,1
	// 小笼包 0   1  2,3
	// 西瓜皮 1   2  4,5

	/*int32 CurLayer = _Layer - TranslucencyConst::GetLayer(TranslucencyConst::ETranslucency::EMeshe);
	switch ((CurLayer - 1))
	{
	case -1:
		CurLayer = _Layer;
		; break;
	case 0:
		CurLayer = _Layer + 1;
		; break;
	case 1:
		CurLayer = _Layer + 2;
		; break;
	default:
		CurLayer = _Layer + 3;
		break;
	}*/

	Super::SetRenderLayer(_Layer);
	//this->M_CardGradeStaticMesh->SetTranslucentSortPriority(_Layer + 1);
	if (IsValid(this->CardLevelActor))
	{
		this->CardLevelActor->SetRenderLayer(_Layer + 1);
	}
}

int32 ACardActor::GetCardGrade(const FString& _Name)
{
	if (AGameMapInstance::GetGameMapInstance())
	{
		for (auto Datas : AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->M_CardData)
		{
			if (Datas.ItemName.ToString().Equals(_Name))
			{
				return Datas.M_CardGrade;
			}
		}
	}

	return int32();
}

bool ACardActor::UpdateCardState(const float& _ATK_Value, const float& _UpHP_Value)
{
	if (this->M_FCardActor_HP.M_Life <= 0.f)
	{
		return true;
	}

	if (_UpHP_Value + this->M_FCardActor_HP.M_Life >= this->M_FCardActor_HP.M_TotalLife)
	{
		this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_TotalLife;
	}
	else {
		this->M_FCardActor_HP.M_Life += _UpHP_Value;
	}

	//更新生命值
	this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_Life - (_ATK_Value * this->M_FCardActor_HP.M_AttackDefenceRate);

	if (UFVMGameInstance::GetDebug())
	{
		FString Content = FString(TEXT("【")) + this->GetCardName() + FString(TEXT("】:")) +
			FString::SanitizeFloat(_ATK_Value * this->M_FCardActor_HP.M_AttackDefenceRate) + FString(TEXT("HP:")) +
			FString::SanitizeFloat(this->M_FCardActor_HP.M_Life);

		UGameSystemFunction::FVMLog(__FUNCTION__, Content);
	}

	this->OnCardLifeHpChangeDelegate.ExecuteIfBound((_ATK_Value * this->M_FCardActor_HP.M_AttackDefenceRate), _UpHP_Value);

	if (this->M_FCardActor_HP.M_Life <= 0)
	{
		return true;
	}

	return false;
}

void ACardActor::OnCardMovedUpdate(
	const float& DeltaTime,
	const int32& _Layer,
	const FVector& _Location,
	const FLine& _Line
)
{
	this->SetActorLocation(_Location);
	this->SetRenderLayer(_Layer + this->M_SourceCardDataBase.M_CardLayer + 1);
	this->SetLine(_Line);
}

void ACardActor::OnCardMoveBegin(const int32& _Layer, const FLine& _Line)
{
	this->M_bFloatMode = true;
	this->SetLine(_Line);
	this->SetRenderLayer(_Layer + this->M_SourceCardDataBase.M_CardLayer + 1);
}

void ACardActor::OnCardMoveEnd(const int32& _Layer, const FLine& _Line)
{
	this->M_bFloatMode = false;
	this->SetLine(_Line);
	this->SetRenderLayer(_Layer + this->M_SourceCardDataBase.M_CardLayer + 1);
}

void ACardActor::BeginPlay()
{
	Super::BeginPlay();

	this->M_AGameMapInstance = AGameMapInstance::GetGameMapInstance();

	this->GameBuff = UGameBuff::MakeGameBuff(this, EGameBuffCharTag::Card);

	if (!this->M_AGameMapInstance)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error, TEXT("[ACardActor::BeginPlay]:游戏地图实例对象获取失败"));
		}

		this->SetActorTickEnabled(false);

		return;
	}


	//设置卡片碰撞类型
	this->M_CardTypeBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);

	//设置卡片碰撞类型
	this->bCollisionActive = true;
	this->bCollisionState = true;

	this->M_CardTypeBoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(
		UGameSystemFunction::GetCardCollisionBoxType(
			this->GetCardData().M_ECardCollisionType), ECollisionResponse::ECR_Block);
	this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(
		UGameSystemFunction::GetCardCollisionBoxType(ECardCollisionType::E_CardActor), ECollisionResponse::ECR_Block);
	this->M_CardTypeBoxCollision->SetBoxExtent(FVector(this->GetCardData().M_CardCollsionWidth));
	this->M_CardTypeBoxCollision->SetRelativeLocation(FVector::ZeroVector);

	if (UFVMGameInstance::GetDebug())
	{
		this->M_CardTypeBoxCollision->SetHiddenInGame(false);
	}

	this->SetActorRotation(FRotator(0.f, 90.f, 0.f));

	//获取白天和夜晚
	this->M_CardProperty.M_Day = this->M_AGameMapInstance->M_ResourceManagerComponent->GetDay();

	//如果选择关闭碰撞
	if (!this->GetCardData().M_bEnableCardCollision)
	{
		//获取卡片的碰撞box
		for (auto& Box : this->GetComponents())
		{
			//如果检测到盒子碰撞（设置射线检测）
			if (UBoxComponent* BoxComp = Cast<UBoxComponent>(Box))
			{
				//设置卡片碰撞类型
				this->bCollisionActive = false;
				this->bCollisionState = false;
				BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
				break;
			}
		}
	}

	//卡片激活(白天卡)
	if (this->GetCardDay() && this->GetMapDay() || this->GetCardDay() && !this->GetMapDay())
	{
		this->CardActive();
	}
	else if (!this->GetCardDay() && !this->GetMapDay())//(夜间卡)
	{
		this->CardActive();
	}
}

void ACardActor::KillCard()
{
	if (IsValid(this->M_UUI_MapMeshe))
	{
		//卡片死亡，网格调用死亡函数
		if (this->M_UUI_MapMeshe->EradicateCard(this->M_CardProperty.M_CardID))
		{
			//网格必须存在
			if (this->M_UUI_MapMeshe->GetMapMeshe())
			{
				this->M_UUI_MapMeshe->GetMapMeshe()->RemoveCard(this);
				this->UpdateCardCollsion();

				if (IsValid(this->CardLevelActor))
				{
					this->CardLevelActor->Destroy();
				}

				this->Destroy();
				return;
			}
		}
	}

	//this->GetRenderComponent()->OnDynamicAnimationEndDelegate.Clear();
	//如果所有条件都不满足，直接移除

	if (IsValid(this->CardLevelActor))
	{
		this->CardLevelActor->Destroy();
	}
	this->Destroy();
}

void ACardActor::SetUIMapMesh(UUI_MapMeshe* _MapMesh)
{
	this->M_UUI_MapMeshe = _MapMesh;
}

UUI_MapMeshe* ACardActor::GetUIMapMesh()
{
	return this->M_UUI_MapMeshe;
}

void ACardActor::ParseBuff_Information(const FGameBuffInfor& _Buff, UObject* CurObject)
{
	//添加buff
	for (const auto& Cur : _Buff.CurBuffs)
	{
		const TSubclassOf<UBuffDynamicProperty>* Sub = _Buff.CurBuffPropertys.Find(Cur.Key);
		if (Sub && *Sub)
		{
			UBuffDynamicProperty* Pro = UDynamicProperty::MakeDynamicPropertyByClass(*Sub);
			Pro->SetDefObject(CurObject);
			this->GameBuff->AddBuff(Cur.Key, Cur.Value, Pro);
		}
		else {
			this->GameBuff->AddBuff(Cur.Key, Cur.Value, nullptr);
		}
	}
}

void ACardActor::AnalysisActor(AActor* _Actor)
{
	if (Cast<AMouseActor>(_Actor) && IsValid(_Actor) && Cast<AMouseActor>(_Actor)->GetCurrentHP() > 0.f)
	{
		this->SetCurrentMouse(Cast<AMouseActor>(_Actor));
	}
}

void ACardActor::UpdateCardEnableState()
{
	//卡片休息->颜色设置（卡片夜晚并且地图白天）
	if (!this->GetCardDay() && this->GetMapDay())
	{
		//如果没有设置则设置卡片颜色
		if (!this->M_bColorSet)
		{
			this->SetSpineRenderColor(SPINE_COLOR_DARK);
			this->M_bColorSet = true;
		}
	}
	else {
		if (this->M_bColorSet)
		{
			this->SetSpineRenderColor(SPINE_COLOR_WHITE);
			this->M_bColorSet = false;
			this->CardActive();
		}
	}
}

void ACardActor::CardActive()
{
	//调用代理
	if (this->OnCardActiveDelegate.IsBind())
	{
		this->OnCardActiveDelegate.Call();
	}
}

void ACardActor::CardBeHurt(AMouseActor* CurMouseActor)
{
	//调用代理
	this->OnCardBeHurtDelegate.ExecuteIfBound(CurMouseActor);
}

void ACardActor::CardDeath()
{
	//调用代理
	if (this->OnCardDeathDelegate.IsBind())
	{
		this->OnCardDeathDelegate.Call();
	}

	if (IsValid(this->CardLevelActor))
	{
		this->CardLevelActor->Destroy();
	}
}

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = true;

	this->CurCheckMesheLine = FLine(-1);

	/*this->M_CardGradeStaticMesh = this->CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("CardGradeMeshComponent")
	);
	this->M_CardGradeStaticMesh->SetupAttachment(this->GetPointComponent());
	this->M_CardGradeStaticMesh->SetWorldRotation(FRotator(0.f, 0.f, 90.f));*/

	this->M_CardTypeBoxCollision = this->CreateDefaultSubobject<UBoxComponent>(TEXT("CardTypeBox"));
	this->M_CardTypeBoxCollision->SetupAttachment(this->GetPointComponent());

	this->CurCardDataComponent = this->CreateDefaultSubobject<UCardDataComponent>(
		TEXT("CurCardDataComponent")
	);

	//新增卡片等级
	this->CardLevelLocationComp = this->CreateDefaultSubobject<USceneComponent>(
		TEXT("CardLevelLocationCompnent")
	);
	this->CardLevelLocationComp->SetupAttachment(
		this->GetPointComponent()
	);
}

void ACardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//重新设置更新速度
	DeltaSeconds = DeltaSeconds * this->GameBuff->GetTickRate();

	if (this->GameBuff->GetBuffExist())
	{
		this->GameBuff->UpdateBuff(DeltaSeconds);
	}

	if (this->DelayTime > 0.f)
	{
		this->DelayTime -= DeltaSeconds;
	}
	else {
		this->DelayTime = 0.2f;

		if (this->GamePause())
		{
			return;
		}

		//更新卡片激活状态
		this->UpdateCardEnableState();
	}
}

UCardDataComponent* ACardActor::GetCardDataComponent()
{
	return this->CurCardDataComponent;
}

UBoxComponent* ACardActor::GetBoxComponent()
{
	return this->M_CardTypeBoxCollision;
}

void ACardActor::SetAttackDefnceRate(float Rate)
{
	this->M_FCardActor_HP.M_AttackDefenceRate = Rate;
}

void ACardActor::SetCardCollisonState(bool bActive)
{
	if (bActive)
	{
		if (!this->bCollisionActive)
		{
			this->bCollisionActive = true;
			this->bCollisionState = true;

			this->M_CardTypeBoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
			this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

			this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(
				UGameSystemFunction::GetCardCollisionBoxType(this->GetCardData().M_ECardCollisionType),
				ECollisionResponse::ECR_Block);

			this->M_CardTypeBoxCollision->SetCollisionResponseToChannel(
				UGameSystemFunction::GetCardCollisionBoxType(ECardCollisionType::E_CardActor),
				ECollisionResponse::ECR_Block);

			this->M_CardTypeBoxCollision->SetBoxExtent(FVector(this->GetCardData().M_CardCollsionWidth));
			this->M_CardTypeBoxCollision->SetRelativeLocation(FVector::ZeroVector);
		}
	}
	else {
		if (this->bCollisionActive)
		{
			this->bCollisionActive = false;
			this->bCollisionState = false;

			//设置卡片碰撞类型
			this->M_CardTypeBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		}
	}
}

void ACardActor::SetCollisionEnable(bool bValue)
{
	//如果是开启碰撞的卡片
	if (this->M_SourceCardDataBase.M_bEnableCardCollision)
	{
		if (bValue)
		{
			if (!this->bCollisionState)
			{
				this->bCollisionState = true;
				this->M_CardTypeBoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		}
		else {
			if (this->bCollisionState)
			{
				this->bCollisionState = false;
				this->M_CardTypeBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}
