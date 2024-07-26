// Copyright (C) 2020, @Author: AngrySteiner
// This file is mainly copied from Engine/Source/Runtime/SlateCore/Private/Widgets/SOverlay.cpp

#include "SPolygonOverlay.h"
#include "Types/PaintArgs.h"
#include "Layout/ArrangedChildren.h"
#include "SlateSettings.h"

#define LOCTEXT_NAMESPACE "PolygonButton"


/**
 * A dummy layer on top of SPolygonOverlay to capture event
 */
class SDummyLayer : public SWidget
{
public:
	SLATE_BEGIN_ARGS( SDummyLayer )
		{}
	SLATE_END_ARGS()

	SDummyLayer() : ChildSlot(this)
	{
		SetVisibility(EVisibility::Visible);
	}

	void Construct( const FArguments& InArgs ) {}

public:
	virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override
	{
		return LayerId;
	}
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		return FReply::Unhandled();
	}
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		return FReply::Unhandled();
	}
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		return FReply::Unhandled();
	}
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override {}
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override {}

	virtual FChildren* GetChildren() override { return &ChildSlot; }
	virtual void OnArrangeChildren( const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren ) const override {}

protected:
	virtual FVector2D ComputeDesiredSize(float) const override
	{
		return FVector2D::ZeroVector;
	}

	FSimpleSlot ChildSlot;
};


// SPolygonOverlay

SLATE_IMPLEMENT_WIDGET(SPolygonOverlay)
void SPolygonOverlay::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

SPolygonOverlay::SPolygonOverlay()
	: SOverlay()
	, HandleWidgetIndex(INDEX_NONE)
{
	SetCanTick(false);
	bCanSupportFocus = false;
}

SOverlay::FScopedWidgetSlotArguments SPolygonOverlay::AddSlot( int32 ZOrder )
{
	// Add dummy layer on top of overlay
	if ( Children.Num() == 0 )
	{
		FScopedWidgetSlotArguments DummySlot {MakeUnique<FOverlaySlot>((int32) 0x7FFFFFFF), this->Children, 0};
		DummySlot
			.Padding(FMargin(0, 0, 0, 0))
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SDummyLayer)
			];
	}

	int32 Index = INDEX_NONE;
	if ( ZOrder == INDEX_NONE )
	{
		// No ZOrder was specified; just add to the end of the list under dummy layer.
		ZOrder = (Children.Num() <= 1)
			? 0
			: ( Children[ Children.Num()-2 ].GetZOrder() + 1 );
		Index = Children.Num() - 1;
	}
	else
	{
		// Figure out where to add the widget based on ZOrder
		bool bFoundSlot = false;
		int32 CurSlotIndex = 0;
		for( ; CurSlotIndex < Children.Num(); ++CurSlotIndex )
		{
			const FOverlaySlot& CurSlot = Children[ CurSlotIndex ];
			if( ZOrder < CurSlot.ZOrder )
			{
				// Insert before
				bFoundSlot = true;
				break;
			}
		}
		Index = CurSlotIndex;
	}

	Invalidate(EInvalidateWidget::Layout);

	FScopedWidgetSlotArguments Result {MakeUnique<FOverlaySlot>(ZOrder), this->Children, Index};
	return MoveTemp(Result);
}

bool SPolygonOverlay::SupportsKeyboardFocus() const
{
	return true;
}

// Dispatch events to children
FReply SPolygonOverlay::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (Children.Num() <= 1)
	{
		return FReply::Unhandled();
	}

	FReply TempReply = FReply::Unhandled();
	for (int32 SlotIndex = 0; SlotIndex < Children.Num() - 1; ++SlotIndex)
	{
		TSharedRef<SWidget> Widget = Children[SlotIndex].GetWidget();
		if (TSharedPtr<SPolygonButton> Button = StaticCastSharedRef<SPolygonButton>(Widget))
		{
			Button->EnterOverlayEvent();
			// Must use widget's geometry instead of overlay's
			TempReply = Button->OnMouseButtonDown(Widget->GetCachedGeometry(), MouseEvent);
			Button->LeaveOverlayEvent();
			if (TempReply.IsEventHandled())
			{
				// Modify capture widget to overlay, remember which child handle the event
				HandleWidgetIndex = SlotIndex;
				TempReply.CaptureMouse( AsShared() );
				return TempReply;
			}
		}
	}
	return FReply::Unhandled();
}

FReply SPolygonOverlay::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (Children.Num() <= 1)
	{
		return FReply::Unhandled();
	}

	if (HandleWidgetIndex < 0 || HandleWidgetIndex >= Children.Num() - 1)
	{
		return FReply::Unhandled();
	}

	// Just the widget which handled down event needs to do up event
	TSharedRef<SWidget> Widget = Children[HandleWidgetIndex].GetWidget();
	TSharedRef<SPolygonButton> Button = StaticCastSharedRef<SPolygonButton>(Widget);

	Button->EnterOverlayEvent();
	FReply TempReply = Button->OnMouseButtonUp(Widget->GetCachedGeometry(), MouseEvent);
	Button->LeaveOverlayEvent();

	if (TempReply.GetMouseCaptor().IsValid() == false && HasMouseCapture())
	{
		TempReply.ReleaseMouseCapture();
	}

	HandleWidgetIndex = INDEX_NONE;
	return TempReply;
}

FReply SPolygonOverlay::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (Children.Num() <= 1)
	{
		return FReply::Unhandled();
	}

	for (int32 SlotIndex = 0; SlotIndex < Children.Num() - 1; ++SlotIndex)
	{
		TSharedRef<SWidget> Widget = Children[SlotIndex].GetWidget();
		if (TSharedPtr<SPolygonButton> Button = StaticCastSharedRef<SPolygonButton>(Widget))
		{
			Button->EnterOverlayEvent();
			Button->OnMouseMove(Widget->GetCachedGeometry(), MouseEvent);
			Button->LeaveOverlayEvent();
		}
	}

	return FReply::Unhandled();
}

void SPolygonOverlay::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	for (int32 SlotIndex = 0; SlotIndex < Children.Num() - 1; ++SlotIndex)
	{
		TSharedRef<SWidget> Widget = Children[SlotIndex].GetWidget();
		if (TSharedPtr<SPolygonButton> Button = StaticCastSharedRef<SPolygonButton>(Widget))
		{
			Button->EnterOverlayEvent();
			Button->OnMouseEnter(Widget->GetCachedGeometry(), MouseEvent);
			Button->LeaveOverlayEvent();
		}
	}
}

void SPolygonOverlay::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	for (int32 SlotIndex = 0; SlotIndex < Children.Num() - 1; ++SlotIndex)
	{
		TSharedRef<SWidget> Widget = Children[SlotIndex].GetWidget();
		if (TSharedPtr<SPolygonButton> Button = StaticCastSharedRef<SPolygonButton>(Widget))
		{
			Button->EnterOverlayEvent();
			Button->OnMouseLeave(MouseEvent);
			Button->LeaveOverlayEvent();
		}
	}
	HandleWidgetIndex = INDEX_NONE;
}

