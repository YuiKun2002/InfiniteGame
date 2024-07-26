// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "Modules/ModuleManager.h"


DEFINE_LOG_CATEGORY_STATIC( LogPolygonButton, Log, All );


class FPolygonButtonModule: public IModuleInterface
{
public:
	static inline FPolygonButtonModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FPolygonButtonModule >("FPolygonButtonModule");
	}
	
	static inline bool IsAvaiable()
	{
		return FModuleManager::Get().IsModuleLoaded("FPolygonButtonModule");
	}

public:
	/** IModuleInterfce implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

