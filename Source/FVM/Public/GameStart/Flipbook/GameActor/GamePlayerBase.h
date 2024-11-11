// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpineActor.h"
#include "GameSystem/PlayerStructManager.h"
#include "GamePlayerBase.generated.h"

class UPaperFlipbook;
class USceneComponent;
class UFlipbookBaseComponent;
class AGameActorFlipbookBase;

UCLASS()
class FVM_API AGamePlayerBase : public ASpineActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AGamePlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//初始化角色基本数据
	void InitPlayerData(FItemHeroBase Data);

	//获取角色数据
	void GetPlayerDataPa(FItemHeroBase& OutData);
private:
	UPROPERTY()
	FItemHeroBase HeroData;
};
