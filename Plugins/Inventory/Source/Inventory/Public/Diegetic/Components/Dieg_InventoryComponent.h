// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Diegetic/UStructs/Dieg_InventorySlot.h"
#include "Diegetic/UStructs/Dieg_PrePopulate.h"
#include "Dieg_InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryInitialized);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UDieg_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDieg_InventoryComponent();

	UPROPERTY(BlueprintAssignable)
	FOnInventoryInitialized OnInventoryInitialized;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	bool bDebugLogs{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	bool bIsInitialized{false};

	// All inventory slots containing metadata and item references
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component|Storage")
	TArray<FDieg_InventorySlot> InventorySlots;

	// Quick lookup map to check if a slot exists and whether it's occupied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Component|Storage")
	TMap<FIntPoint, bool> SlotsOccupation;

	// Default tags applied to all slots (e.g., for filtering or item restrictions)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Component|Storage")
	FGameplayTagContainer SlotTags;

	// Preferred rotation when placing items (degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Component|Storage",
		meta=(ClampMin="-90.0", ClampMax="180.0", UIMin="-90.0", UIMax="180.0"))
	float ItemRotationPriority{90.0};

	// Total number of slots in the inventory grid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component|Setup")
	int32 TotalSlots{35};
	
	// Number of columns in the inventory grid (used to calculate slot coordinates)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component|Setup")
	int32 MaxColumns{10};

	// Items to prepopulate in the inventory on initialization
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component|Setup")
	TArray<FDieg_PrePopulate> PrePopulateData;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	// Initializes slots and prepopulates items
	void Initialize(int32 NumSlots, int32 NumColumns, const FGameplayTagContainer& Tags);

	// Attempts to add an item; updates Remaining quantity
	bool TryAddItem(UDieg_ItemInstance* ItemToAdd, int32& Remaining);

	// Attempts to remove an item.
	bool TryRemoveItem(UDieg_ItemInstance* ItemToRemove);

	// Checks if an item can be added (stackable or empty slots)
	bool CanAddItem(const UDieg_ItemInstance* ItemToAdd);

	// Checks if an item can be removed (exists in inventory)
	bool CanRemoveItem(UDieg_ItemInstance* ItemToRemove);

	// Returns true if inventory contains at least one instance of the item
	bool DoesInventoryContainItem(const UDieg_ItemInstance* ItemToCheck) { return FindRootSlotByItemType(ItemToCheck).Num() > 0; }

	// Returns all root slots containing items equal to the provided instance
	TSet<FDieg_InventorySlot*> FindRootSlotByItemType(const UDieg_ItemInstance* ItemToCheck);

	// Returns all root slots containing exactly the provided instance
	TSet<FDieg_InventorySlot*> FindRootSlotByInstance(const UDieg_ItemInstance* ItemToCheck);

	TArray<FDieg_InventorySlot*> GetRootSlotsMutable();
	int32 GetTotalSlots() const { return TotalSlots; };
	int32 GetMaxColumns() const { return MaxColumns; };

	// Computes all coordinates the item would occupy based on root, shape, and rotation
	static TArray<FIntPoint> GetRelevantCoordinates(const FIntPoint& SlotCoordinates, const TArray<FIntPoint>& Shape, const FIntPoint& ShapeRoot, float Rotation, FIntPoint& RootSlotOut);
	
	// Returns all inventory slot indices containing items overlapping the given shape
	TArray<int32> GetRelevantItems(const TArray<FIntPoint>& ShapeCoordinates, const UDieg_ItemInstance* Object);

	// Checks if all slots in the shape are free; optionally ignores some slots
	bool AreSlotsAvailable(const TArray<FIntPoint>& InputShape, const TArray<FIntPoint>* Ignore = nullptr);
private:
	// Places an item in inventory starting at given slot with rotation; returns root slot
	const FDieg_InventorySlot* AddItemToInventory(UDieg_ItemInstance* ItemToAdd, const FIntPoint& SlotCoordinates, float RotationUsed);

	// Removes an item from inventory, clearing all occupied slots; returns root slot
	const FDieg_InventorySlot* RemoveItemFromInventory(UDieg_ItemInstance* ItemToRemove);

	// Adds quantity to an existing stackable slot; returns amount added
	int32 AddQuantityToSlot(const UDieg_ItemInstance* ItemToAdd, int32 QuantityIn);

	// Checks if a given slot can accept the item shape (with optional rotation)
	bool CanAddItemToSlot(const FIntPoint& SlotCoordinates, const TArray<FIntPoint>& ItemShape, const FIntPoint& ItemShapeRoot, int32& RotationUsedOut);

	// Returns true if a coordinate is outside the inventory bounds
	bool IsSlotPointOutOfBounds(const FIntPoint& SlotPoint); 

	// Returns true if a coordinate is currently occupied
	bool IsSlotPointOccupied(const FIntPoint& SlotPoint);

	// Creates a new item instance from prepopulate data
	UDieg_ItemInstance* MakeInstanceFromPrePopulateData(const FDieg_PrePopulate& PrePopData);
};
