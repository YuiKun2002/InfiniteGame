// Copyright (C) 2020, @Author: AngrySteiner
// This file is mainly copied from Engine/Source/Runtime/UMG/Private/Components/Button.cpp

#include "PolygonButton.h"

#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SButton.h"
#include "Components/ButtonSlot.h"

#define LOCTEXT_NAMESPACE "UMG"


UPolygonButton::UPolygonButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPolygonButton::SetPolygonVertices(const TArray<FVector2D>& VertexList)
{
	MyPolygonButton->NormalizedPolygon = FPolygon2D(VertexList);
}

const TArray<FVector2D> UPolygonButton::GetPolygonVertices()
{
	return MyPolygonButton->NormalizedPolygon.Vertices;
}

void UPolygonButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyPolygonButton.Reset();
}

TSharedRef<SWidget> UPolygonButton::RebuildWidget()
{
	MyPolygonButton = SNew(SPolygonButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject( this, &ThisClass::SlateHandleHovered )
		.OnUnhovered_UObject( this, &ThisClass::SlateHandleUnhovered )
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if ( GetChildrenCount() > 0 )
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyPolygonButton.ToSharedRef());
	}

	MyPolygonButton->NormalizedPolygon = NormalizedPolygon;
	
	return MyPolygonButton.ToSharedRef();
}

void UPolygonButton::SynchronizeProperties()
{
	UContentWidget::SynchronizeProperties();

	MyPolygonButton->SetColorAndOpacity( ColorAndOpacity );
	MyPolygonButton->SetBorderBackgroundColor( BackgroundColor );
}

UClass* UPolygonButton::GetSlotClass() const
{
	return UButtonSlot::StaticClass();
}

void UPolygonButton::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if ( MyPolygonButton.IsValid() )
	{
		CastChecked<UButtonSlot>(InSlot)->BuildSlot(MyPolygonButton.ToSharedRef());
	}
}

void UPolygonButton::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyPolygonButton.IsValid() )
	{
		MyPolygonButton->SetContent(SNullWidget::NullWidget);
	}
}

void UPolygonButton::SetStyle(const FButtonStyle& InStyle)
{
	WidgetStyle = InStyle;
	if ( MyPolygonButton.IsValid() )
	{
		MyPolygonButton->SetButtonStyle(&WidgetStyle);
	}
}

void UPolygonButton::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if ( MyPolygonButton.IsValid() )
	{
		MyPolygonButton->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UPolygonButton::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	if ( MyPolygonButton.IsValid() )
	{
		MyPolygonButton->SetBorderBackgroundColor(InBackgroundColor);
	}
}

bool UPolygonButton::IsPressed() const
{
	if ( MyPolygonButton.IsValid() )
	{
		return MyPolygonButton->IsPressed();
	}

	return false;
}

void UPolygonButton::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
	if ( MyPolygonButton.IsValid() )
	{
		MyPolygonButton->SetClickMethod(ClickMethod);
	}
}

void UPolygonButton::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
	if ( MyPolygonButton.IsValid() )
	{
		MyPolygonButton->SetTouchMethod(TouchMethod);
	}
}

void UPolygonButton::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
	if ( MyPolygonButton.IsValid() )
	{
		MyPolygonButton->SetPressMethod(PressMethod);
	}
}

#if WITH_EDITOR
const FText UPolygonButton::GetPaletteCategory()
{
	return LOCTEXT("Polygon", "Polygon");
}
#endif


#undef LOCTEXT_NAMESPACE


