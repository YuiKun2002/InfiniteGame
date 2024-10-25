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

USceneComponent* ASpineActor::GetPointComponent()
{
	return this->PointComp;
}

USpineSkeletonRendererComponent* ASpineActor::GetRenderComponent()
{
	return this->AnimRenderComp;
}

USpineSkeletonAnimationComponent* ASpineActor::GetAnimationComponent()
{
	return this->AnimComp;
}

FString ASpineActor::GetCurrentAnimationName(int32 TrackIndex) const
{
	//获取当前轨道
	UTrackEntry* CurTrack = this->AnimComp->GetCurrent(TrackIndex);
	if (IsValid(CurTrack))
	{
		return CurTrack->getAnimationName();
	}

	return FString();
}

UTrackEntry* ASpineActor::GetCurrentAnimationTrackEntry(int32 TrackIndex) const
{
	return this->AnimComp->GetCurrent(TrackIndex);
}

FRotator ASpineActor::GetRotation() const
{
	return this->PointComp->GetRelativeRotation();
}

FLinearColor ASpineActor::GetSpineRenderColor() const
{
	return this->AnimRenderComp->Color;
}

int32 ASpineActor::GetSpineRenderLayer()
{
	return this->GetRenderLayer();
}

int32 ASpineActor::GetRenderLayer()
{
	return this->mRenderLayer;
}

UTrackEntry* ASpineActor::SetAnimation(int32 TrackIndex, FString AnimationName, bool Loop)
{
	return this->AnimComp->SetAnimation(TrackIndex, AnimationName, Loop);
}

UTrackEntry* ASpineActor::SetAnimation(UTrackEntry*& CurTrack, int32 TrackIndex, const FName& AnimName)
{
	CurTrack = nullptr;
	CurTrack = this->SetAnimation(
		0,
		AnimName.ToString(),
		true
	);

	return CurTrack;
}

UTrackEntry* ASpineActor::AddAnimation(int32 TrackIndex, FString AnimationName, bool Loop, float Delay)
{
	return this->AnimComp->AddAnimation(TrackIndex, AnimationName, Loop, Delay);
}

UTrackEntry* ASpineActor::SetEmptyAnimation(int32 TrackIndex, float MixDuration)
{
	return this->AnimComp->SetEmptyAnimation(TrackIndex, MixDuration);
}

UTrackEntry* ASpineActor::AddEmptyAnimation(int32 TrackIndex, float MixDuration, float Delay)
{
	return this->AnimComp->AddEmptyAnimation(TrackIndex, MixDuration, Delay);
}

void ASpineActor::SetAnimationTimeScale(int32 TrackIndex, float Value)
{
	UTrackEntry* CurTrack = this->AnimComp->GetCurrent(TrackIndex);
	if (IsValid(CurTrack))
	{
		CurTrack->SetTimeScale(Value);
	}
}

void ASpineActor::SetAnimationClear(int32 TrackIndex)
{
	if (TrackIndex == -1)
	{
		this->AnimComp->ClearTracks();
	}
	else {
		this->AnimComp->ClearTrack(TrackIndex);
	}
}

void ASpineActor::SetSpineRenderColor(FLinearColor SpineColor)
{
	this->AnimRenderComp->Color = SpineColor;
}

void ASpineActor::SetSpineRenderLayer(int32 RenderLayer)
{
	SetRenderLayer(RenderLayer);
}

void ASpineActor::SetRenderLayer(int32 RenderLayer)
{
	if (this->mRenderLayer == RenderLayer)
	{
		return;
	}

	this->mRenderLayer = RenderLayer;
	this->AnimRenderComp->SetTranslucentSortPriority(RenderLayer);
}

void ASpineActor::InitSpineShow()
{
	//设置旋转角度
	this->AnimRenderComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	//矫正X与Y的坐标
	if (this->AnimRenderComp->GetRelativeLocation().X == this->AnimRenderComp->GetRelativeLocation().Y)
	{
		return;
	}

	const float& _CurrentX = this->AnimRenderComp->GetRelativeLocation().X;
	const float& _CurrentY = this->AnimRenderComp->GetRelativeLocation().Y;
	const float& _CurrentZ = this->AnimRenderComp->GetRelativeLocation().Z;

	this->AnimRenderComp->SetRelativeLocation(FVector(_CurrentY, _CurrentX, _CurrentZ));
}

void ASpineActor::SetRotation(float Angle)
{
	//设置旋转角度
	FTransform Trans = this->PointComp->GetComponentToWorld();
	FRotationConversionCache a;
	Trans.SetRotation(a.RotatorToQuat(FRotator(0.f, 0.f, Angle)));

	this->PointComp->SetWorldTransform(Trans);
}
