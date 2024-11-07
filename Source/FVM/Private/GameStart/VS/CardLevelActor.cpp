// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/CardLevelActor.h"

void ACardLevelActor::SetCardLevelLocation(
	const FVector& Location,
	const int32& CardLevel,
	const int32& RenderLayer,
	const FVector& Offset
)
{
	this->InitSpineShow();
	this->SetAnimation(0,FString::FromInt(CardLevel),true);
	this->SetActorLocation(Location + Offset);
}
