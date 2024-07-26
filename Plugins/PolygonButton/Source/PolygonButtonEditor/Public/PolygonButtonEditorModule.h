// Copyright (C) 2020, @Author: AngrySteiner

#pragma once

#include "Modules/ModuleManager.h"


DEFINE_LOG_CATEGORY_STATIC( LogPolygonButtonEditor, Log, All );


class FPolygonButtonEditorModule: public IModuleInterface
{
public:
	static inline FPolygonButtonEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FPolygonButtonEditorModule >("FPolygonButtonEditorModule");
	}
	
	static inline bool IsAvaiable()
	{
		return FModuleManager::Get().IsModuleLoaded("FPolygonButtonEditorModule");
	}

public:
	/* IModuleInterfce implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

