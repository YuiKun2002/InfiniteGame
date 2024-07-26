// Copyright (C) 2020, @Author: AngrySteiner

#include "PolygonButtonEditorModule.h"

#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"
#include "UMGEditorModule.h"
#include "PropertyEditorModule.h"

#include "Polygon2D.h"
#include "PolygonButtonExtension.h"
#include "Polygon2DCustomization.h"


void FPolygonButtonEditorModule::StartupModule()
{
	UE_LOG(LogPolygonButtonEditor, Display, TEXT("PolygonButtonModuleEditor StartupModule"));
	
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FPolygon2D::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPolygon2DStructCustomization::MakeInstance));

	IUMGEditorModule& UMGEditorInterface = FModuleManager::GetModuleChecked<IUMGEditorModule>("UMGEditor");
	TSharedPtr<FDesignerExtensibilityManager> DesignerExtensibilityManager = UMGEditorInterface.GetDesignerExtensibilityManager();

	DesignerExtensibilityManager->AddDesignerExtensionFactory(FPolygonButtonExtension::MakeDesignerExtension());
}

void FPolygonButtonEditorModule::ShutdownModule()
{
	UE_LOG(LogPolygonButtonEditor, Display, TEXT("PolygonButtonEditorModule ShutdownModule"));
	
	FPropertyEditorModule* PropertyEditorModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule->UnregisterCustomClassLayout(FPolygon2D::StaticStruct()->GetFName());
}


IMPLEMENT_MODULE(FPolygonButtonEditorModule, PolygonButtonEditor)

