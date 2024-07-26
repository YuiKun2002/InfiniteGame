// Copyright (C) 2020, @Author: AngrySteiner

#include "PolygonButtonExtension.h"
#include "PolygonButtonEditorModule.h"

#include "IUMGDesigner.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SCanvas.h"
#include "WidgetBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "PolygonButton.h"


#define LOCTEXT_NAMESPACE "UMG"


/**
 * Extension factory that create actual extension for designer view.
 */
class FPolygonButtonExtensionFactory : public IDesignerExtensionFactory, public TSharedFromThis<FPolygonButtonExtensionFactory>
{
public:
	virtual ~FPolygonButtonExtensionFactory() {};

	virtual TSharedRef<FDesignerExtension> CreateDesignerExtension() const
	{
		return StaticCastSharedRef<FDesignerExtension>(MakeShared<FPolygonButtonExtension>());
	}
};



FPolygonButtonExtension::FPolygonButtonExtension()
	: bMovingVertex(false)
{
}

FPolygonButtonExtension::~FPolygonButtonExtension()
{
}

TSharedRef<IDesignerExtensionFactory> FPolygonButtonExtension::MakeDesignerExtension()
{
	static TSharedPtr<FPolygonButtonExtensionFactory> ExtensionFactory = MakeShared<FPolygonButtonExtensionFactory>();

	return ExtensionFactory.ToSharedRef();
}

bool FPolygonButtonExtension::CanExtendSelection(const TArray< FWidgetReference >& Selection) const
{
	for ( const FWidgetReference& Widget : Selection )
	{
		if ( !Widget.GetTemplate()->IsA(UPolygonButton::StaticClass()) )
		{
			return false;
		}
	}

	return Selection.Num() == 1;
}

void FPolygonButtonExtension::ExtendSelection(const TArray< FWidgetReference >& Selection, TArray< TSharedRef<FDesignerSurfaceElement> >& SurfaceElements)
{
	SelectionCache = Selection;

	for (const FWidgetReference& SelectionRef : SelectionCache)
	{
		UPolygonButton* PreviewWidget = Cast<UPolygonButton>(SelectionRef.GetPreview());
		if (!PreviewWidget)
		{
			UE_LOG(LogPolygonButtonEditor, Error, TEXT("Extending Polygon Vertex with non-UPolygonButton object"));
			break;
		}
		if (PreviewWidget->NormalizedPolygon.Num() <= 0)
		{
			UPolygonButton* TemplateWidget = Cast<UPolygonButton>(SelectionRef.GetTemplate());
			if (TemplateWidget->NormalizedPolygon.Num() <= 0)
			{
				TemplateWidget->NormalizedPolygon.PushVertex(FVector2D(0, 0));
				TemplateWidget->NormalizedPolygon.PushVertex(FVector2D(1, 0));
				TemplateWidget->NormalizedPolygon.PushVertex(FVector2D(1, 1));
				TemplateWidget->NormalizedPolygon.PushVertex(FVector2D(0, 1));
			}
			PreviewWidget->NormalizedPolygon = TemplateWidget->NormalizedPolygon;
		}
		{
			// Calculate offset to avoid pivot correction of SDesignerView so we won't overlap with STransformHandle
			// and we also get more accurate positions
			// NOTE: this is not good enough if we resize the button, any better solution?
			const FGeometry& Geometry = PreviewWidget->GetCachedGeometry();
			FVector2D Offset = Designer->GetDesignerGeometry().AbsoluteToLocal(Geometry.LocalToAbsolute(FVector2D(0, 0))) + FVector2D(10, 10);
			Offset = Geometry.AbsoluteToLocal(Designer->GetDesignerGeometry().LocalToAbsolute(Offset));
			Offset = Offset * FVector2D(Geometry.GetAccumulatedRenderTransform().GetMatrix().GetScale().GetVector()) / Designer->GetDesignerGeometry().Scale;
			TSharedRef<SWidget> PolygonWidget = MakePolygonWidget(PreviewWidget);

			TSharedRef<FDesignerSurfaceElement> TempSurfaceElement = MakeShareable(new FDesignerSurfaceElement(PolygonWidget, EExtensionLayoutLocation::TopLeft, Offset));
			ExtensionElement = TempSurfaceElement;

			SurfaceElements.Add(TempSurfaceElement);
		}
	}
}

void FPolygonButtonExtension::Paint(const TSet< FWidgetReference >& Selection, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
	// Paint polygon lines
	if (SelectionCache.Num() <= 0)
	{
		return;
	}
	UWidget* PreviewCache = SelectionCache[0].GetPreview();
	UWidget* Preview = nullptr;
	for (const FWidgetReference& SelectedWidget : Selection)
	{
		Preview = SelectedWidget.GetPreview();
		if (!Preview || !PreviewCache || PreviewCache != Preview)
		{
			Preview = nullptr;
			continue;
		}
		break;
	}
	if (!Preview || !Preview->IsVisibleInDesigner())
	{
		return;
	}
	UPolygonButton* PreviewWidget = Cast<UPolygonButton>(SelectionCache[0].GetPreview());
	if (PreviewWidget->NormalizedPolygon.Num() <= 0)
	{
		return;
	}

	TArray<FVector2D> LinePoints;
	static FLinearColor LineColor(1.0f, 0.0f, 1.0f);
	FVector2D Origin = PreviewWidget->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0, 0));
	for (int i = 0; i < PreviewWidget->NormalizedPolygon.Num(); ++i)
	{
		FVector2D Point = PreviewWidget->NormalizedPolygon[i];
		Point = PreviewWidget->GetCachedGeometry().GetLocalPositionAtCoordinates(Point) - Origin;
		LinePoints.Add(Point);
	}
	Origin = LinePoints[0];
	LinePoints.Add(Origin);

	FGeometry CanvasGeometry;
	Designer->GetWidgetGeometry(PreviewWidget, CanvasGeometry);
	CanvasGeometry = Designer->MakeGeometryWindowLocal(CanvasGeometry);

	// Paint lines
	for (int i = 1; i < LinePoints.Num(); ++i)
	{
		TArray<FVector2D> CurLine;
		CurLine.Add(LinePoints[i - 1]);
		CurLine.Add(LinePoints[i]);
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			CanvasGeometry.ToPaintGeometry(),
			CurLine,
			ESlateDrawEffect::None,
			LineColor,
			true,
			2.0f
		);
	}

	// Paint transform handlers
	for (int i = 0; i < PreviewWidget->NormalizedPolygon.Num(); ++i)
	{
		FVector2D Point = PreviewWidget->NormalizedPolygon[i];
		Point = CanvasGeometry.GetAbsolutePositionAtCoordinates(Point);
		FGeometry HandlerGeometry(FVector2D(-3, -3), Point, FVector2D(6, 6), 1.0f);
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			HandlerGeometry.ToPaintGeometry(),
			FEditorStyle::Get().GetBrush("UMGEditor.TransformHandle"),
			ESlateDrawEffect::None,
			FLinearColor::White
		);
	}

	if (ExtensionElement.IsValid())
	{
		// Calculate offset to avoid pivot correction of SDesignerView so we won't overlap with STransformHandle
		// and we also get more accurate positions
		const FGeometry& Geometry = PreviewWidget->GetCachedGeometry();
		FVector2D Offset = Designer->GetDesignerGeometry().AbsoluteToLocal(Geometry.LocalToAbsolute(FVector2D(0, 0))) + FVector2D(10, 10);
		Offset = Geometry.AbsoluteToLocal(Designer->GetDesignerGeometry().LocalToAbsolute(Offset));
		Offset = Offset * FVector2D(Geometry.GetAccumulatedRenderTransform().GetMatrix().GetScale().GetVector()) / Designer->GetDesignerGeometry().Scale;
		ExtensionElement.Pin()->SetOffset(Offset);
	}
}

TSharedRef<SWidget> FPolygonButtonExtension::MakePolygonWidget(UPolygonButton* PolygonButton)
{
	PolygonSWidget.Reset();
	VertexSWidgets.Empty();
	VertexSWidgets.SetNumZeroed(PolygonButton->NormalizedPolygon.Num());
	SAssignNew(PolygonSWidget, SCanvas)
		.Visibility(EVisibility::Visible);

	for (int32 i = 0; i < PolygonButton->NormalizedPolygon.Num(); ++i)
	{
		PolygonSWidget->AddSlot()
			.Position_Lambda([this, PolygonButton, i]() { return this->GetVertexPosition(PolygonButton, i); })
			.Size(FVector2D(6, 6))
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				MakeVertexWidget(PolygonButton, i)
			];
	}
	// Prevent extension widget from clipping
	PolygonSWidget->SetClipping(EWidgetClipping::ClipToBoundsWithoutIntersecting);

	return PolygonSWidget.ToSharedRef();
}

TSharedRef<SWidget> FPolygonButtonExtension::MakeVertexWidget(UPolygonButton* PolygonButton, int VertexIndex)
{
	 SAssignNew(VertexSWidgets[VertexIndex], SBorder)
		.BorderImage(FEditorStyle::Get().GetBrush("NoBrush"))
		.OnMouseButtonDown(this, &FPolygonButtonExtension::HandleVertexBeginDrag, VertexIndex)
		.OnMouseButtonUp(this, &FPolygonButtonExtension::HandleVertexEndDrag, VertexIndex)
		.OnMouseMove(this, &FPolygonButtonExtension::HandleVertexDragging, VertexIndex)
		// Used to hide those vertices that is "removed" from polygon
		.Visibility_Lambda([this, VertexIndex]() { return this->CheckVertexVisibility(VertexIndex); })
		.Padding(FMargin(0))
		[
			// Use NoBrush, only for dragging, visual vertex will be painted in Paint()
			SNew(SImage)
			.Image(FEditorStyle::Get().GetBrush("NoBrush"))
		];
	 VertexSWidgets[VertexIndex]->SetCursor(EMouseCursor::GrabHand);
	 // Prevent handler from clipping
	 VertexSWidgets[VertexIndex]->SetClipping(EWidgetClipping::ClipToBoundsWithoutIntersecting);
	 return VertexSWidgets[VertexIndex].ToSharedRef();
}

FVector2D FPolygonButtonExtension::GetVertexPosition(UPolygonButton* PolygonButton, int VertexIndex) const
{
	if (VertexIndex >= PolygonButton->NormalizedPolygon.Num())
	{
		return FVector2D::ZeroVector;
	}
	const FGeometry& Geometry = PolygonButton->GetCachedGeometry();
	const FVector2D Origin = Geometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));
	const FVector2D Vertex = Geometry.GetLocalPositionAtCoordinates(PolygonButton->NormalizedPolygon[VertexIndex]);
	const FSlateRenderTransform& RenderTransform = Geometry.GetAccumulatedRenderTransform();

	return RenderTransform.TransformVector(Vertex - Origin) / Geometry.Scale * Designer->GetPreviewScale();
}

EVisibility FPolygonButtonExtension::CheckVertexVisibility(int VertexIndex) const
{
	UPolygonButton* PreviewWidget = Cast<UPolygonButton>(SelectionCache[0].GetPreview());
	return VertexIndex < PreviewWidget->NormalizedPolygon.Num() ? EVisibility::Visible : EVisibility::Hidden;
}

void FPolygonButtonExtension::InsertVertex(UPolygonButton* PolygonButton, int VertexIndex)
{
	BeginTransaction(LOCTEXT("PolygonButton_Insert", "Insert Polygon Vertex"));

	UPolygonButton* TemplateWidget = Cast<UPolygonButton>(SelectionCache[0].GetTemplate());

	FVector2D VertexPosition = PolygonButton->NormalizedPolygon[VertexIndex];
	PolygonButton->NormalizedPolygon.InsertVertex(VertexIndex, VertexPosition);
	TemplateWidget->NormalizedPolygon = PolygonButton->NormalizedPolygon;
	int MaxIndex = PolygonButton->NormalizedPolygon.Num() - 1;
	// Only create new vertex widgets when we run out of "deleted vertices"
	if (MaxIndex >= VertexSWidgets.Num())
	{
		VertexSWidgets.AddZeroed(1);
		PolygonSWidget->AddSlot()
			.Position_Lambda([this, PolygonButton, MaxIndex]() { return this->GetVertexPosition(PolygonButton, MaxIndex); })
			.Size(FVector2D(6, 6))
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				MakeVertexWidget(PolygonButton, MaxIndex)
			];
	}

	EndTransaction();
}

void FPolygonButtonExtension::DeleteVertex(UPolygonButton* PolygonButton, int VertexIndex)
{
	BeginTransaction(LOCTEXT("PolygonButton_Delete", "Delete Polygon Vertex"));

	UPolygonButton* TemplateWidget = Cast<UPolygonButton>(SelectionCache[0].GetTemplate());
	if (PolygonButton->NormalizedPolygon.Num() < 4)
	{
		return;
	}
	// We don't really delete vertex widgets, just hide them by visibilities
	PolygonButton->NormalizedPolygon.RemoveVertex(VertexIndex);
	TemplateWidget->NormalizedPolygon = PolygonButton->NormalizedPolygon;

	EndTransaction();
}

FReply FPolygonButtonExtension::HandleVertexBeginDrag(const FGeometry& Geomerty, const FPointerEvent& Event, int VertexIndex)
{
	UPolygonButton* PreviewWidget = Cast<UPolygonButton>(SelectionCache[0].GetPreview());

	if (FSlateApplication::Get().GetModifierKeys().IsControlDown())
	{
		InsertVertex(PreviewWidget, VertexIndex);
	}
	else if (FSlateApplication::Get().GetModifierKeys().IsShiftDown())
	{
		DeleteVertex(PreviewWidget, VertexIndex);
		return FReply::Handled();
	}

	BeginTransaction(LOCTEXT("PolygonButton_Move", "Move Polygon Vertex"));

	bMovingVertex = true;
	MouseDownVertex = PreviewWidget->NormalizedPolygon[VertexIndex];
	MouseDownPosition = PreviewWidget->GetCachedGeometry().AbsoluteToLocal(Event.GetScreenSpacePosition());

	VertexSWidgets[VertexIndex]->SetCursor(EMouseCursor::GrabHandClosed);

	return FReply::Handled().CaptureMouse(VertexSWidgets[VertexIndex].ToSharedRef());
}

FReply FPolygonButtonExtension::HandleVertexEndDrag(const FGeometry& Geomerty, const FPointerEvent& Event, int VertexIndex)
{
	if (!bMovingVertex)
	{
		return FReply::Handled().ReleaseMouseCapture();
	}
	
	EndTransaction();

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint.Get());

	bMovingVertex = false;
	VertexSWidgets[VertexIndex]->SetCursor(EMouseCursor::GrabHand);

	return FReply::Handled().ReleaseMouseCapture();
}

FReply FPolygonButtonExtension::HandleVertexDragging(const FGeometry& Geomerty, const FPointerEvent& Event, int VertexIndex)
{
	if (!bMovingVertex)
	{
		return FReply::Unhandled();
	}

	for (const FWidgetReference& SelectionRef : SelectionCache)
	{
		UPolygonButton* PreviewWidget = Cast<UPolygonButton>(SelectionRef.GetPreview());
		UPolygonButton* TemplateWidget = Cast<UPolygonButton>(SelectionRef.GetTemplate());
		if (!PreviewWidget)
		{
			UE_LOG(LogPolygonButtonEditor, Error, TEXT("Get Polygon Widget width with non-UPolygonButton object"));
			break;
		}
		FVector2D VertexDelta = PreviewWidget->GetCachedGeometry().AbsoluteToLocal(Event.GetScreenSpacePosition()) - MouseDownPosition;
		VertexDelta /= PreviewWidget->GetCachedGeometry().GetLocalSize();
		VertexDelta = MouseDownVertex + VertexDelta;
		PreviewWidget->NormalizedPolygon[VertexIndex] = VertexDelta.ClampAxes(0.0f, 1.0f);

		TemplateWidget->NormalizedPolygon[VertexIndex] = VertexDelta.ClampAxes(0.0f, 1.0f);
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

