// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MapData/MapDataStruct.h"
#include "AssetRegistryModule.h"
#include "UObject/Package.h"
#include "Misc/PackageName.h"
#include "UObject/SavePackage.h"
#include "Engine/DataTable.h"
#include "AssetRegistry/AssetRegistryModule.h"

UMapDataStructAsset* UMapDataStruct::CreateMapDataAsset(FString MapName)
{
	// DataTable'/Game/Resource/BP/Data/MapData/GameMapData.GameMapData'
	// D:/UnrealWork/InfiniteGame/Content/Resource/BP/Data/MapData/GameMapData.uasset
	FString PackageName = TEXT("/Game/Resource/BP/Data/MapData/Maps/");
	PackageName += MapName;

	UPackage* Package = CreatePackage(*PackageName);
	Package->FullyLoad();

	UMapDataStructAsset* TestAsset = NewObject<UMapDataStructAsset>(
		Package,
		*MapName,
		EObjectFlags::RF_Public | EObjectFlags::RF_Standalone
	);

	TestAsset->AddToRoot();
	FGameMapData NewMapData;

	
	TestAsset->MarkPackageDirty();
	Package->MarkPackageDirty();

	FAssetRegistryModule::AssetCreated(Package);
	Package->SetDirtyFlag(true);

	return TestAsset;
}
