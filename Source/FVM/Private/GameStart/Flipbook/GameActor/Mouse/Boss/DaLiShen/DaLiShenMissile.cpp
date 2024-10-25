// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/DaLiShen/DaLiShenMissile.h"

#include <Kismet/KismetMathLibrary.h>

void ADaLiShenMissile::BeginPlay()
{
	Super::BeginPlay();

	this->InitSpineShow();
}

void ADaLiShenMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->M_CurrentTime > 0.f)
	{
		this->M_CurrentTime -= DeltaTime;
	}
	else
	{
		if (!this->M_Result)
		{
			this->M_Result = true;
		}
	}

}

void ADaLiShenMissile::SetTarget(const FVector& _Value)
{
	this->M_Target = _Value;
}

void ADaLiShenMissile::InitLocation()
{
	//垂直对齐目标点
	this->SetActorLocation(FVector(this->M_Target.X, this->M_Target.Y, 700.f));

	this->M_Current = this->GetActorLocation();

	//设置旋转
	this->SetRotation(180.f);
}

void ADaLiShenMissile::UpdatePosition(float value)
{
	this->SetActorLocation(FVector(this->M_Current.X, this->M_Current.Y, UKismetMathLibrary::Lerp(700.f, this->M_Target.Z, value)));
}

bool ADaLiShenMissile::GetResult()
{
	return this->M_Result;
}
