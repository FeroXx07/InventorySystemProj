// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AC_Inventory.h"

#include "BlueprintFunctionLibrary/BPF_DoubleLogger.h"
#include "PrimaryDataAsset/PDA_ItemCore.h"

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();
	UDoubleLogger::LogWarning("AC_Inventory C++ Begin Play");
	// ...
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
bool UInventory::AddItemToInventory(UInventoryItem* ItemToAdd, const int64 ContainerUniqueId)
{
	for (FInventoryContainer& Container : InventoryContainers)
	{
		if (Container.UniqueID == ContainerUniqueId)
		{
			InventoryItems.Add(ItemToAdd);
			Container.InventoryItems.Add(ItemToAdd);
			
			TArray< FStringFormatArg > args; 
			args.Add( FStringFormatArg( ItemToAdd->DataAssetItem->GetUniqueID()) ); 
			args.Add( FStringFormatArg( ItemToAdd->UniqueID ) ); 
			args.Add( FStringFormatArg( ItemToAdd->DataAssetItem->Item.Name.ToString()) ); 
			FString logString = FString::Format( TEXT( "Item added, Id = {0}, InventoryItem UniqueId = {1}, Item Name = {2}" ), args );
			UDoubleLogger::Log(logString);
			
			return true;
		}
	}
	return false;
}
bool UInventory::AddItemToInventoryByDataAsset(UItemCore* ItemToAddDataAsset, const int64 ContainerUniqueId)
{
	for (FInventoryContainer& Container : InventoryContainers)
	{
		if (Container.UniqueID == ContainerUniqueId)
		{
			const TObjectPtr<UInventoryItem> ItemToAdd = NewObject<UInventoryItem>();
			ItemToAdd->DataAssetItem = ItemToAddDataAsset;
			
			InventoryItems.Add(ItemToAdd);
			Container.InventoryItems.Add(ItemToAdd);

			TArray< FStringFormatArg > args; 
			args.Add( FStringFormatArg( ItemToAdd->DataAssetItem->GetUniqueID()) ); 
			args.Add( FStringFormatArg( ItemToAdd->UniqueID ) ); 
			args.Add( FStringFormatArg( ItemToAdd->DataAssetItem->Item.Name.ToString()) ); 
			FString logString = FString::Format( TEXT( "Item added, Id = {0}, InventoryItem UniqueId = {1}, Item Name = {2}" ), args );
			UDoubleLogger::Log(logString);
			return true;
		}
	}
	
	return false;
}
bool UInventory::RemoveItemFromInventory(UInventoryItem* ItemToAdd, const int64 ContainerUniqueId)
{
	for (FInventoryContainer& Container : InventoryContainers)
	{
		if (Container.UniqueID == ContainerUniqueId)
		{
			InventoryItems.Remove(ItemToAdd);
			Container.InventoryItems.Remove(ItemToAdd);
			
			TArray< FStringFormatArg > args; 
			args.Add( FStringFormatArg( ItemToAdd->DataAssetItem->GetUniqueID()) ); 
			args.Add( FStringFormatArg( ItemToAdd->UniqueID ) ); 
			args.Add( FStringFormatArg( ItemToAdd->DataAssetItem->Item.Name.ToString()) ); 
			FString logString = FString::Format( TEXT( "Item removed, Id = {0}, InventoryItem UniqueId = {1}, Item Name = {2}" ), args );
			UDoubleLogger::Log(logString);
			
			return true;
		}
	}
	return false;
}

