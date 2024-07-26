// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/MapMeshe_Cloud.h"
#include "GameStart/Flipbook/SpriteActor.h"

// Sets default values
AMapMeshe_Cloud::AMapMeshe_Cloud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMapMeshe_Cloud::SetCloudTag(ASpriteActor* _Cloud)
{
	this->M_CloudActor = _Cloud;

	this->M_CloudActor->SetActorHiddenInGame(true);
}

void AMapMeshe_Cloud::RemoveCloudTag()
{
	this->M_CloudActor->SetActorHiddenInGame(false);
}

// Called when the game starts or when spawned
void AMapMeshe_Cloud::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMapMeshe_Cloud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//同步坐标
	if (IsValid(this->M_CloudActor))
	{
		this->SetActorLocation(this->M_CloudActor->GetSpriteLocation() + FVector(-15.f, 0.f, 0.f));

		if (this->M_CloudActor->GetSpriteLocation().Y < -90.f)
		{
			this->RemoveCloudTag();
			this->Destroy();
		}
	}
}

