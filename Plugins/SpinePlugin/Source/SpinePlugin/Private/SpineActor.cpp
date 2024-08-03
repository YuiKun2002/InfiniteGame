// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "SpineActor.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"

// Sets default values
ASpineActor::ASpineActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComp = this->CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	this->SetRootComponent(this->RootComp);

	this->PointComp = this->CreateDefaultSubobject<USceneComponent>(TEXT("PointComp"));
	this->PointComp->SetupAttachment(this->RootComp, FName(TEXT("PointSocket")));

	this->AnimRenderComp = this->CreateDefaultSubobject<USpineSkeletonRendererComponent>(TEXT("AnimRenderComp"));
	this->AnimRenderComp->SetupAttachment(this->PointComp, FName(TEXT("AnimRenderSocket")));

	this->AnimComp = this->CreateDefaultSubobject<USpineSkeletonAnimationComponent>(TEXT("AnimComp"));
}

USpineSkeletonRendererComponent* ASpineActor::GetRenderComponent()
{
	return this->AnimRenderComp;
}

USpineSkeletonAnimationComponent* ASpineActor::GetAnimationComponent()
{
	return this->AnimComp;
}

