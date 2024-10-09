// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/MapMeshe.h"
#include "MesheLauncherActor.generated.h"

/**
 * 网格发射器角色对象
 */
UCLASS()
class FVM_API AMesheLauncherActor : public ASpineActor
{
	GENERATED_BODY()

public:

	//静态工程方法
	static AMesheLauncherActor* MakeMesheLauncherActor(UObject* WorldContentObject, UClass* Res, AMapMeshe* MapMeshe, const FLine& Line, const FTransform& Trans);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Line")
		FLine GetLine();
	//当前当前网格卡片数量
	UFUNCTION(BlueprintCallable, Category = "Card")
		int32 GetCardNums();

private:
	//当前的线路
	UPROPERTY()
		FLine CurLine;
	//网格对象
	UPROPERTY()
		AMapMeshe* CurMapMeshe = nullptr;
	//UI网格对象
	UPROPERTY()
		UUI_MapMeshe* CurUI_MapMeshe = nullptr;
};
