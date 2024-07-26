// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "Components/OverlaySlot.h"

#include "PolygonOverlaySlot.generated.h"


/**
 * Slot for the UPolygonOverlay panel
 * Allows content to be hover above other content
 */
UCLASS()
class POLYGONBUTTON_API UPolygonOverlaySlot : public UOverlaySlot
{
	GENERATED_UCLASS_BODY()

public:
	/** Builds the underlying FSlot for the Slate layout panel. */
	virtual void BuildSlot(TSharedRef<SOverlay> InOverlay) override;
};
