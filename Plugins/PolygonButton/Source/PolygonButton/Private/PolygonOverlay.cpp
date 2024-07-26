// Copyright (C) 2020, @Author: AngrySteiner
// This file is mainly copied from Engine/Source/Runtime/UMG/Private/Components/Overlay.cpp

#include "PolygonOverlay.h"
#include "PolygonOverlaySlot.h"
#include "Components/OverlaySlot.h"

#define LOCTEXT_NAMESPACE "UMG"


UPolygonOverlay::UPolygonOverlay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::Visible;
}

void UPolygonOverlay::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyOverlay.Reset();
}

UPolygonOverlaySlot* UPolygonOverlay::AddChildToOverlay(UWidget* Content)
{
	return Cast<UPolygonOverlaySlot>(Super::AddChild(Content));
}

UClass* UPolygonOverlay::GetSlotClass() const
{
	return UPolygonOverlaySlot::StaticClass();
}

void UPolygonOverlay::OnSlotAdded(UPanelSlot* InSlot)
{
	if ( MyOverlay.IsValid() )
	{
		CastChecked<UPolygonOverlaySlot>(InSlot)->BuildSlot(MyOverlay.ToSharedRef());
	}
}

void UPolygonOverlay::OnSlotRemoved(UPanelSlot* InSlot)
{
	if ( MyOverlay.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			MyOverlay->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UPolygonOverlay::RebuildWidget()
{
	MyOverlay = SNew(SPolygonOverlay);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UPolygonOverlaySlot* TypedSlot = Cast<UPolygonOverlaySlot>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyOverlay.ToSharedRef());
		}
	}

	return MyOverlay.ToSharedRef();
}

#if WITH_EDITOR

const FText UPolygonOverlay::GetPaletteCategory()
{
	return LOCTEXT("Polygon", "Polygon");
}

#endif


#undef LOCTEXT_NAMESPACE
