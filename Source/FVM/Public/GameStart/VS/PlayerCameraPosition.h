// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "PlayerCameraPosition.generated.h"

/**
 *
 */
UCLASS()
class FVM_API APlayerCameraPosition : public APaperSpriteActor
{
	GENERATED_BODY()

public:

	APlayerCameraPosition();

public:

	//相机坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* CameraLocation = nullptr;

};
