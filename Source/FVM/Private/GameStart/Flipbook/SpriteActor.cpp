// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/SpriteActor.h"

// Sets default values
ASpriteActor::ASpriteActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->M_Position = CreateDefaultSubobject<USceneComponent>(TEXT("MySpriteActor_Pos"));
	this->M_SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MySpriteActor_SpriteComp"));

	this->SetRootComponent(this->M_Position);
	this->M_SpriteComp->SetupAttachment(this->M_Position);
}

void ASpriteActor::SetSprite(UPaperSprite* _NewSprite)
{
	this->M_SpriteComp->SetSprite(_NewSprite);
}

void ASpriteActor::SetSpritePitchRotation(float _Angle)
{
	this->M_SpriteComp->SetRelativeRotation(FRotator(_Angle, this->M_SpriteComp->GetRelativeRotation().Yaw, this->M_SpriteComp->GetRelativeRotation().Roll));
}

void ASpriteActor::SetSpriteOffset(const FVector& _Offset)
{
	this->M_Offset = _Offset;
}

void ASpriteActor::SetSpriteLocation(const FVector& _Location, bool _bAddOffset)
{
	if (_bAddOffset)
		this->SetActorLocation(_Location + this->M_Offset);
	else
		this->SetActorLocation(_Location);
}

void ASpriteActor::SetSpriteColorOpacity(float _Alpha)
{
	this->M_SpriteComp->SetSpriteColor(FLinearColor(1.f,1.f,1.f, _Alpha));
}

FVector ASpriteActor::GetSpriteLocation()
{
	return this->GetActorLocation();
}

float ASpriteActor::GetSpriteColorOpacity()
{
	return this->M_SpriteComp->GetSpriteColor().A;
}

const int32 ASpriteActor::GetTranslucency() const
{
	return this->Translucency;
}

void ASpriteActor::SetTranslucency(const int32& _Translucency)
{
	this->M_SpriteComp->SetTranslucentSortPriority(_Translucency);
	this->Translucency = _Translucency;
}

const FVector& ASpriteActor::GetSpriteOffset()
{
	return this->M_Offset;
}

// Called when the game starts or when spawned
void ASpriteActor::BeginPlay()
{
	Super::BeginPlay();


	this->M_SpriteComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

// Called every frame
void ASpriteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

