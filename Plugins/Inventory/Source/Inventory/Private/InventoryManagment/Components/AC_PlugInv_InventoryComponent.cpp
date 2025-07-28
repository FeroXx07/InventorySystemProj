// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

#include "Widgets/Inventory/InventoryBase/UW_PlugInv_InventoryBase.h"

// Sets default values for this component's properties
UPlugInv_InventoryComponent::UPlugInv_InventoryComponent() : InventoryList(this)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

/** Check in client-side and tell server, then it all replicates down to clients. **/
void UPlugInv_InventoryComponent::TryAddItem(UPlugInv_ItemComponent* ItemComponent)
{
	UPlugInv_DoubleLogger::Log("Try add item for InventoryComponent + OnNoRoomInInventory.Broadcast()");
	FPlugInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	// Scenario 3: Inventory is full, no room for new items and stackable items count reached max.
	if (Result.TotalRoomToFill == 0)
	{
		UPlugInv_DoubleLogger::Log("InventoryComponent::TryAddItem() : Scenario 3: Inventory is full");
		OnNoRoomInInventory.Broadcast();
		return;
	}

	// Scenario 1: Item already exists in inventory.
	if (Result.Item.IsValid() && Result.bStackable)
	{
		UPlugInv_DoubleLogger::Log("InventoryComponent::TryAddItem() : Scenario 1: Item already exists in inventory.");
		// Add stacks to an item that already exists in the inventory. Update the stack count and not create a new item of this type.
		Server_AddStacksToItem_Implementation(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0) // Scenario 2: New item type.
	{
		UPlugInv_DoubleLogger::Log("InventoryComponent::TryAddItem() : Scenario 2: New item type.");
		// TotalRoomToFill > 0 because we need some room!, maybe there is no room.
		// This item type doesn't exist in the inventory. Create a new one and update all pertinent slots.
		const int32 StackCount = Result.bStackable ? Result.TotalRoomToFill : 0;
		Server_AddNewItem_Implementation(ItemComponent, StackCount);
	}
}

void UPlugInv_InventoryComponent::Server_AddNewItem_Implementation(UPlugInv_ItemComponent* ItemComponent, int32 StackCount)
{
	UPlugInv_InventoryItem* NewInventoryItem = InventoryList.AddEntry(ItemComponent);

	// Maybe the current player is playing/acting on a listen server (local player who is a host or standalone)
	if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
	{
		UPlugInv_DoubleLogger::Log("InventoryComponent::Server_AddNewItem_Implementation : OnItemAdded.Broadcast()");
		this->OnItemAdded.Broadcast(NewInventoryItem);
	}
	else if (GetOwner()->GetNetMode() == ENetMode::NM_Client || GetOwner()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		// No need to broadcast as the fast array serializer already does it with PostReplicatedAdd().
	}
	
	// TODO: Tell the inventory component to destroy its owning actor.
}

void UPlugInv_InventoryComponent::Server_AddStacksToItem_Implementation(UPlugInv_ItemComponent* ItemComponent, int32 StackCount,
	int32 Remainder)
{
}

// Called when the game starts
void UPlugInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();	
}

void UPlugInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UPlugInv_InventoryComponent::AddSubObjToReplication(UObject* SubObject)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObject))
	{
		AddReplicatedSubObject(SubObject);
	}
}

void UPlugInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UPlugInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningPlayerController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(true);
}

void UPlugInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningPlayerController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(false);
}

void UPlugInv_InventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	checkf(OwningPlayerController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner"));
	if (!OwningPlayerController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UPlugInv_InventoryBase>(OwningPlayerController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	
	CloseInventoryMenu();
}



