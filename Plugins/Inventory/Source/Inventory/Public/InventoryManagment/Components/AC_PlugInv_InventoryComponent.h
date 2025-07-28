// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagment/Containers/BPF_FastArray.h"
#include "AC_PlugInv_InventoryComponent.generated.h"

class UPlugInv_ItemComponent;
class UPlugInv_InventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UPlugInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UPlugInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties.
	UPlugInv_InventoryComponent();
	
	// Check in client-side and tell server, then it all replicates down to clients.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UPlugInv_ItemComponent* ItemComponent);

	// Server RPC (Client->Server).
	UFUNCTION(Server, Reliable, Category = "Inventory")
	void Server_AddNewItem(UPlugInv_ItemComponent* ItemComponent, int32 StackCount);

	// Server RPC (Client->Server).
	UFUNCTION(Server, Reliable, Category = "Inventory")
	void Server_AddStacksToItem(UPlugInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	// Adds Unreal SubObjects to replication.
	void AddSubObjToReplication(UObject* SubObject);
	
	// Manage inventory widget visibility.
	void ToggleInventoryMenu();

	// CRUD Events.
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FInventoryItemChange OnItemChanged;
	FNoRoomInInventory OnNoRoomInInventory;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Flag for inventory visibility.
	bool bInventoryMenuOpen;

	// Fast array container.
	UPROPERTY(Replicated)
	FPlugInv_InventoryFastArray InventoryList;
	
	// Weak ptr holding a ref back to player without affecting GB (Garbage Collection).
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	// Inventory menu widget class.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_InventoryBase> InventoryMenuClass;

	// Inventory menu widget.
	UPROPERTY()
	TObjectPtr<UPlugInv_InventoryBase> InventoryMenu;

	// Inventory widget visibility functions.
	void OpenInventoryMenu();
	void CloseInventoryMenu();
	
	// Creates the inventory widget.
	void ConstructInventory();
};
