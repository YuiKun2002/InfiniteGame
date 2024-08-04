// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/MapLauncher/Launchers/Meshe/MesheLauncherActor.h"

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/GameMapInstance.h"
#include <Kismet/GameplayStatics.h>

AMesheLauncherActor* AMesheLauncherActor::MakeMesheLauncherActor(UObject* WorldContentObject, UClass* Res, AMapMeshe* MapMeshe, const FLine& Line, const FTransform& Trans)
{
	AMesheLauncherActor* NewMesheLauncherActor = Cast<AMesheLauncherActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(WorldContentObject, Res, Trans));

	NewMesheLauncherActor->CurLine = Line;

	NewMesheLauncherActor->CurMapMeshe = MapMeshe;

	NewMesheLauncherActor->SetRenderLayer(7000);

	NewMesheLauncherActor->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));

	UGameplayStatics::FinishSpawningActor(NewMesheLauncherActor, Trans);

	return NewMesheLauncherActor;
}

void AMesheLauncherActor::BeginPlay()
{
	//初始化UI网格
	this->CurUI_MapMeshe = this->CurMapMeshe->GetUIMapMeshe();

	Super::BeginPlay();


}

void AMesheLauncherActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

FLine AMesheLauncherActor::GetLine()
{
	return this->CurLine;
}

int32 AMesheLauncherActor::GetCardNums()
{
	if (IsValid(this->CurUI_MapMeshe))
	{
		return this->CurUI_MapMeshe->GetCardDatas().Num();
	}

	return 0;
}
