// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Manifest/F_PlugInv_ItemManifest.h"

#include "Items/O_PlugInv_InventoryItem.h"
#include "Items/Components/AC_PlugInv_ItemComponent.h"

UPlugInv_InventoryItem* FPlugInv_ItemManifest::Manifest(UObject* Outer)
{
	UPlugInv_InventoryItem* Item = NewObject<UPlugInv_InventoryItem>(Outer, UPlugInv_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);
	return Item;
}

void FPlugInv_ItemManifest::SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation,
	const FRotator& SpawnRotation)
{
	if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	const AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);

	if (!IsValid(SpawnedActor)) return;
	
	// Set the item manifest, item category, item type, etc.
	UPlugInv_ItemComponent* ItemComp = SpawnedActor->FindComponentByClass<UPlugInv_ItemComponent>();
	check(ItemComp);
	ItemComp->InitItemManifest(*this);
}
