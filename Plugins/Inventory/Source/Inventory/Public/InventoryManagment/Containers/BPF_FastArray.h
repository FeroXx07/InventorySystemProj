// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "BPF_FastArray.generated.h"

struct FGameplayTag;
class UPlugInv_InventoryComponent;
class UPlugInv_InventoryItem;
class UPlugInv_ItemComponent;

/**
 * 
 */
UCLASS()
class INVENTORY_API UFastArray : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

/** Single entry for the performant and replication container array **/
USTRUCT(BlueprintType)
struct FPlugInv_InventoryItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FPlugInv_InventoryItemEntry() {}

	// FFastSerializerItem contract (called intentionally, not virtual nor override)
	// void PreReplicatedRemove(const struct FExampleArray& InArraySerializer);
	// void PostReplicatedAdd(const struct FExampleArray& InArraySerializer);
	// void PostReplicatedChange(const struct FExampleArray& InArraySerializer);
	// End of contract
	
private:
	friend UPlugInv_InventoryComponent;
	friend struct FPlugInv_InventoryFastArray;
	
	UPROPERTY()
	TObjectPtr<UPlugInv_InventoryItem> Item = nullptr;
};

/** The performant and replication container array **/
USTRUCT(BlueprintType)
struct FPlugInv_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	// Default constructor.
	FPlugInv_InventoryFastArray() : OwnerComponent(nullptr) {}

	// Parameterized constructor that accepts UActorComponent.
	FPlugInv_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	// Container getter
	TArray<TObjectPtr<UPlugInv_InventoryItem>> GetAllItems() const;

	// FFastSerializer contract (called intentionally, not virtual nor override).
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	// End of contract.
	

	// Template function, handles delta serialization.
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPlugInv_InventoryItemEntry, FPlugInv_InventoryFastArray>( Entries, DeltaParms, *this );
	}

	// Adds an entry to the container accepting an Item Component.
	TObjectPtr<UPlugInv_InventoryItem> AddEntry(TObjectPtr<UPlugInv_ItemComponent> ItemComponent);

	// Adds an entry to the container accepting an Inventory Item.
	TObjectPtr<UPlugInv_InventoryItem> AddEntry(TObjectPtr<UPlugInv_InventoryItem> Item);

	// Removes an entry from the container.
	void RemoveEntry(TObjectPtr<UPlugInv_InventoryItem> Item);

	// Return the first Item by type
	TObjectPtr<UPlugInv_InventoryItem> FindFirstItemByType(const FGameplayTag& ItemType);
private:
	friend UPlugInv_InventoryComponent;

	// Container of entries.
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FPlugInv_InventoryItemEntry> Entries;

	// Reference to the owner component, doesn't need to be replicated.
	UPROPERTY(VisibleAnywhere, NotReplicated, Category = "Inventory")
	TObjectPtr<UActorComponent> OwnerComponent;
};

/** For reflection and engine, they need to know that these have to be delta serialized, using a type trait template **/
template<>
struct TStructOpsTypeTraits< FPlugInv_InventoryFastArray > : public TStructOpsTypeTraitsBase2< FPlugInv_InventoryFastArray >
{
	enum 
	{
		WithNetDeltaSerializer = true,
   };
};
