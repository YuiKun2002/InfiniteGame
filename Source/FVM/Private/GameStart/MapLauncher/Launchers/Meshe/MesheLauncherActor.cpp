// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/MapLauncher/Launchers/Meshe/MesheLauncherActor.h"

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/GameMapInstance.h"
#include <Kismet/GameplayStatics.h>

AMesheLauncherActor* AMesheLauncherActor::MakeMesheLauncherActor(UObject* WorldContentObject, UClass* Res, AMapMeshe* MapMeshe, const FLine& Line, const FTransform& Trans)
{
	AMesheLauncherActor* NewMesheLauncherActor = Cast<AMesheLauncherActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(WorldContentObject, Res, Trans)
	);

	NewMesheLauncherActor->CurLine = Line;

	NewMesheLauncherActor->CurMapMeshe = MapMeshe;

	NewMesheLauncherActor->CurUI_MapMeshe = MapMeshe->GetUIMapMeshe();

	NewMesheLauncherActor->SetRenderLayer(MapMeshe->GetTranslucency() + 21);

	NewMesheLauncherActor->InitSpineShow();

	UGameplayStatics::FinishSpawningActor(NewMesheLauncherActor, Trans);

	return NewMesheLauncherActor;
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
