// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Components/PanelWidget.h"
#include "SPolygonOverlay.h"

#include "PolygonOverlay.generated.h"


class UPolygonOverlaySlot;


/**
 * Similar to UOverlay except that it dispatches event to UPolygonButton
 */
UCLASS()
class POLYGONBUTTON_API UPolygonOverlay : public UPanelWidget
{
	GENERATED_UCLASS_BODY()

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/**  */
	UFUNCTION(BlueprintCallable, Category="Widget")
	UPolygonOverlaySlot* AddChildToOverlay(UWidget* Content);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:

	TSharedPtr<class SPolygonOverlay> MyOverlay;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};
