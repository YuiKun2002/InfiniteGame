// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Card/CardDoubleAttackComponent.h"

#include "GameStart/Flipbook/GameActor/Mouse/Normal/FlyStateMouse.h"
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

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"

void UCardDoubleAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	this->MouseComp = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;
}

void UCardDoubleAttackComponent::LoadResource()
{
	Super::LoadResource();

	this->LineComp = this->MouseComp->GetMouseLineManager(this->AttackCardActor->GetLine().Row);
}

void UCardDoubleAttackComponent::Spawn()
{
	if (IsValid(this->CurSky))
	{
		//当前老鼠无效
		if (!IsValid(this->AttackCardActor->GetCurrentMouse()))
		{
			//手动置空
			this->AttackCardActor->SetCurrentMouse(nullptr);
			this->SetAttackModEnabled(false);
			return;
		}

		if (this->bSFirst)
		{
			//设置随机数
			this->SetRandom();
			//随机子弹
			this->LauncherItem(this->SOtherItems, this->CurSFinishItems, this->STargetCardOtherItem);
		}
		else {
			//首次发射完毕，下一次发射将进行初始化随机数
			this->bSFirst = true;
		}

		if (IsValid(this->STargetCardOtherItem.GetRes()))
		{
			if (!IsValid(this->SPool[this->STargetCardOtherItem.GetIndex()]) || this->SPool.Num() < 1)
			{
				return;
			}

			//获取对象的变换位置
			const FTransform& _trans = this->AttackCardActor->GetActorTransform();
			FTransform NewTrans;
			NewTrans.SetLocation(_trans.GetLocation());

			float CurATK = this->AttackCardActor->GetCurrentATK();
			if (Cast<AFlyStateMouse>(this->CurSky))
			{
				CurATK = this->CurSky->GetTotalHP() * 0.21f;
			}

			//获取对象池的对象
			//生成子弹
			AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(
				this->SPool[this->STargetCardOtherItem.GetIndex()]->GetObjectActor()
			);

			//新生成的对象设置自定义拥有者(CardActor)
			_TargetActor->SetLine(this->AttackCardActor->GetLine().Row);
			_TargetActor->SetActorTransform(NewTrans);
			_TargetActor->AddActorLocalOffset(_TargetActor->M_OffsetPosition);
			_TargetActor->M_CustomActorOwner = this->AttackCardActor;
			_TargetActor->SetMouseActorLocation(this->AttackCardActor->GetCurrentMouse());
			_TargetActor->SetATK(CurATK);
			_TargetActor->SetSecondATK(
				this->AttackCardActor->GetCurrentSecondATK(
					this->AttackCardActor->GetATKCardData().M_SputteringATKRate)
			);
			_TargetActor->SetFloatModeEnable(this->AttackCardActor->GetFloatMode());
			_TargetActor->Init();
			_TargetActor->OnInit();

			AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent;
		}
	}
	else {
		Super::Spawn();
	}
}

void UCardDoubleAttackComponent::PlayAttackAnimation()
{
	if (IsValid(this->CurSky))
	{
		//初始化首次发射
		this->bSFirst = false;
		//初始化
		this->InitLaunch(this->bSInitItemSort, this->SOtherItems, this->CurSFinishItems);
		//设置随机数
		this->SetRandom();
		//初始化数据
		this->LauncherItem(this->SOtherItems, this->CurSFinishItems, this->STargetCardOtherItem);

		//this->AttackCardActor->SetPlayAnimation(this->STargetCardOtherItem.GetAnim());

		this->AttackCardActor->SetAnimation(0, SpineCardAnimationState_Attack, true);

	}
	else {
		Super::PlayAttackAnimation();
	}
}

void UCardDoubleAttackComponent::BeginDestroy()
{
	Super::BeginDestroy();

	for (auto& Cur : this->SPool)
	{
		if (IsValid(Cur))
		{
			Cur->ClearAllObjectActor();
			Cur = nullptr;
		}
	}

	this->SPool.Empty();
}

void UCardDoubleAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	this->CurSky = this->LineComp->GetMouseTopBySky();
	if (IsValid(this->CurSky))
	{
		//if (this->CurSky->GetActorLocation().Y > this->AttackCardActor->GetActorLocation().Y)
		//{
		this->TickCondition(DeltaTime);

		this->AttackCardActor->AnalysisActor(this->CurSky);
		this->SetAttackModEnabled(true);
		//}
	}
	else {
		//陆地攻击
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	}
}

void UCardDoubleAttackComponent::AddLaunchRadomItemSecond(
	int32 RandomValue,
	TSoftClassPtr<AFlyItemActor> Res,
	TSoftClassPtr<UAssetCategoryName> AnimName
)
{
	if (RandomValue <= 0)
	{
		RandomValue = 1;
	}
	else if (RandomValue > 100)
	{
		RandomValue = 0;
	}

	UClass* NameClass = AnimName.LoadSynchronous();
	TSubclassOf<UAssetCategoryName> NameObjectResource(NameClass);
	if (IsValid(NameObjectResource.GetDefaultObject()))
	{
		//添加新的攻击方式
		this->SPool.Emplace(UObjectPoolManager::MakePoolManager(this->GetWorld(), Res, 1));
		this->SOtherItems.Emplace(FCardOtherItem(
			this->SPool.Num() - 1,
			RandomValue,
			Res,
			NameObjectResource.GetDefaultObject()->GetCategoryName().ToString()
		));
	}
}
