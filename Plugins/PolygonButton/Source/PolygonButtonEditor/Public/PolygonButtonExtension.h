// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "CoreMinimal.h"
#include "DesignerExtension.h"
#include "IHasDesignerExtensibility.h"


class SCanvas;
class UPolygonButton;


/**
 * Extension that enable polygon editing for PolygonButton in UMGEditor
 */
class FPolygonButtonExtension: public FDesignerExtension
{
public:
	FPolygonButtonExtension();

	virtual ~FPolygonButtonExtension();

	static TSharedRef<IDesignerExtensionFactory> MakeDesignerExtension();

	/** Override FDesignerExtension*/
	virtual bool CanExtendSelection(const TArray< FWidgetReference >& Selection) const override;
	virtual void ExtendSelection(const TArray< FWidgetReference >& Selection, TArray< TSharedRef<FDesignerSurfaceElement> >& SurfaceElements) override;
	virtual void Paint(const TSet< FWidgetReference >& Selection, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId) const override;
	/** */

protected:
	TSharedRef<SWidget> MakePolygonWidget(UPolygonButton* PolygonButton);
	TSharedRef<SWidget> MakeVertexWidget(UPolygonButton* PolygonButton, int VertexIndex);

	FVector2D GetVertexPosition(UPolygonButton* PolygonButton, int VertexIndex) const;
	EVisibility CheckVertexVisibility(int VertexIndex) const;

protected:
	FReply HandleVertexBeginDrag(const FGeometry& Geomerty, const FPointerEvent& Event, int VertexIndex);
	FReply HandleVertexEndDrag(const FGeometry& Geomerty, const FPointerEvent& Event, int VertexIndex);
	FReply HandleVertexDragging(const FGeometry& Geomerty, const FPointerEvent& Event, int VertexIndex);

	void InsertVertex(UPolygonButton* PolygonButton, int VertexIndex);
	void DeleteVertex(UPolygonButton* PolygonButton, int VertexIndex);

private:
	TSharedPtr<SCanvas> PolygonSWidget;
	TArray< TSharedPtr<SWidget> > VertexSWidgets;
	
	TWeakPtr<FDesignerSurfaceElement> ExtensionElement;

	bool bMovingVertex;
	FVector2D MouseDownVertex;
	FVector2D MouseDownPosition;
};

