// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/CustomMapMeshe.h"
#include "GameStart/VS/UI/UI_CustomMapMeshe.h"
#include <Components/WidgetComponent.h>

// Sets default values
ACustomMapMeshe::ACustomMapMeshe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomMapMeshe::BeginPlay()
{
	Super::BeginPlay();

	this->bSetLine = false;
	this->CurLine = FLine(-1,-1);
}

// Called every frame
void ACustomMapMeshe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bInit)
	{

	}
	else {
		this->AddActorLocalOffset(FVector(-80.f, 0.f, 0.f));
		this->SetActorTickEnabled(false);
		this->bInit = true;
	}
}

UWidgetComponent* ACustomMapMeshe::GetWidget()
{
	UWidgetComponent* Cur =	Cast<UWidgetComponent>(this->GetComponentByClass(UWidgetComponent::StaticClass()));
	if (IsValid(Cur))
	{
		Cast<UUI_CustomMapMeshe>(Cur->GetWidget())->CurMeshe = this;
	}
	return Cur;
}

void ACustomMapMeshe::SetLine(FLine NewLine)
{
	this->bSetLine = true;
	this->CurLine = NewLine;
}

bool ACustomMapMeshe::GetLine(FLine& OutLine)
{
	OutLine = CurLine;
	return this->bSetLine;
}

