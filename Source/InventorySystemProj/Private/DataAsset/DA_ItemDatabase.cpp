// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_ItemDatabase.h"
#include "Logging/LogMacros.h"
#include "Engine/AssetManager.h"

void UItemDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	PopulateDatabaseArray();
}
void UItemDatabase::PostInitProperties()
{
	Super::PostInitProperties();
	PopulateDatabaseArray();
}
void UItemDatabase::PopulateDatabaseArray()
{
	// To avoid crashes wait until asset manager singleton is intialized
	if(!UAssetManager::IsInitialized())
		return;
	
	// Get primary data asset by type
	const UAssetManager& AssetManager = UAssetManager::Get();
	const FPrimaryAssetType Type = FPrimaryAssetType(FName("ItemCore"));
	TArray<FAssetData> Assets;
	AssetManager.GetPrimaryAssetDataList(Type, Assets);
	
	// Empty the current array if it isn't
	if (!DataAssets.IsEmpty())
	{
		DataAssets.Empty();
	}
	
	// Assign the array
	for (auto Asset : Assets)
	{
		UObject* LoadedAsset = Asset.GetAsset();
		DataAssets.Push(Cast<UItemCore>(LoadedAsset));
	}
}
