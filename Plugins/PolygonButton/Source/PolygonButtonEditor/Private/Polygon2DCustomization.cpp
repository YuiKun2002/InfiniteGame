// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "Polygon2DCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "DetailLayoutBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"

#define LOCTEXT_NAMESPACE "UMG"

TSharedRef<IPropertyTypeCustomization> FPolygon2DStructCustomization::MakeInstance() 
{
	return MakeShareable( new FPolygon2DStructCustomization() );
}

void FPolygon2DStructCustomization::CustomizeHeader( TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
	HeaderRow
	.NameContent()
	[
		StructPropertyHandle->CreatePropertyNameWidget(LOCTEXT("PolygonLabel", "Polygon"))
	];
}

void FPolygon2DStructCustomization::CustomizeChildren( TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
	uint32 NumberOfChild;
	if (StructPropertyHandle->GetNumChildren(NumberOfChild) == FPropertyAccess::Success)
	{
		for (uint32 Index = 0; Index < NumberOfChild; ++Index)
		{
			TSharedRef<IPropertyHandle> ChildPropertyHandle = StructPropertyHandle->GetChildHandle(Index).ToSharedRef();
			if (ChildPropertyHandle->GetProperty()->GetFName() == FName("Vertices"))
			{
				TSharedRef<FDetailArrayBuilder> VertexBuilder = MakeShareable(new FDetailArrayBuilder(ChildPropertyHandle));
				VertexBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(this, &FPolygon2DStructCustomization::GenerateVertexWidget));
				StructBuilder.AddCustomBuilder(VertexBuilder);
			}
			else
			{
				StructBuilder.AddProperty(ChildPropertyHandle);
			}
		}
	}
}

void FPolygon2DStructCustomization::GenerateVertexWidget(TSharedRef<IPropertyHandle> PropertyHandle, int32 ArrayIndex, IDetailChildrenBuilder& StructBuilder)
{
	const FString MinString("0");
	const FString MaxString("1");
	PropertyHandle->GetProperty()->SetMetaData(TEXT("UIMin"), *MinString);
	PropertyHandle->GetProperty()->SetMetaData(TEXT("UIMax"), *MaxString);
	PropertyHandle->GetProperty()->SetMetaData(TEXT("ClampMin"), *MinString);
	PropertyHandle->GetProperty()->SetMetaData(TEXT("ClampMax"), *MaxString);
	StructBuilder.AddProperty(PropertyHandle);
	
	uint32 NumberOfChild;
	if (PropertyHandle->GetNumChildren(NumberOfChild) == FPropertyAccess::Success)
	{
		for (uint32 Index = 0; Index < NumberOfChild; ++Index)
		{
			TSharedRef<IPropertyHandle> ChildPropertyHandle = PropertyHandle->GetChildHandle(Index).ToSharedRef();
			ChildPropertyHandle->SetInstanceMetaData(TEXT("UIMin"), *MinString);
			ChildPropertyHandle->SetInstanceMetaData(TEXT("UIMax"), *MaxString);
			ChildPropertyHandle->SetInstanceMetaData(TEXT("ClampMin"), *MinString);
			ChildPropertyHandle->SetInstanceMetaData(TEXT("ClampMax"), *MaxString);
		}
	}
}

#undef LOCTEXT_NAMESPACE

