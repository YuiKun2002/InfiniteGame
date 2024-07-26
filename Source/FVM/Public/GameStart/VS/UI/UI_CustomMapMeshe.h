// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameStart/VS/MapMeshe.h"
#include <Kismet/GameplayStatics.h>
#include "GameStart/VS/UI/UI_Card.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "UI_CustomMapMeshe.generated.h"

/**
 *自定义网格
 */

class UCardManagerComponent;
class ACustomMapMeshe;

UCLASS()
class FVM_API UUI_CustomMapMeshe : public UWidgetBase
{
	GENERATED_BODY()

		friend class ACustomMapMeshe;

public:
	//创建卡片
	UFUNCTION(BlueprintCallable)
		virtual	void CreateNewCard(UCardManagerComponent* _CardMComp);
	//创建卡片蓝图实现
	UFUNCTION(BlueprintNativeEvent)
		void BPCreateNewCard(UCardManagerComponent* _CardMComp);
	//创建卡片
	UFUNCTION(BlueprintCallable)
		ACardActor* CreateCard(UCardManagerComponent* _CardMComp);
	//显示文本
	UFUNCTION(BlueprintCallable)
		void ShowText(FString NewContent);
	//获取自定义网格
	UFUNCTION(BlueprintCallable)
		ACustomMapMeshe* GetCustomMapMeshe();
private:
	//网格
	UPROPERTY()
		ACustomMapMeshe* CurMeshe = nullptr;
};
