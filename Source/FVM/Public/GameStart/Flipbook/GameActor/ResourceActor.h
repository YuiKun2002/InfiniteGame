// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "ResourceActor.generated.h"

/**
 * 资源对象，老鼠掉落物，金币，其他资源等
 */

class UTexture2D;

UCLASS()
class FVM_API AResourceActor : public AGameActorFlipbookBase
{
	GENERATED_BODY()

public:

	//静态工厂
	static AResourceActor* MakeResourceActorCoin(FString ResourceName, int32 index, int32 Count);

	static AResourceActor* MakeResourceActor(FString ResourceName, FSoftObjectPath HeadPath, int32 Count);

public:

	virtual void BeginPlay() override;

private:

	void InitResource(FString ResourceName, FSoftObjectPath HeadPath, int32 Count);

	void InitCoin(FString ResourceName, int32 index, int32 Count);

public:

	//初始化资产
	UFUNCTION(BlueprintImplementableEvent)
		void InitResource(UTexture2D* Resource);

	//初始化货币
	UFUNCTION(BlueprintImplementableEvent)
		void InitCoin(int32 index);
};
