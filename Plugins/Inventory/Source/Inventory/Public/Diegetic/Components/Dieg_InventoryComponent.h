// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Diegetic/UStructs/Dieg_InventorySlot.h"
#include "Diegetic/UStructs/Dieg_PrePopulate.h"
#include "Dieg_InventoryComponent.generated.h"

/**
 * @brief Delegate fired when the inventory component has been fully initialized.
 * 
 * This delegate is broadcast after the inventory grid has been created, slots have been initialized,
 * and any prepopulate data has been processed. Use this to perform any setup that depends on
 * the inventory being ready.
 * 
 * @see UDieg_InventoryComponent::Initialize
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryInitialized);

/**
 * @brief Core inventory management component for the Diegetic inventory system.
 * 
 * The UDieg_InventoryComponent is the heart of the diegetic inventory system, providing a grid-based
 * inventory management solution with support for:
 * - Grid-based slot management with configurable dimensions
 * - Item placement with shape-based collision detection
 * - Item stacking and quantity management
 * - Rotation support for items
 * - Gameplay tag-based filtering and restrictions
 * - Prepopulation of items on initialization
 * 
 * The component maintains a 2D grid of inventory slots where items can be placed. Each item has a
 * defined shape that determines which slots it occupies. Items can be rotated and the system will
 * automatically find valid placement positions.
 * 
 * @note This component is designed to work with UDieg_ItemInstance objects and requires proper
 * initialization before use.
 * 
 * @see UDieg_ItemInstance
 * @see FDieg_InventorySlot
 * @see UDieg_3DInventoryComponent
 * 
 * @since 1.0
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UDieg_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor for the inventory component.
	 * 
	 * Initializes the component with default values. The inventory must be explicitly
	 * initialized using Initialize() before it can be used.
	 */
	UDieg_InventoryComponent();

	/**
	 * @brief Delegate fired when the inventory has been fully initialized.
	 * 
	 * Bind to this delegate to receive notifications when the inventory is ready for use.
	 * This is fired after Initialize() completes successfully.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Game|Dieg|InventoryComponent")
	FOnInventoryInitialized OnInventoryInitialized;

protected:
	/**
	 * @brief Called when the game starts.
	 * 
	 * Performs initial setup and validation of the inventory component.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Array of all inventory slots containing metadata and item references.
	 * 
	 * This is the core storage for the inventory system. Each slot contains information
	 * about its position, whether it's occupied, and what item (if any) is placed there.
	 * 
	 * @see FDieg_InventorySlot
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent|Storage", meta = (AllowPrivateAccess = "true"))
	TArray<FDieg_InventorySlot> InventorySlots;

	/**
	 * @brief Quick lookup map to check if a slot exists and whether it's occupied.
	 * 
	 * Maps grid coordinates to boolean values indicating occupation status.
	 * Used for fast collision detection during item placement.
	 * 
	 * @note This is automatically maintained by the inventory system.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent|Storage", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, bool> SlotsOccupation;

	/**
	 * @brief Default tags applied to all slots for filtering or item restrictions.
	 * 
	 * These tags are applied to every slot in the inventory and can be used to
	 * restrict certain types of items or provide additional metadata.
	 * 
	 * @see FGameplayTagContainer
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game|Dieg|InventoryComponent|Storage", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer SlotTags;

	/**
	 * @brief Preferred rotation when placing items (in degrees).
	 * 
	 * When the system attempts to place an item, it will try this rotation first
	 * before attempting other rotations. Range: -90.0 to 180.0 degrees.
	 * 
	 * @note This is a hint - the system may use different rotations if the preferred
	 * rotation doesn't result in a valid placement.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game|Dieg|InventoryComponent|Storage",
		meta=(ClampMin="-90.0", ClampMax="180.0", UIMin="-90.0", UIMax="180.0", AllowPrivateAccess = "true"))
	float ItemRotationPriority{90.0};

	/**
	 * @brief Total number of slots in the inventory grid.
	 * 
	 * Defines the maximum capacity of the inventory. The actual grid size is
	 * determined by TotalSlots and MaxColumns.
	 * 
	 * @see MaxColumns
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent|Setup", meta = (AllowPrivateAccess = "true"))
	int32 TotalSlots{35};
	
	/**
	 * @brief Number of columns in the inventory grid.
	 * 
	 * Used to calculate slot coordinates and determine the grid layout.
	 * The number of rows is calculated as TotalSlots / MaxColumns.
	 * 
	 * @see TotalSlots
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent|Setup", meta = (AllowPrivateAccess = "true"))
	int32 MaxColumns{10};

	/**
	 * @brief Items to prepopulate in the inventory on initialization.
	 * 
	 * Array of prepopulate data that will be processed during Initialize().
	 * Each entry defines an item type and quantity to add to the inventory.
	 * 
	 * @see FDieg_PrePopulate
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent|Setup", meta = (AllowPrivateAccess = "true"))
	TArray<FDieg_PrePopulate> PrePopulateData;

	/**
	 * @brief Enable debug logging for inventory operations.
	 * 
	 * When enabled, the inventory component will log detailed information about
	 * item placement, removal, and other operations for debugging purposes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent")
	bool bDebugLogs{false};
	
	/**
	 * @brief Flag indicating whether the inventory has been initialized.
	 * 
	 * Set to true after successful initialization. Used to prevent operations
	 * on uninitialized inventories.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent")
	bool bIsInitialized{false};

public:
	/**
	 * @brief Called every frame.
	 * 
	 * @param DeltaTime Time elapsed since last frame
	 * @param TickType Type of tick being performed
	 * @param ThisTickFunction Function being ticked
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * @brief Initializes the inventory with specified dimensions and processes prepopulate data.
	 * 
	 * This method must be called before the inventory can be used. It creates the slot grid,
	 * initializes all slots, and processes any prepopulate data to add initial items.
	 * 
	 * @param NumSlots Total number of slots in the inventory grid
	 * @param NumColumns Number of columns in the grid (determines layout)
	 * @param Tags Gameplay tags to apply to all slots
	 * 
	 * @note This method will fire the OnInventoryInitialized delegate upon successful completion.
	 * 
	 * @see OnInventoryInitialized
	 * @see PrePopulateData
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	void Initialize(int32 NumSlots, int32 NumColumns, const FGameplayTagContainer& Tags);

	/**
	 * @brief Attempts to add an item to the inventory with automatic stacking.
	 * 
	 * This method will first try to stack the item with existing items of the same type,
	 * then attempt to place it in empty slots if stacking is not possible or complete.
	 * 
	 * @param ItemToAdd The item instance to add to the inventory
	 * @param Remaining [Out] The quantity that could not be added (0 if fully added)
	 * @return true if any quantity was successfully added, false otherwise
	 * 
	 * @note The item instance is not consumed - the caller retains ownership.
	 * @see CanAddItem
	 * @see AddQuantityToSlot
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool TryAddItem(UDieg_ItemInstance* ItemToAdd, int32& Remaining);

	/**
	 * @brief Attempts to remove an item from the inventory.
	 * 
	 * Searches for the specified item instance and removes it from the inventory,
	 * clearing all occupied slots and updating the occupation map.
	 * 
	 * @param ItemToRemove The item instance to remove
	 * @return true if the item was found and removed, false otherwise
	 * 
	 * @note The item instance is not destroyed - the caller gains ownership.
	 * @see CanRemoveItem
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool TryRemoveItem(UDieg_ItemInstance* ItemToRemove);

	/**
	 * @brief Checks if an item can be added to the inventory.
	 * 
	 * Performs a comprehensive check to determine if there's space for the item,
	 * considering both stacking opportunities and available empty slots.
	 * 
	 * @param ItemToAdd The item instance to check
	 * @return true if the item can be added, false otherwise
	 * 
	 * @see TryAddItem
	 * @see AreSlotsAvailable
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Inventory omponent")
	bool CanAddItem(const UDieg_ItemInstance* ItemToAdd);

	/**
	 * @brief Checks if an item can be removed from the inventory.
	 * 
	 * Searches for the specified item instance to determine if it exists in the inventory.
	 * 
	 * @param ItemToRemove The item instance to check
	 * @return true if the item exists and can be removed, false otherwise
	 * 
	 * @see TryRemoveItem
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool CanRemoveItem(UDieg_ItemInstance* ItemToRemove);

	/**
	 * @brief Returns true if inventory contains at least one instance of the item.
	 * 
	 * Performs a quick check to see if any slots contain items of the same type
	 * as the provided item instance.
	 * 
	 * @param ItemToCheck The item instance to search for
	 * @return true if at least one matching item is found, false otherwise
	 * 
	 * @see FindRootSlotByItemType
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool DoesInventoryContainItem(const UDieg_ItemInstance* ItemToCheck) { return FindRootSlotByItemType(ItemToCheck).Num() > 0; }

	/**
	 * @brief Returns all root slots containing items equal to the provided instance (C++ only).
	 * 
	 * Searches through all inventory slots to find root slots that contain items
	 * of the same type as the provided item instance. Returns pointers for efficiency.
	 * 
	 * @param ItemToCheck The item instance to search for
	 * @return Array of pointers to root slots containing matching items
	 * 
	 * @see FindRootSlotByItemTypeBP
	 */
	TArray<FDieg_InventorySlot*> FindRootSlotByItemType(const UDieg_ItemInstance* ItemToCheck);
	
	/**
	 * @brief Returns all root slots containing items equal to the provided instance (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns structs by value instead of pointers.
	 * 
	 * @param ItemToCheck The item instance to search for
	 * @return Array of root slots containing matching items
	 * 
	 * @see FindRootSlotByItemType
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	TArray<FDieg_InventorySlot> FindRootSlotByItemTypeBP(const UDieg_ItemInstance* ItemToCheck);

	/**
	 * @brief Returns all root slots containing exactly the provided instance (C++ only).
	 * 
	 * Searches for slots containing the exact same item instance (not just same type).
	 * Returns pointers for efficiency.
	 * 
	 * @param ItemToCheck The exact item instance to search for
	 * @return Array of pointers to root slots containing the exact item
	 * 
	 * @see FindRootSlotByInstanceBP
	 */
	TArray<FDieg_InventorySlot*> FindRootSlotByInstance(const UDieg_ItemInstance* ItemToCheck);
	
	/**
	 * @brief Returns all root slots containing exactly the provided instance (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns structs by value instead of pointers.
	 * 
	 * @param ItemToCheck The exact item instance to search for
	 * @return Array of root slots containing the exact item
	 * 
	 * @see FindRootSlotByInstance
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	TArray<FDieg_InventorySlot> FindRootSlotByInstanceBP(const UDieg_ItemInstance* ItemToCheck);

	/**
	 * @brief Gets a root slot at the specified coordinates (C++ only).
	 * 
	 * Returns a pointer to the root slot at the given coordinates for efficient access.
	 * 
	 * @param SlotCoordinates The grid coordinates to query
	 * @return Pointer to the root slot, or nullptr if not found
	 * 
	 * @see GetRootSlotBP
	 */
	FDieg_InventorySlot* GetRootSlot(const FIntPoint& SlotCoordinates);
	
	/**
	 * @brief Gets a root slot at the specified coordinates (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns a struct by value.
	 * 
	 * @param SlotCoordinates The grid coordinates to query
	 * @return The root slot struct, or empty slot if not found
	 * 
	 * @see GetRootSlot
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	FDieg_InventorySlot GetRootSlotBP(const FIntPoint& SlotCoordinates);
	
	/**
	 * @brief Gets any slot at the specified coordinates (C++ only).
	 * 
	 * Returns a pointer to any slot at the given coordinates, regardless of whether
	 * it's a root slot or part of a multi-slot item.
	 * 
	 * @param SlotCoordinates The grid coordinates to query
	 * @return Pointer to the slot, or nullptr if not found
	 * 
	 * @see GetSlotBP
	 */
	FDieg_InventorySlot* GetSlot(const FIntPoint& SlotCoordinates);
	
	/**
	 * @brief Gets any slot at the specified coordinates (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns a struct by value.
	 * 
	 * @param SlotCoordinates The grid coordinates to query
	 * @return The slot struct, or empty slot if not found
	 * 
	 * @see GetSlot
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	FDieg_InventorySlot GetSlotBP(const FIntPoint& SlotCoordinates);
	
	/**
	 * @brief Gets all root slots in the inventory (C++ only).
	 * 
	 * Returns pointers to all root slots for efficient iteration and modification.
	 * 
	 * @return Array of pointers to all root slots
	 * 
	 * @see GetRootSlotsMutableBP
	 */
	TArray<FDieg_InventorySlot*> GetRootSlotsMutable();
	
	/**
	 * @brief Gets all root slots in the inventory (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns structs by value.
	 * 
	 * @return Array of all root slots
	 * 
	 * @see GetRootSlotsMutable
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	TArray<FDieg_InventorySlot> GetRootSlotsMutableBP();
	
	/**
	 * @brief Gets all slots in the inventory (C++ only).
	 * 
	 * Returns pointers to all slots, including both root and non-root slots.
	 * 
	 * @return Array of pointers to all slots
	 * 
	 * @see GetSlotsMutableBP
	 */
	TArray<FDieg_InventorySlot*> GetSlotsMutable();
	
	/**
	 * @brief Gets all slots in the inventory (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns structs by value.
	 * 
	 * @return Array of all slots
	 * 
	 * @see GetSlotsMutable
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	TArray<FDieg_InventorySlot> GetSlotsMutableBP();
	
	/**
	 * @brief Gets the total number of slots in the inventory.
	 * 
	 * @return Total number of slots
	 * 
	 * @see MaxColumns
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	int32 GetTotalSlots() const { return TotalSlots; };
	
	/**
	 * @brief Gets the number of columns in the inventory grid.
	 * 
	 * @return Number of columns
	 * 
	 * @see TotalSlots
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	int32 GetMaxColumns() const { return MaxColumns; };

	/**
	 * @brief Computes all coordinates an item would occupy based on root, shape, and rotation.
	 * 
	 * This static utility function calculates which grid coordinates would be occupied
	 * by an item with the given shape when placed at the specified root coordinates
	 * with the given rotation.
	 * 
	 * @param SlotCoordinates The root coordinates where the item would be placed
	 * @param Shape The shape definition of the item
	 * @param ShapeRoot The root point of the shape (usually {0,0})
	 * @param Rotation The rotation angle in degrees
	 * @param RootSlotOut [Out] The actual root slot coordinates after rotation
	 * @return Array of all coordinates that would be occupied
	 * 
	 * @note This is a static utility function that doesn't modify the inventory.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	static TArray<FIntPoint> GetRelevantCoordinates(const FIntPoint& SlotCoordinates, const TArray<FIntPoint>& Shape, const FIntPoint& ShapeRoot, float Rotation, FIntPoint& RootSlotOut);
	
	/**
	 * @brief Returns all inventory root slot coordinates containing items overlapping the given shape.
	 * 
	 * Searches for existing items that would overlap with the specified shape coordinates
	 * and returns the root coordinates of those items.
	 * 
	 * @param ShapeCoordinates The coordinates to check for overlaps
	 * @param Object The item instance to check against (for type matching)
	 * @return Array of root slot coordinates that overlap with the shape
	 * 
	 * @see GetRelevantCoordinates
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	TArray<FIntPoint> GetRelevantItems(const TArray<FIntPoint>& ShapeCoordinates, const UDieg_ItemInstance* Object);

	/**
	 * @brief Checks if all slots in the shape are free; optionally ignores some slots.
	 * 
	 * Performs collision detection to determine if the specified shape can be placed
	 * without overlapping existing items. Can ignore specific slots (useful for
	 * replacing existing items).
	 * 
	 * @param InputShape The shape coordinates to check
	 * @param Ignore Array of coordinates to ignore during collision detection
	 * @return true if all slots are available, false otherwise
	 * 
	 * @see AreSlotsAvailableSimple
	 * @see CanAddItemToSlot
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool AreSlotsAvailable(const TArray<FIntPoint>& InputShape, const TArray<FIntPoint>& Ignore);

	/**
	 * @brief Checks if all slots in the shape are free (simple version).
	 * 
	 * Simplified version that doesn't ignore any slots. Equivalent to calling
	 * AreSlotsAvailable with an empty ignore array.
	 * 
	 * @param InputShape The shape coordinates to check
	 * @return true if all slots are available, false otherwise
	 * 
	 * @see AreSlotsAvailable
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool AreSlotsAvailableSimple(const TArray<FIntPoint>& InputShape);

	/**
	 * @brief Adds quantity to an existing stackable slot; returns amount added.
	 * 
	 * Attempts to add the specified quantity to existing stackable items of the same type.
	 * This is used internally by TryAddItem for stacking functionality.
	 * 
	 * @param ItemToAdd The item instance to stack
	 * @param QuantityIn The quantity to add
	 * @return The actual amount that was added (may be less than requested)
	 * 
	 * @see TryAddItem
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	int32 AddQuantityToSlot(const UDieg_ItemInstance* ItemToAdd, int32 QuantityIn);

	/**
	 * @brief Places an item in inventory starting at given slot with rotation (C++ only).
	 * 
	 * Directly places an item at the specified coordinates with the given rotation.
	 * Returns a pointer to the root slot for efficiency.
	 * 
	 * @param ItemToAdd The item instance to place
	 * @param SlotCoordinates The root coordinates where to place the item
	 * @param RotationUsed The rotation angle to use
	 * @return Pointer to the root slot where the item was placed, or nullptr if failed
	 * 
	 * @see AddItemToInventoryBP
	 * @see CanAddItemToSlot
	 */
	FDieg_InventorySlot* AddItemToInventory(UDieg_ItemInstance* ItemToAdd, const FIntPoint& SlotCoordinates, float RotationUsed);
	
	/**
	 * @brief Places an item in inventory starting at given slot with rotation (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns a struct by value.
	 * 
	 * @param ItemToAdd The item instance to place
	 * @param SlotCoordinates The root coordinates where to place the item
	 * @param RotationUsed The rotation angle to use
	 * @return The root slot where the item was placed, or empty slot if failed
	 * 
	 * @see AddItemToInventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	FDieg_InventorySlot AddItemToInventoryBP(UDieg_ItemInstance* ItemToAdd, const FIntPoint& SlotCoordinates, float RotationUsed);

	/**
	 * @brief Removes an item from inventory, clearing all occupied slots (C++ only).
	 * 
	 * Searches for and removes the specified item instance, clearing all slots it occupies.
	 * Returns a pointer to the root slot for efficiency.
	 * 
	 * @param ItemToRemove The item instance to remove
	 * @return Pointer to the root slot that was cleared, or nullptr if not found
	 * 
	 * @see RemoveItemFromInventoryBP
	 * @see TryRemoveItem
	 */
	FDieg_InventorySlot* RemoveItemFromInventory(UDieg_ItemInstance* ItemToRemove);
	
	/**
	 * @brief Removes an item from inventory, clearing all occupied slots (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns a struct by value.
	 * 
	 * @param ItemToRemove The item instance to remove
	 * @return The root slot that was cleared, or empty slot if not found
	 * 
	 * @see RemoveItemFromInventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	FDieg_InventorySlot RemoveItemFromInventoryBP(UDieg_ItemInstance* ItemToRemove);

	/**
	 * @brief Checks if a given slot can accept the item shape (with optional rotation).
	 * 
	 * Performs comprehensive validation to determine if an item can be placed at the
	 * specified coordinates, considering shape, rotation, and existing items.
	 * 
	 * @param SlotCoordinates The root coordinates to check
	 * @param ItemShape The shape definition of the item
	 * @param ItemShapeRoot The root point of the shape
	 * @param RotationUsedOut [Out] The rotation that would be used for placement
	 * @return true if the item can be placed, false otherwise
	 * 
	 * @see CanAddItemInstanceToSlot
	 * @see AreSlotsAvailable
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool CanAddItemToSlot(const FIntPoint& SlotCoordinates, const TArray<FIntPoint>& ItemShape, const FIntPoint& ItemShapeRoot, int32& RotationUsedOut);

	/**
	 * @brief Checks if a given slot can accept the item instance (with rotation).
	 * 
	 * Validates placement of a specific item instance at the given coordinates
	 * with the specified rotation.
	 * 
	 * @param SlotCoordinates The root coordinates to check
	 * @param ItemInstance The item instance to validate
	 * @param Rotation The rotation angle to check
	 * @return true if the item can be placed, false otherwise
	 * 
	 * @see CanAddItemToSlot
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool CanAddItemInstanceToSlot(const FIntPoint& SlotCoordinates, UDieg_ItemInstance* ItemInstance, int32 Rotation);

	/**
	 * @brief Returns true if a coordinate is outside the inventory bounds.
	 * 
	 * @param SlotPoint The coordinates to check
	 * @return true if the coordinates are outside the valid range, false otherwise
	 * 
	 * @see IsSlotPointOccupied
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool IsSlotPointOutOfBounds(const FIntPoint& SlotPoint); 

	/**
	 * @brief Returns true if a coordinate is currently occupied.
	 * 
	 * @param SlotPoint The coordinates to check
	 * @return true if the slot is occupied, false otherwise
	 * 
	 * @see IsSlotPointOutOfBounds
	 * @see SlotsOccupation
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool IsSlotPointOccupied(const FIntPoint& SlotPoint);

	/**
	 * @brief Creates a new item instance from prepopulate data.
	 * 
	 * Factory method that creates and initializes an item instance based on
	 * prepopulate data. Used during inventory initialization.
	 * 
	 * @param PrePopData The prepopulate data to use
	 * @return New item instance, or nullptr if creation failed
	 * 
	 * @see PrePopulateData
	 * @see Initialize
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	UDieg_ItemInstance* MakeInstanceFromPrePopulateData(const FDieg_PrePopulate& PrePopData);
};
