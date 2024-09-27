// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Card/CardAttackComponent.h"

#include "GameStart/Flipbook/GameActor/CardActor/AttackCardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "GameSystem/GameDataSubsystem.h"
#include "SpineSkeletonAnimationComponent.h"

void UCardAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化攻击卡片
	this->AttackCardActor = Cast<AAttackCardActor>(this->GetOwner());

	if (!this->AttackCardActor)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(TEXT("UCardAttackComponent"), TEXT("攻击卡片初始化失败"));
			this->SetComponentTickEnabled(false);
		}
	}
}

void UCardAttackComponent::SpawnBullet(AFlyItemActor* NewBullet)
{
	//获取对象的变换位置
	FTransform NewTrans;
	NewTrans.SetLocation(this->AttackCardActor->GetBulletLauncherLocation());
	NewTrans.SetLocation(
		FVector(
			550.f,
			NewTrans.GetLocation().Y,
			NewTrans.GetLocation().Z
		)
	);

	//新生成的对象设置自定义拥有者(CardActor)
	NewBullet->SetLine(this->AttackCardActor->GetLine().Row);
	NewBullet->SetActorTransform(NewTrans);
	NewBullet->SetObjectActorLocation(this->AttackCardActor->GetCurrentMouse());
	NewBullet->SetATK(this->AttackCardActor->GetCurrentATK());
	NewBullet->SetSecondATK(
		this->AttackCardActor->GetCurrentSecondATK(
			this->AttackCardActor->GetATKCardData().M_SputteringATKRate)
	);
	NewBullet->SetFloatModeEnable(this->AttackCardActor->GetFloatMode());
	NewBullet->Init();
	
}

void UCardAttackComponent::LoadResource()
{
	Super::LoadResource();

	//初始化条件
	this->InitLaunchProperty(
		this->AttackCardActor->GetCurrentAttackCount(),
		this->AttackCardActor->GetCurrentAttackSpeed(),
		this->AttackCardActor->GetCurrentFristAttackDelay(),
		this->AttackCardActor->GetCurrentSecondAttackDelay()
	);	

	//初始化默认子弹
	this->InitLaunchBulletByDef(this->AttackCardActor->CardActor_BulletClassObj);

	//播放发呆动画
	this->AttackCardActor->SetAnimation(0, this->GetIdleAnimName(), true);
	this->SetTrackEntry(nullptr);
}

void UCardAttackComponent::OnAnimationComplete(class UTrackEntry* Track)
{
	this->OnAnimationPlayEnd();
	this->SetTrackEntry(nullptr);
}

void UCardAttackComponent::ReInitDefIdleAnimName(TSubclassOf<class UAssetCategoryName> IdleName)
{
	Super::ReInitDefIdleAnimName(IdleName);

	this->AttackCardActor->SetAnimation(0, this->GetIdleAnimName(), true);
}

void UCardAttackComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	//播放动画
	UTrackEntry* Track = this->AttackCardActor->SetAnimation(0, this->GetAttackAnimName(), true);
	//绑定动画事件
	Track->AnimationComplete.AddDynamic(
		this, &UCardAttackComponent::OnAnimationComplete);
	this->SetTrackEntry(Track);
}

void UCardAttackComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->AttackCardActor->SetAnimation(0, this->GetIdleAnimName(), true);
}

void UCardAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (time < 0.3f)
	{
		this->time += DeltaTime;
	}
	else {
		//老鼠检测
		if (FVM_VS_GamePause())
		{
			return;
		}

		//卡片休息
		if (!this->AttackCardActor->GetCardDay() && this->AttackCardActor->GetMapDay())
		{
			return;
		}

		this->time = 0;

		//计算浮动地板的检测线条
		if (this->AttackCardActor->GetFloatMode())
		{
			this->AttackCardActor->CalculateLineTracePosition(this->AttackCardActor->GetLineTracePosition());
		}

		//当前检测到的老鼠线路
		ELineTraceType CurCheckLineType;
		//当前老鼠
		AActor* CurMouse = UGameSystemFunction::CreateCheckMouseLineTrace(
			this->GetWorld(),
			this->AttackCardActor->GetLineTraceSetting(), CurCheckLineType
		);

		//老鼠有效则设置攻击模式
		if (Cast<AMouseActor>(CurMouse) && IsValid(CurMouse))
		{
			this->AttackCardActor->AnalysisActor(CurMouse);
			this->SetAttackModEnabled(true);
		}
		else
		{
			this->AttackCardActor->SetCurrentMouse(nullptr);
			this->SetAttackModEnabled(false);
		}
	}
}