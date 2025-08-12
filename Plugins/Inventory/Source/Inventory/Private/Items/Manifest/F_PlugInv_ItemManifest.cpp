// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Manifest/F_PlugInv_ItemManifest.h"

#include "Items/O_PlugInv_InventoryItem.h"
#include "Items/Components/AC_PlugInv_ItemComponent.h"
#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"
#include "Widgets/Composite/UW_PlugInv_CompositeBase.h"

UPlugInv_InventoryItem* FPlugInv_ItemManifest::Manifest(UObject* Outer)
{
	UPlugInv_InventoryItem* Item = NewObject<UPlugInv_InventoryItem>(Outer, UPlugInv_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	for (TInstancedStruct<FPlugInv_ItemFragment>& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}
	
	ClearFragments();
	
	return Item;
}

void FPlugInv_ItemManifest::AssimilateInventoryFragments(UPlugInv_CompositeBase* Composite) const
{
	const TArray<const FPlugInv_InventoryItemFragment*>& InventoryItemFragments = GetAllFragmentsOfType<FPlugInv_InventoryItemFragment>();

	for (const FPlugInv_InventoryItemFragment* Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction([Fragment](UPlugInv_CompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
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

void FPlugInv_ItemManifest::ClearFragments()
{
	for (TInstancedStruct<FPlugInv_ItemFragment>& Fragment : Fragments)
	{
		Fragment.Reset();
	}
	
	Fragments.Empty();
}
