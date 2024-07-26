// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/Button.h"

#include "Polygon2D.h"
#include "SPolygonButton.h"
#include "PolygonButton.generated.h"

class SPolygonButton;


/**
 * UMG button that use a polygon to determine whether or not to response mouse events
 * Polygon is normalized
 */
UCLASS()
class POLYGONBUTTON_API UPolygonButton : public UButton
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Interaction", meta=( DisplayName="Polygon" ))
	FPolygon2D NormalizedPolygon;

	UFUNCTION(BlueprintCallable, Category="PolygonButton")
	void SetPolygonVertices(const TArray<FVector2D>& VertexList);

	UFUNCTION(BlueprintCallable, Category="PolygonButton")
	const TArray<FVector2D> GetPolygonVertices();

public:
	/** Sets the color multiplier for the button background */
	void SetStyle(const FButtonStyle& InStyle);

	/** Sets the color multiplier for the button content */
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/** Sets the color multiplier for the button background */
	void SetBackgroundColor(FLinearColor InBackgroundColor);

	bool IsPressed() const;

	void SetClickMethod(EButtonClickMethod::Type InClickMethod);

	void SetTouchMethod(EButtonTouchMethod::Type InTouchMethod);

	void SetPressMethod(EButtonPressMethod::Type InPressMethod);

public:

	virtual void SynchronizeProperties() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
	TSharedPtr<SPolygonButton> MyPolygonButton;

};

