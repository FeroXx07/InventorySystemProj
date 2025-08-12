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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FPlugInv_SlotAvailabilityResult&, Result);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UPlugInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties.
	UPlugInv_InventoryComponent();

	virtual void PostInitProperties() override;
	
	// Check in client-side and tell server, then it all replicates down to clients.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UPlugInv_ItemComponent* ItemComponent);

	// Server RPC (Client->Server).
	UFUNCTION(Server, Reliable, Category = "Inventory")
	void Server_AddNewItem(UPlugInv_ItemComponent* ItemComponent, int32 StackCount);

	// Server RPC (Client->Server).
	UFUNCTION(Server, Reliable, Category = "Inventory")
	void Server_AddStacksToItem(UPlugInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	// Server RPC (Client->Server).
	UFUNCTION(Server, Reliable, Category = "Inventory")
	void Server_DropItem(UPlugInv_InventoryItem* Item, int32 StackCount);

	// Server RPC (Client->Server).
	UFUNCTION(Server, Reliable, Category = "Inventory")
	void Server_ConsumeItem(UPlugInv_InventoryItem* Item);
	
	// Adds Unreal SubObjects to replication.
	void AddSubObjToReplication(UObject* SubObject);
	
	// Manage inventory widget visibility.
	void ToggleInventoryMenu();

	void SpawnDroppedItem(UPlugInv_InventoryItem* Item, int32 StackCount) const;

	UPlugInv_InventoryBase* GetInventoryMenu() const { return InventoryMenu; }
	
	// CRUD Events.
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FInventoryItemChange OnItemChanged;
	FNoRoomInInventory OnNoRoomInInventory;
	FStackChange OnStackChange;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Flag for inventory visibility.
	bool bInventoryMenuOpen;

	// Fast array container.
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Inventory")
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

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMin = -85.f;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistMin = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistMax = 50.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float RelativeSpawnZOffset = -70.f;
};
