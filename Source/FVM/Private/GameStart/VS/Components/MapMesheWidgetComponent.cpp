// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/MapMesheWidgetComponent.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include <Blueprint/UserWidget.h>

UMapMesheWidgetComponent::UMapMesheWidgetComponent()
{
}

void UMapMesheWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Error, TEXT("[UMapMesheInteractionComponent::OnComponentCreated]: UUI_MapMeshe 加载失败!"));

	this->bReceiveHardwareInput = true;
}

void UMapMesheWidgetComponent::InitByMesheControllComp(const ELineType& _LineType, const int32& _Translucency)
{
	this->M_ELineType = _LineType;

	//根据类型生成网格
	switch (this->M_ELineType)
	{
	case ELineType::OnGround:
	{
		this->SetWidget(
			CreateWidget<UUI_MapMeshe>(this->GetWorld()->GetFirstPlayerController(),
				LoadClass<UUI_MapMeshe>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/MapMeshe/BPUI_MeshGround.BPUI_MeshGround_C'")
					))
		);
	}
	break;
	case ELineType::Sky:
		break;
	case ELineType::OnWater:
	{
		this->SetWidget(
			CreateWidget<UUI_MapMeshe>(this->GetWorld()->GetFirstPlayerController(),
				LoadClass<UUI_MapMeshe>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/MapMeshe/BPUI_MeshOnWater.BPUI_MeshOnWater_C'")
					))
		);
	}
	break;
	case ELineType::Underwater:
	{
		this->SetWidget(
			CreateWidget<UUI_MapMeshe>(this->GetWorld()->GetFirstPlayerController(),
				LoadClass<UUI_MapMeshe>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/MapMeshe/BPUI_MeshUnderWater.BPUI_MeshUnderWater_C'")
					))
		);
	}
	break;
	case ELineType::Magma:
	{
		this->SetWidget(
			CreateWidget<UUI_MapMeshe>(this->GetWorld()->GetFirstPlayerController(),
				LoadClass<UUI_MapMeshe>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/MapMeshe/BPUI_MeshMagma.BPUI_MeshMagma_C'")
					))
		);
	}
	break;
	case ELineType::All:
	{
		this->SetWidget(
			CreateWidget<UUI_MapMeshe>(this->GetWorld()->GetFirstPlayerController(),
				LoadClass<UUI_MapMeshe>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/MapMeshe/BPUI_MeshAll.BPUI_MeshAll_C'")
					))
		);
	}
	break;
	case ELineType::DisEnable:
	{
		this->SetWidget(
			CreateWidget<UUI_MapMeshe>(this->GetWorld()->GetFirstPlayerController(),
				LoadClass<UUI_MapMeshe>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/MapMeshe/BPUI_MeshAll.BPUI_MeshAll_C'")
					))
		);
	}break;
	default:
		break;
	}

	//加载ui
	if (!this->GetWidget())
	{
		this->SetWidget(
			CreateWidget<UUI_MapMeshe>(this->GetWorld()->GetFirstPlayerController(),
				LoadClass<UUI_MapMeshe>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_MapMeshe.UI_MapMeshe_C'")
					))
		);
		UE_LOG(LogTemp, Error, TEXT("[UMapMesheInteractionComponent::OnComponentCreated]: UUI_MapMeshe 生成默认方案，因为当前指定类型不明确!"));
	}

	if (!this->GetWidget())
	{
		UE_LOG(LogTemp, Error, TEXT("[UMapMesheInteractionComponent::OnComponentCreated]: UUI_MapMeshe 加载失败!"));
		return;
	}

	if (Cast<UUI_MapMeshe>(this->GetWidget()))
	{
		this->M_UUI_MapMeshe = Cast<UUI_MapMeshe>(this->GetWidget());
	}

	this->SetTranslucency(_Translucency);
}

const int32 UMapMesheWidgetComponent::GetTranslucency() const
{
	return this->M_MesheTranslucency;
}

void UMapMesheWidgetComponent::SetTranslucency(const int32& _Translucency)
{
	//初始化渲染层级

	this->SetTranslucentSortPriority(_Translucency);

	this->M_MesheTranslucency = _Translucency;

	if (Cast<UUI_MapMeshe>(this->GetWidget()))
	{
		Cast<UUI_MapMeshe>(this->GetWidget())->SetTranslucency(_Translucency);
	}
}
