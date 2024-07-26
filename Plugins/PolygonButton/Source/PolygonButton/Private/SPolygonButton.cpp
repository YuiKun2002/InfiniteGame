// Copyright (C) 2020, @Author: AngrySteiner

#include "SPolygonButton.h"


SLATE_IMPLEMENT_WIDGET(SPolygonButton)
void SPolygonButton::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

void SPolygonButton::SetOverlayMode()
{
	bOverlayMode = true;
}

void SPolygonButton::EnterOverlayEvent()
{
	bInOverlayEvent = true;
}

void SPolygonButton::LeaveOverlayEvent()
{
	bInOverlayEvent = false;
}

bool SPolygonButton::PointerUnderPolygon(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) const
{
	if (NormalizedPolygon.Num() > 0)
	{
		const FVector2D LocalSpacePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		const FVector2D NormalizedPosition = LocalSpacePosition / MyGeometry.GetLocalSize();
		if (!NormalizedPolygon.Contains(NormalizedPosition))
		{
			return false;
		}
	}

	return true;
}

FReply SPolygonButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bOverlayMode && !bInOverlayEvent)
	{
		return FReply::Unhandled();
	}

	if (!PointerUnderPolygon(MyGeometry, MouseEvent))
	{
		return FReply::Unhandled();
	}
	return SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SPolygonButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bOverlayMode && !bInOverlayEvent)
	{
		return FReply::Unhandled();
	}

	return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SPolygonButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bOverlayMode && !bInOverlayEvent)
	{
		return FReply::Unhandled();
	}

	// Keep track mouse "Enter"/"Leave" event
	if (bMouseEnterWidget)
	{
		const bool LocalIsUnderPolygon = PointerUnderPolygon(MyGeometry, MouseEvent);
		if (IsHovered())
		{
			if (!LocalIsUnderPolygon)
			{
				SButton::OnMouseLeave(MouseEvent);
			}
		}
		else if (LocalIsUnderPolygon)
		{
			SButton::OnMouseEnter(MyGeometry, MouseEvent);
		}
	}

	return FReply::Unhandled();
}

void SPolygonButton::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bOverlayMode && !bInOverlayEvent)
	{
		return;
	}

	bMouseEnterWidget = true;
	if (!PointerUnderPolygon(MyGeometry, MouseEvent))
	{
		return;
	}
	SButton::OnMouseEnter(MyGeometry, MouseEvent);
}

void SPolygonButton::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	if (bOverlayMode && !bInOverlayEvent)
	{
		return;
	}

	bMouseEnterWidget = false;
	if (!IsHovered())
	{
		return;
	}
	SButton::OnMouseLeave(MouseEvent);
}

