// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameStart/VS/MapBaseType.h"
#include "MapMesheWidgetComponent.generated.h"

class UUI_MapMeshe;
class AMapMeshe;

UCLASS()
class FVM_API UMapMesheWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	//网格类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp | LineType")
		ELineType M_ELineType = ELineType::OnGround;
	//网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp | MapMeshe")
		UUI_MapMeshe* M_UUI_MapMeshe = nullptr;
public:

	UMapMesheWidgetComponent();

	virtual void BeginPlay() override;

	void InitByMesheControllComp(const ELineType& _LineType,const int32& _Translucency);

public:
	//获取排序优先级
	const int32 GetTranslucency() const;
	//设置排序优先级
	void SetTranslucency(const int32& _Translucency);

private:
	//渲染层级
	UPROPERTY()
		int32 M_MesheTranslucency = 0;
};
