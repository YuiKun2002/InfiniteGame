// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Card/CardRangeATKComponent.h"

#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Components/BoxComponent.h>

#include "GameSystem/Tools/GameSystemFunction.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

UCardRangeATKComponent::UCardRangeATKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCardRangeATKComponent::BeginPlay()
{
	Super::BeginPlay();

	this->RangeATKCardActor = Cast<ARangeATKCardActor>(this->GetOwner());

	if (!this->RangeATKCardActor)
	{
		UE_LOG(LogTemp, Error, TEXT("获取卡片ARangeATKCardActor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}
}

void UCardRangeATKComponent::Spawn()
{
	//Super::Spawn();


	TArray<ETraceTypeQuery> CurAttackType;
	switch (this->RangeATKCardActor->GetRangeATKCardData().M_ELineTraceType)
	{
	case ELineTraceType::E_MouseGround:CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery4); break;
	case ELineTraceType::E_MouseSky:CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery5); break;
	case ELineTraceType::E_MouseUnder:CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery6); break;
	case ELineTraceType::E_MouseGroundAndSky:
	{
		CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery4);
		CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery5);
	}break;
	default:CurAttackType.Emplace(UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType::MouseActor)); break;
	}

	for (const ETraceTypeQuery& Type : CurAttackType)
	{
		bool BResult = false;
		TArray<FHitResult> Hits;
		if (UGameSystemFunction::AddSphereTraceMulti(this->RangeATKCardActor,
			Hits,
			FVector::ZeroVector,
			FVector::ZeroVector,
			this->RangeATKCardActor->GetRangeATKCardData().M_ATKRadius,
			Type)
			)
		{
			FItem_Buff Buff = this->RangeATKCardActor->GetRangeATKCardData().M_FItem_Buff;

			for (auto& Mouse : Hits)
			{
				if (AMouseActor* LMouse = Cast<AMouseActor>(Mouse.GetActor()))
				{
					if (IsValid(LMouse) && LMouse->GetCurrentHP() > 0.f)
					{
						UGameSystemFunction::HitMouse(this->RangeATKCardActor,
							this->RangeATKCardActor->GetCurrentATK(),
							LMouse,
							Buff,
							this->RangeATKCardActor->GetRangeATKCardData().AttackType
						);
					}
				}
			}
		}
	}

	//是否开启了矩阵
	if (this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_bEnable)
	{
		UGameSystemFunction::CreateMapMatrix(this->RangeATKCardActor->GetLine(),
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_RowCount,
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_ColCount,
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_Offset,
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Matrix.M_AnimPath
		);
	}

}

void UCardRangeATKComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	this->RangeATKCardActor->SetPlayAnimation(UGameSystemFunction::LoadRes(this->RangeATKCardActor->CardActor_AttackAnim));

	if (this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Audio.M_bEnable)
	{
		//播放音效
		UFVMGameInstance::PlayBGM_S_Static(
			this->RangeATKCardActor->GetRangeATKCardData().M_FCardRangeATK_Audio.M_AudioName,
			TEXT("ItemAudio"));
	}
}

void UCardRangeATKComponent::PlayIdleAnimation()
{
	this->RangeATKCardActor->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->RangeATKCardActor->CardActor_DefAnim)
	);
}

void UCardRangeATKComponent::InitCondition()
{
	Super::InitCondition();
}

void UCardRangeATKComponent::LoadResource()
{
	Super::LoadResource();

	//初始化攻击模式
	if (this->RangeATKCardActor->GetRangeATKCardData().M_AutoAttack)
	{
		this->SetAttackModEnabled(true);
	}

	//设置存活时间
	this->M_CurrentTime = this->RangeATKCardActor->GetRangeATKCardData().M_CardLifeTime;

	//初始化发射器
	this->InitLaunchProperty(
		this->RangeATKCardActor->GetCurrentAttackCount(),
		this->RangeATKCardActor->GetCurrentAttackSpeed(),
		this->RangeATKCardActor->GetCurrentFristAttackDelay(),
		this->RangeATKCardActor->GetCurrentSecondAttackDelay()
	);

	//播放默认动画
	this->RangeATKCardActor->SetPlayAnimation(
		UGameSystemFunction::LoadRes(this->RangeATKCardActor->CardActor_DefAnim)
	);

	//打印速度Log
	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("%.2f  %.2f  %.2f"),
			this->RangeATKCardActor->GetCurrentAttackSpeed(),
			this->RangeATKCardActor->GetCurrentFristAttackDelay(),
			this->RangeATKCardActor->GetCurrentSecondAttackDelay());
	}
}

bool UCardRangeATKComponent::CheckMouse(FHitResult& OutResult)
{
	TArray<ETraceTypeQuery> CurAttackType;
	switch (this->RangeATKCardActor->GetRangeATKCardData().M_ELineTraceType)
	{
	case ELineTraceType::E_MouseGround:CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery4); break;
	case ELineTraceType::E_MouseSky:CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery5); break;
	case ELineTraceType::E_MouseUnder:CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery6); break;
	case ELineTraceType::E_MouseGroundAndSky:
	{
		CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery4);
		CurAttackType.Emplace(ETraceTypeQuery::TraceTypeQuery5);
	}break;
	default:CurAttackType.Emplace(UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType::MouseActor)); break;
	}

	for (const ETraceTypeQuery& Type : CurAttackType)
	{
		bool BResult = false;
		if (UGameSystemFunction::AddSphereTrace(this->RangeATKCardActor,
			OutResult,
			FVector::ZeroVector,
			FVector::ZeroVector,
			this->RangeATKCardActor->GetRangeATKCardData().M_ATKRadius,
			Type)
			)
		{

			//设置攻击模式开
			this->SetAttackModEnabled(true);

			return true;
		}
	}

	return false;
}

void UCardRangeATKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FVM_VS_GamePause())
		return;

	if (!this->RangeATKCardActor)
		return;

	//卡片休息
	if (!this->RangeATKCardActor->GetCardDay() && this->RangeATKCardActor->GetMapDay())
	{
		this->SetAttackModEnabled(false);
		return;
	}

	//如果是自动攻击
	if (this->RangeATKCardActor->GetRangeATKCardData().M_AutoAttack)
	{
		this->SetAttackModEnabled(true);
	}
	else {
		if (this->CheckMouseDelay > 0.f)
		{
			this->CheckMouseDelay -= DeltaTime;
		}
		else {
			this->CheckMouseDelay = 0.5f;

			FHitResult Hit;
			if (!this->CheckMouse(Hit))
			{
				this->SetAttackModEnabled(false);
			}
		}
	}

	//有时间限制
	if (this->RangeATKCardActor->GetRangeATKCardData().M_bEnabledTime)
	{
		this->M_CurrentTime -= DeltaTime;
		if (M_CurrentTime <= 0.f)
		{
			this->SetComponentTickEnabled(false);
			this->RangeATKCardActor->KillCard();
		}
	}
}
