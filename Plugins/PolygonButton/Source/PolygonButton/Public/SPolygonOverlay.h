// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SOverlay.h"


/**
 * Slate overlay that holds polygon buttons and dispatches events to them
 */
class SPolygonOverlay : public SOverlay
{
	SLATE_DECLARE_WIDGET(SPolygonOverlay, SOverlay)
public:

	SPolygonOverlay();

	/**
	 * Adds a content slot.
	 *
	 * @return The added slot.
	 */
	FScopedWidgetSlotArguments AddSlot( int32 ZOrder=INDEX_NONE );

public:
	virtual bool SupportsKeyboardFocus() const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

private:
	int32 HandleWidgetIndex;
};

