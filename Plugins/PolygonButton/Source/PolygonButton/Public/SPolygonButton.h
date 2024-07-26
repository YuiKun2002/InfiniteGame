// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SButton.h"

#include "Polygon2D.h"


/**
 * Slate button that use a polygon to determine whether or not to response mouse events
 * Polygon is normalized
 */
class SPolygonButton : public SButton
{
	SLATE_DECLARE_WIDGET(SPolygonButton, SButton)

public:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

public:
	void SetOverlayMode();
	void EnterOverlayEvent();
	void LeaveOverlayEvent();

protected:
	bool PointerUnderPolygon(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) const;

public:
	FPolygon2D NormalizedPolygon;

protected:
	bool bMouseEnterWidget = false;
	bool bOverlayMode = false;
	bool bInOverlayEvent = false;
};

