// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/UI/Meshe/UIMesheWidthMagma.h"
#include "UIMesheWidthUnderWater.generated.h"

/**
 *
 */

class ACardActor;
class AActor;

UCLASS()
class FVM_API UUIMesheWidthUnderWater : public UUI_MapMeshe
{
	GENERATED_BODY()
public:
	//初始化
	virtual bool Initialize() override;

	virtual bool CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck) override;

	//网格更新
	virtual void TickByAMapMeshe(float DeltaTime) override;

	virtual bool UpdateCardByMesheType(TMap<int32, ACardActor*>& KillCards, int32 Layer, ACardActor* CurCard) override;

	virtual void PlayFinish(ACardActor* NewCard) override;

private:
	//网格更新
	UPROPERTY()
		FMapMeshUpdate M_MapMeshUpdate;
	//气泡
	UPROPERTY()
		AActor* WaterBubbles = nullptr;
};
