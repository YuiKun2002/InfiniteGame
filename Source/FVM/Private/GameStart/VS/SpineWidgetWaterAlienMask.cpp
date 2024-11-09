// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/SpineWidgetWaterAlienMask.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "SpineWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USpineWidgetWaterAlienMask::SetWaterViewTransform(FWidgetTransform Trans)
{
	this->CurWaterViewTrans = Trans;
	this->WaterView->SetRenderTransform(Trans);
}

void USpineWidgetWaterAlienMask::SetInWaterViewTransform(FWidgetTransform Trans)
{
	this->CurInWaterViewTrans = Trans;
	this->InWaterView->SetRenderTransform(Trans);
}

void USpineWidgetWaterAlienMask::SetMaskObjectTransform(FWidgetTransform Trans)
{
	this->CurMaskObjectTrans = Trans;
	//this->MaskCharView->SetRenderTransform(Trans);

	UWidgetLayoutLibrary::SlotAsCanvasSlot(this->MaskCharView)->SetPosition(
		FVector2D(Trans.Translation.X,
			Trans.Translation.Y)
	);
}

void USpineWidgetWaterAlienMask::GetMaskObjectTransform(FWidgetTransform& OutTrans)
{
	OutTrans = this->CurMaskObjectTrans;
}

void USpineWidgetWaterAlienMask::SetMaskObjectRotation(float Angle)
{
	this->CurMaskObjectTrans.Angle = Angle;
	this->SetMaskObjectTransform(this->CurMaskObjectTrans);
}

void USpineWidgetWaterAlienMask::SetMaskWidgetMaxHight(float MaxHight)
{
	this->MaskMaxHight = MaxHight;

	UWidgetLayoutLibrary::SlotAsCanvasSlot(this->MaskWidget)->SetSize(FVector2D(0.f, MaxHight));
}

void USpineWidgetWaterAlienMask::SetMaskValue(float Value)
{
	if (Value > this->MaskMaxValue)
	{
		return;
	}

	//当前遮罩位置
	const float& TargetValue = UKismetMathLibrary::Lerp(this->MaskMaxHight, 0, Value);

	//设置水动画位置
	this->CurWaterViewTrans.Translation.Y = TargetValue;
	this->SetWaterViewTransform(this->CurWaterViewTrans);

	//设置入水动画位置
	this->CurInWaterViewTrans.Translation.Y = TargetValue;
	this->SetInWaterViewTransform(this->CurInWaterViewTrans);

	//设置遮罩层位置
	UWidgetLayoutLibrary::SlotAsCanvasSlot(this->MaskWidget)->SetSize(FVector2D(0.f, TargetValue));
}

void USpineWidgetWaterAlienMask::SetMaskAutoValue()
{
	this->SetMaskValue(this->MaskMaxValue);
}

void USpineWidgetWaterAlienMask::SetMaskMaxValue(float Value)
{
	this->MaskMaxValue = Value;

#if WITH_EDITOR
	this->SetMaskValue(Value);
#endif
}

void USpineWidgetWaterAlienMask::SetMaskObjectRenderColor(FLinearColor NewColor)
{
	this->MaskCharView->Color = NewColor;
}

void USpineWidgetWaterAlienMask::PreViewMaskObject(UWidgetComponent* Comp)
{
#if WITH_EDITOR
	if (IsValid(Comp))
	{
		if (IsValid(Comp->GetWidget()))
		{
			Comp->GetWidget()->PreConstruct(true);
		}
	}
#endif
}

bool USpineWidgetWaterAlienMask::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (IsValid(this->MaskCharView))
	{
		//赋予纹理
		this->MaskCharView->Atlas = this->MaskObjectAtlas;
		//赋予骨架
		this->MaskCharView->SkeletonData = this->MaskObjectSkeletonData;
	}

	return true;
}

void USpineWidgetWaterAlienMask::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	this->MaskCharView->Tick(InDeltaTime, true);
	this->InWaterView->Tick(InDeltaTime, true);
	if (this->bPlayWaterAnim)
	{
		this->WaterView->Tick(InDeltaTime, true);
	}
}