// Copyright (C) 2020, @Author: AngrySteiner

#include "PolygonButtonModule.h"

#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"


void FPolygonButtonModule::StartupModule()
{
	UE_LOG(LogPolygonButton, Display, TEXT("PolygonButtonModule StartupModule"));
}

void FPolygonButtonModule::ShutdownModule()
{
	UE_LOG(LogPolygonButton, Display, TEXT("PolygonButtonModule ShutdownModule"));
}


IMPLEMENT_MODULE(FPolygonButtonModule, PolygonButton)

