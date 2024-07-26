// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/UI/UI_CustomMapMeshe.h"
#include "UI_MapMeshe_Cloud.generated.h"


 //这个板块只能放置 棉花糖

class UImage;
class ACardActor;
class AMapMeshe;
class UCardManagerComponent;
class UUI_Card;

class AMapMeshe_Cloud;

UCLASS()
class FVM_API UUI_MapMeshe_Cloud : public UUI_CustomMapMeshe
{
	GENERATED_BODY()
private:
	UPROPERTY()
		AMapMeshe_Cloud* M_AMapMeshe_Cloud = nullptr;
public:
	//创建卡片
	virtual	void CreateNewCard(UCardManagerComponent* _CardMComp) override;

	//设置云朵对象
	UFUNCTION(BlueprintCallable)
		void SetMapMesheCloud(AMapMeshe_Cloud* _Meshe);
};
