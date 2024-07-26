// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/CustomMapMeshe.h"
#include "MapMeshe_Cloud.generated.h"

class ASpriteActor;

UCLASS()
class FVM_API AMapMeshe_Cloud : public ACustomMapMeshe
{
	GENERATED_BODY()
private:
	//被标记的云朵
	UPROPERTY()
		ASpriteActor* M_CloudActor = nullptr;
public:
	// Sets default values for this actor's properties
	AMapMeshe_Cloud();

public:
	//设置云朵标记
	void SetCloudTag(ASpriteActor* _Cloud);
	//移除标记
	void RemoveCloudTag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
