// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardDefenceComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include "GameStart/Flipbook/GameActor/Card/DefenceCardActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include <Kismet/GameplayStatics.h>

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"

// Sets default values for this component's properties
UCardDefenceComponent::UCardDefenceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardDefenceComponent::UpdateState()
{
	if (this->DefenceCardActor)
	{
		//获取到卡片生命值
		const float& HPTop = this->DefenceCardActor->GetTotalHP();
		float HP = this->DefenceCardActor->GetCurrentHP();

		if (HP >= HPTop * (0.6f))
		{
			//第一状态
			this->DefenceCardActor->SetAnimation(0, CardDefenceAnimState_Full, true);
		}
		else if (HP >= HPTop * (0.3f) && HP < HPTop * (0.6f))
		{
			//第二状态
			this->DefenceCardActor->SetAnimation(0, CardDefenceAnimState_Damage, true);
		}
		else {
			//第三状态
			this->DefenceCardActor->SetAnimation(0, CardDefenceAnimState_Low, true);
		}

	}
}

void UCardDefenceComponent::LoadResource()
{

}

void UCardDefenceComponent::BeHurt(AMouseActor* CurMouseActor)
{
	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("受到老鼠的攻击"));
	}

	//获取卡片数据
	const FItemCardDefence& CardData = this->DefenceCardActor->GetDefenceCardData();

	//是否开启卡片反伤
	if (CardData.M_bEnableReturnATK)
	{
		//获取当前攻击该卡片的老鼠
		if (IsValid(CurMouseActor) && CurMouseActor->GetCurrentHP() > 0.f)
		{
			FItem_Buff buff;
			if (UGameSystemFunction::HitMouse(this->DefenceCardActor,
				CardData.M_ATK * CardData.M_ATK_UpRate * this->DefenceCardActor->GetTotalHP() * 0.1f / 2.f,
				CurMouseActor, buff, EFlyItemAttackType::Def)
				)
			{
				if (UFVMGameInstance::GetDebug())
				{
					UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("给攻击") +
						CardData.ItemName + TEXT("的老鼠造成反伤：")) +
						FString::SanitizeFloat(CardData.M_ATK *
							CardData.M_ATK_UpRate * this->DefenceCardActor->GetTotalHP() * 0.1f / 2.f)
					);
				}
			}
		}
	}
}

void UCardDefenceComponent::HpChange(float ATK_Value, float UpHP_Value)
{
	//更新状态
	this->UpdateState();
}

void UCardDefenceComponent::Death()
{
	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("防御类型的卡片死亡"));
	}

	//获取卡片数据
	const FItemCardDefence& CardData = this->DefenceCardActor->GetDefenceCardData();

	//是否开启播放音效
	if (CardData.M_FCardRangeATK_Audio.M_bEnable)
	{
		UFVMGameInstance::PlayBGM_S_Static(CardData.M_FCardRangeATK_Audio.M_AudioName, TEXT("ItemAudio"));
	}

	//是否开启动画播放
	if (CardData.M_bEnableDeathBombAnim)
	{
		FTransform Trans = this->DefenceCardActor->GetTransform();
		Trans.SetLocation(Trans.GetLocation() + CardData.M_AnimOffset);

		AGameActorFlipbookBase* Flip = this->DefenceCardActor->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
			LoadClass<AGameActorFlipbookBase>(0, TEXT("Class'/Script/FVM.GameActorFlipbookBase'"))
		);
		Flip->SetRenderLayer(8000);
		Flip->InitRotation();
		Flip->SetActorLocation(Trans.GetLocation());
		Flip->GetMyActor()->SetFlipbook(Cast<UPaperFlipbook>(CardData.M_DeathBombAnimFlipbookPath.TryLoad()));
		Flip->SetAnimationPlayEndDestroy();
	}

	//是否开启了死亡爆炸
	if (CardData.M_bEnableDeathBomb)
	{
		TArray<FHitResult> Hits;

		if (UGameSystemFunction::AddSphereTraceMulti(
			this->DefenceCardActor,
			Hits,
			FVector::ZeroVector,
			FVector::ZeroVector,
			CardData.M_ATKRadius,
			UGameSystemFunction::GetMouseCollisionTraceType(CardData.M_ELineTraceType)
		))
		{
			float HurtValue = this->DefenceCardActor->GetCurrentHP() > 0.f ?
				this->DefenceCardActor->GetCurrentHP() * CardData.M_LifeToATK_Rate :
				this->DefenceCardActor->GetTotalHP() * 0.1f;

			FItem_Buff buff = CardData.M_Buffs;

			for (auto& Mouse : Hits) {
				if (
					UGameSystemFunction::HitMouse(this->DefenceCardActor,
						HurtValue,
						Cast<AMouseActor>(Mouse.GetActor()), buff, EFlyItemAttackType::Bomb)
					)
				{
					if (UFVMGameInstance::GetDebug())
					{
						UGameSystemFunction::FVMLog(__FUNCTION__,
							FString(TEXT("给攻击") +
								CardData.ItemName +
								TEXT("的老鼠造成反伤：")) +
							FString::SanitizeFloat(HurtValue)
						);
					}
				}
			}
		}
	}
}


// Called when the game starts
void UCardDefenceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->DefenceCardActor = Cast<ADefenceCardActor>(this->GetOwner());

	if (!this->DefenceCardActor)
	{
		//UE_LOG(LogTemp, Error, TEXT("获取卡片Actor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}

	//绑定函数代理
	this->DefenceCardActor->OnCardBeHurtDelegate.BindUObject(this, &UCardDefenceComponent::BeHurt);
	this->DefenceCardActor->OnCardLifeHpChangeDelegate.BindUObject(this, &UCardDefenceComponent::HpChange);
	this->DefenceCardActor->OnCardDeathDelegate.Bind(this, TEXT("Death"));
}


// Called every frame
void UCardDefenceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	////卡片休息
	//if (!this->DefenceCardActor->GetCardDay() && this->DefenceCardActor->GetMapDay())
	//	return;
}

