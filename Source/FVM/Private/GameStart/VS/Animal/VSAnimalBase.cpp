// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Animal/VSAnimalBase.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "GameStart/Flipbook/GameActor/Mouse/Custom/MouseCustomBase.h"
#include <Components/BoxComponent.h>

AVSAnimalBase::AVSAnimalBase()
{
	this->BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("AnimBoxComponent"));
	this->BoxComponent->SetupAttachment(this->GetRenderComponent());
}

void AVSAnimalBase::BeginPlay()
{
	Super::BeginPlay();

	this->BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVSAnimalBase::Begin);
	this->BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AVSAnimalBase::End);
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AVSAnimalBase::AnimationPlayEnd);
}

void AVSAnimalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (AGameMapInstance::GetVSGameOver())
	//{
		//this->SetActorTickEnabled(false);
		//this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
		//this->Destroy();
	//}

	if (FVM_VS_GamePause())
	{
		return;
	}

	if (this->bWalk)
	{
		if (this->GetActorLocation().Y > 1000.f)
		{
			this->Destroy();

			return;
		}

		//如果最大距离超过xxxx自动销毁
		this->AddActorLocalOffset(FVector(0.f, 100.f * DeltaTime, 0.f));
	}
}

void AVSAnimalBase::AnimationPlayEnd()
{
	//当准备Ready时
	if (!this->bWalk && this->bReady)
	{
		this->OnAnimationPlayEnd();
	}
}

void AVSAnimalBase::OnMouseEnter(AMouseActor* MouseActor)
{
	if (AGameMapInstance::GetVSGameOver())
	{
		this->SetActorTickEnabled(false);
		this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
		this->Destroy();
		return;
	}

	this->OnOverlapWidgtMouse(MouseActor);

	//准备
	if (!this->bReady)
	{
		this->bReady = true;

		this->OnReady();
	}
}

void AVSAnimalBase::EnableWalk()
{
	if (!this->bWalk && this->bReady)
	{
		this->bWalk = true;
	}
}

void AVSAnimalBase::SetRow(const int32& Row)
{
	this->CurRow = Row;
}

void AVSAnimalBase::Begin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//当是老鼠时
	if (Cast<AMouseActor>(OtherActor))
	{
		//当boss转换失败时
		if (
			Cast<AMouseActor>(OtherActor)->GetMouseBaseType() != EMouseBaseType::Boss
			&&
			Cast<AMouseActor>(OtherActor)->GetMouseBaseType() != EMouseBaseType::Other
			)
		{
			if (Cast<AMouseActor>(OtherActor)->GetMouseLine().Row == this->CurRow)
			{
				this->OnMouseEnter(Cast<AMouseActor>(OtherActor));
			}
		}
	}

	if (Cast<AMapMeshe>(OtherActor))
	{
		AMapMeshe* CurMeshe = Cast<AMapMeshe>(OtherActor);
		//同行
		if (CurMeshe->GetLine().Row == this->CurRow)
		{
			switch (CurMeshe->GetLineType())
			{
			case ELineType::OnWater:
			{
				this->OnInWater();
			}
			break;
			default:
			{
				this->OnGround();
			}
			break;
			}
		}
	}
}

void AVSAnimalBase::End(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
