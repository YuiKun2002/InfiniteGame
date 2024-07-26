// Copyright (C) 2020, @Author: AngrySteiner

#include "PolygonOverlaySlot.h"
#include "SPolygonOverlay.h"
#include "Components/Widget.h"


UPolygonOverlaySlot::UPolygonOverlaySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HorizontalAlignment = HAlign_Left;
	VerticalAlignment = VAlign_Top;
	Slot = NULL;
}

void UPolygonOverlaySlot::BuildSlot(TSharedRef<SOverlay> Overlay)
{
	TSharedRef<SPolygonOverlay> PolygonOverlay = StaticCastSharedRef<SPolygonOverlay>(Overlay);
	Slot = PolygonOverlay->AddSlot()
		.Padding(Padding)
		.HAlign(HorizontalAlignment)
		.VAlign(VerticalAlignment)
		[
			Content == NULL ? SNullWidget::NullWidget : Content->TakeWidget()
		].GetSlot();
	TSharedRef<SWidget> Widget = Slot->GetWidget();
	if (TSharedPtr<SPolygonButton> Button = StaticCastSharedRef<SPolygonButton>(Widget))
	{
		Button->SetOverlayMode();
	}
}
