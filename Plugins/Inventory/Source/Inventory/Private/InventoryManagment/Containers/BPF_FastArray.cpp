// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/Containers/BPF_FastArray.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "Items/O_PlugInv_InventoryItem.h"
#include "Items/Components/AC_PlugInv_ItemComponent.h"

TArray<TObjectPtr<UPlugInv_InventoryItem>> FPlugInv_InventoryFastArray::GetAllItems() const
{
	TArray<TObjectPtr<UPlugInv_InventoryItem>> Result;
	Result.Reserve(this->Entries.Num());
	for (const FPlugInv_InventoryItemEntry& Entry : this->Entries)
	{
		if (!IsValid(Entry.Item))
			continue;
		Result.Add(Entry.Item);
	}
	return Result;
}

void FPlugInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	TObjectPtr<UPlugInv_InventoryComponent> ItemComponent = Cast<UPlugInv_InventoryComponent>(this->OwnerComponent);
	if (!IsValid(ItemComponent)) return;

	for (int32 Index : RemovedIndices)
	{
		UPlugInv_DoubleLogger::Log("InventoryFastArray::PreReplicatedRemove : OnItemRemoved.Broadcast()");
		ItemComponent->OnItemRemoved.Broadcast(Entries[Index].Item);
	}
}

void FPlugInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	TObjectPtr<UPlugInv_InventoryComponent> ItemComponent = Cast<UPlugInv_InventoryComponent>(this->OwnerComponent);
	if (!IsValid(ItemComponent)) return;

	for (int32 Index : AddedIndices)
	{
		UPlugInv_DoubleLogger::Log("InventoryFastArray::PostReplicatedAdd : OnItemAdded.Broadcast()");
		ItemComponent->OnItemAdded.Broadcast(Entries[Index].Item);
	}
}

void FPlugInv_InventoryFastArray::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	TObjectPtr<UPlugInv_InventoryComponent> ItemComponent = Cast<UPlugInv_InventoryComponent>(this->OwnerComponent);
	if (!IsValid(ItemComponent)) return;

	for (int32 Index : ChangedIndices)
	{
		UPlugInv_DoubleLogger::Log("InventoryFastArray::PostReplicatedChange : OnItemChanged.Broadcast()");
		ItemComponent->OnItemChanged.Broadcast(Entries[Index].Item);
	}
}

TObjectPtr<UPlugInv_InventoryItem> FPlugInv_InventoryFastArray::AddEntry(TObjectPtr<UPlugInv_ItemComponent> ItemComponent)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	UPlugInv_InventoryComponent* InventoryComponent = Cast<UPlugInv_InventoryComponent>(this->OwnerComponent);
	if (!IsValid(InventoryComponent)) return nullptr;
	
	FPlugInv_InventoryItemEntry& NewEntryRef = Entries.AddDefaulted_GetRef();

	/**
	* This line NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor); creates a new Item so it needs to be registered.
	* The other overload doesnt create an Item but has an already made Item passed to it
	* and its not this function's responsibility to register it for replication since its not creating it.
	**/
	NewEntryRef.Item = ItemComponent->GetItemManifest().Manifest(OwningActor);
	InventoryComponent->AddSubObjToReplication(NewEntryRef.Item);
	
	MarkItemDirty(NewEntryRef);
	return NewEntryRef.Item;
}

TObjectPtr<UPlugInv_InventoryItem> FPlugInv_InventoryFastArray::AddEntry(TObjectPtr<UPlugInv_InventoryItem> Item)
{
	// Checks and not early returns because it is better to crash or pause execution if this happens in client (which should not), only auth server.
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FPlugInv_InventoryItemEntry& NewEntryRef = Entries.AddDefaulted_GetRef();
	NewEntryRef.Item = Item;

	this->MarkItemDirty(NewEntryRef);
	return Item;
}

void FPlugInv_InventoryFastArray::RemoveEntry(TObjectPtr<UPlugInv_InventoryItem> Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPlugInv_InventoryItemEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			this->MarkArrayDirty();
		}
	}
}

TObjectPtr<UPlugInv_InventoryItem> FPlugInv_InventoryFastArray::FindFirstItemByType(const FGameplayTag& ItemType)
{
	FPlugInv_InventoryItemEntry* FoundItem = Entries.FindByPredicate([Type = ItemType](const FPlugInv_InventoryItemEntry& Entry)
	{
		return IsValid(Entry.Item) && Entry.Item->GetItemManifestMutable().GetItemType().MatchesTagExact(Type);
	});
	return FoundItem ? FoundItem->Item : nullptr;
}
