// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/DragCardShowIdleAnim.h"

#include <Components/SceneCaptureComponent2D.h>

#include <Paper2D/Classes/PaperFlipbookComponent.h>
#include <Paper2D/Classes/PaperFlipbook.h>

// Sets default values
ADragCardShowIdleAnim::ADragCardShowIdleAnim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->M_Capture2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp2D"));
	this->M_AnimComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbookComp"));


	this->SetRootComponent(this->M_Capture2D);
	this->M_AnimComponent->SetupAttachment(this->M_Capture2D);
}

void ADragCardShowIdleAnim::SetAnim(UPaperFlipbook* _anim)
{
	this->M_AnimComponent->SetFlipbook(_anim);
}

// Called when the game starts or when spawned
void ADragCardShowIdleAnim::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADragCardShowIdleAnim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

