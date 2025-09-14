// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Diegetic/Widgets/Dieg_Slot.h"
#include "Dieg_InventoryInputHandler.generated.h"


class UDieg_InventoryComponent;
class ADieg_Briefcase;
class ADieg_WorldItemActor;
class UDieg_3DInventoryComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class ADieg_PlayerController;

/**
 * @brief Delegate fired when an inventory is opened.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryOpened);

/**
 * @brief Delegate fired when an inventory is closed.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryClosed);

/**
 * @brief Delegate fired when dragging an item over an inventory.
 * 
 * @param InventoryInputHandler The input handler managing the drag operation
 * @param InventoryComponent3D The 3D inventory component being hovered
 * @param DraggedItem The item being dragged
 * @param GrabPoint The point where the item was grabbed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDragHoverInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, DraggedItem, FIntPoint, GrabPoint);

/**
 * @brief Delegate fired when dragging an item away from an inventory.
 * 
 * @param InventoryInputHandler The input handler managing the drag operation
 * @param InventoryComponent3D The 3D inventory component being unhovered
 * @param DraggedItem The item being dragged
 * @param GrabPoint The point where the item was grabbed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDragUnHoverInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, DraggedItem, FIntPoint, GrabPoint);

/**
 * @brief Delegate fired when hovering over an inventory.
 * 
 * @param InventoryComponent3D The 3D inventory component being hovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverInventory, UDieg_3DInventoryComponent*, InventoryComponent3D);

/**
 * @brief Delegate fired when unhovering an inventory.
 * 
 * @param InventoryComponent3D The 3D inventory component being unhovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnHoverInventory, UDieg_3DInventoryComponent*, InventoryComponent3D);

/**
 * @brief Delegate fired when hovering over a slot in an inventory.
 * 
 * @param InventoryComponent3D The 3D inventory component containing the slot
 * @param Slot The slot being hovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartHoverSlot, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when unhovering a slot in an inventory.
 * 
 * @param InventoryComponent3D The 3D inventory component containing the slot
 * @param Slot The slot being unhovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartUnHoverSlot, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when starting to drag an item from the world.
 * 
 * @param InventoryInputHandler The input handler managing the drag operation
 * @param DraggedItem The item being dragged
 * @param GrabPoint The point where the item was grabbed
 * @param WorldLocation The world location of the item
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FStartDragWorld, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DraggedItem, FIntPoint, GrabPoint, FVector, WorldLocation);

/**
 * @brief Delegate fired when starting to drag an item from an inventory.
 * 
 * @param InventoryInputHandler The input handler managing the drag operation
 * @param DraggedItem The item being dragged
 * @param GrabPoint The point where the item was grabbed
 * @param Coordinates The grid coordinates of the item
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FStartDragInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DraggedItem, FIntPoint, GrabPoint, FIntPoint, Coordinates);

/**
 * @brief Delegate fired when updating drag slot information.
 * 
 * @param IsValid Whether the drag operation is valid
 * @param InventoryInputHandler The input handler managing the drag operation
 * @param DraggedItem The item being dragged
 * @param RelevantSlots Array of slots that would be affected
 * @param Coordinates The current grid coordinates
 * @param Rotation The current rotation angle
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FUpdateDragSlot, bool, IsValid, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DraggedItem, const TArray<FIntPoint>&, RelevantSlots, FIntPoint, Coordinates, float, Rotation);

/**
 * @brief Delegate fired when dropping an item in the world.
 * 
 * @param InventoryInputHandler The input handler managing the drop operation
 * @param DroppedItem The item being dropped
 * @param DroppedLocation The world location where the item was dropped
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDropWorld, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DroppedItem, FVector, DroppedLocation);

/**
 * @brief Delegate fired when merging items.
 * 
 * @param InventoryInputHandler The input handler managing the merge operation
 * @param DroppedItem The item being dropped for merging
 * @param MergedActors Array of actors that were merged
 * @param OldQuantity The original quantity before merging
 * @param NewQuantity The new quantity after merging
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMergeItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DroppedItem, TArray<AActor*>, MergedActors, int32, OldQuantity, int32, NewQuantity);

/**
 * @brief Delegate fired when an item is consumed during a merge operation.
 * 
 * @param InventoryInputHandler The input handler managing the merge operation
 * @param InventoryComponent3D The 3D inventory component where the merge occurred
 * @param ConsumedActor The actor that was consumed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FConsumedItemInMerge, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, ConsumedActor);

/**
 * @brief Delegate fired when resetting a drag operation in the world.
 * 
 * @param InventoryInputHandler The input handler managing the reset operation
 * @param ResetItem The item being reset
 * @param ResetLocation The world location where the item was reset
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FResetDragWorld, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, ResetItem, FVector, ResetLocation);

/**
 * @brief Delegate fired when resetting a drag operation in an inventory.
 * 
 * @param InventoryInputHandler The input handler managing the reset operation
 * @param InventoryComponent3D The 3D inventory component where the reset occurred
 * @param RestItem The item being reset
 * @param ResetCoordinates The grid coordinates where the item was reset
 * @param ResetRotation The rotation angle where the item was reset
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FResetDragInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, RestItem, FIntPoint, ResetCoordinates, float, ResetRotation);

/**
 * @brief Delegate fired when dropping an item in an inventory.
 * 
 * @param InventoryInputHandler The input handler managing the drop operation
 * @param InventoryComponent3D The 3D inventory component where the item was dropped
 * @param DroppedItem The item being dropped
 * @param DroppedCoordinates The grid coordinates where the item was dropped
 * @param DroppedRotation The rotation angle of the dropped item
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FDropInInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, DroppedItem, FIntPoint, DroppedCoordinates, float, DroppedRotation);

/**
 * @brief Delegate fired when hovering over an item.
 * 
 * @param InventoryInputHandler The input handler managing the hover operation
 * @param InventoryComponent3D The 3D inventory component containing the item
 * @param HoveredItem The item being hovered
 * @param IsInInventory Whether the item is currently in an inventory
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, HoveredItem, bool, IsInInventory);

/**
 * @brief Delegate fired when unhovering an item.
 * 
 * @param InventoryInputHandler The input handler managing the unhover operation
 * @param InventoryComponent3D The 3D inventory component containing the item
 * @param UnHoveredItem The item being unhovered
 * @param IsInInventory Whether the item is currently in an inventory
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FUnHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, UnHoveredItem, bool, IsInInventory);

/**
 * @brief Delegate fired when rotating an item.
 * 
 * @param InventoryInputHandler The input handler managing the rotation operation
 * @param RotatedItem The item being rotated
 * @param NewRotation The new rotation angle
 * @param GripPoint The point where the item is being gripped
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRotateItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, RotatedItem, float, NewRotation, FIntPoint, GripPoint);

/**
 * @brief Input handling component for the diegetic inventory system.
 * 
 * UDieg_InventoryInputHandler manages all input interactions for the diegetic inventory system,
 * including drag-and-drop operations, item rotation, inventory opening/closing, and hover states.
 * It serves as the central coordinator for all inventory-related input events and delegates.
 * 
 * The component handles:
 * - Mouse and touch input for inventory interactions
 * - Drag-and-drop operations between inventories and the world
 * - Item rotation and placement validation
 * - Hover states and visual feedback
 * - Input mapping context management
 * - Camera control during inventory operations
 * 
 * @note This component requires proper setup with input actions and mapping contexts.
 * @note This component is designed to work with UDieg_3DInventoryComponent and ADieg_PlayerController.
 * 
 * @see UDieg_3DInventoryComponent
 * @see ADieg_PlayerController
 * @see UDieg_InventoryComponent
 * @see ADieg_WorldItemActor
 * 
 * @since 1.0
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UDieg_InventoryInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Timer for trace operations.
	 * 
	 * Tracks the time elapsed since the last trace operation.
	 * Used to control the frequency of trace updates.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler", meta = (AllowPrivateAccess = "true"))
	float TraceTimer = 0.0f;

	/**
	 * @brief Interval between trace operations.
	 * 
	 * The time interval (in seconds) between consecutive trace operations.
	 * Controls the update frequency for input detection.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler", meta = (AllowPrivateAccess = "true"))
	float TraceInterval = 0.05f; 

	/**
	 * @brief Whether to enable debug logging.
	 * 
	 * When enabled, this component will output debug information
	 * to the log for troubleshooting purposes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler", meta = (AllowPrivateAccess = "true"))
	bool bDebugLogs{false};
	
	/**
	 * @brief Whether the inventory is currently open.
	 * 
	 * Tracks the current state of the inventory interface.
	 * Used for state management and UI updates.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bIsInventoryOpen{false};

	/**
	 * @brief Class for the briefcase actor.
	 * 
	 * The class to instantiate when creating the briefcase
	 * actor for the diegetic inventory system.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADieg_Briefcase> BriefCaseActorClass;
	
	/**
	 * @brief Reference to the briefcase actor.
	 * 
	 * The actual briefcase actor instance used for the
	 * diegetic inventory system.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Strong References", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADieg_Briefcase> BriefcaseActor;

	/**
	 * @brief Relative spawn location for the briefcase.
	 * 
	 * The relative position where the briefcase actor should be spawned
	 * in relation to the owning actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Briefcase Spawn", meta = (AllowPrivateAccess = "true"))
	FVector RelativeSpawnLocation{0.0f, 0.0, 0.0f};

	/**
	 * @brief Relative spawn rotation for the briefcase.
	 * 
	 * The relative rotation for the briefcase actor when spawned
	 * in relation to the owning actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Briefcase Spawn", meta = (AllowPrivateAccess = "true"))
	FRotator RelativeSpawnRotation{0.0f, 0.0, 0.0f};
	
	/**
	 * @brief Input action for toggling inventory.
	 * 
	 * The input action that triggers opening and closing
	 * of the inventory interface.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Actions", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleInventoryIA;

	/**
	 * @brief Input action for rotating items.
	 * 
	 * The input action that triggers item rotation
	 * during drag operations.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Actions", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateIA;

	/**
	 * @brief Input action for drag operations.
	 * 
	 * The input action that triggers drag and drop
	 * operations for inventory items.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Actions", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DragIA;
	
	/**
	 * @brief Default input mapping context.
	 * 
	 * The input mapping context that defines how input actions
	 * are mapped to player input for inventory operations.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Mapping Contexts", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultIMCs;
	
	/**
	 * @brief Weak reference to the owning player controller.
	 * 
	 * Reference to the player controller that owns this input handler.
	 * Used for input processing and component coordination.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_PlayerController> OwningPlayerController;

	/**
	 * @brief Weak reference to the player camera component.
	 * 
	 * Reference to the camera component used for viewport-to-world
	 * projection and input ray casting.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UCameraComponent> PlayerCameraComponent;

	/**
	 * @brief Weak reference to the currently hovering inventory component.
	 * 
	 * Reference to the 3D inventory component that the mouse is currently
	 * hovering over during input operations.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_3DInventoryComponent> HoveringInventoryComponent3D;

	/**
	 * @brief Weak reference to the currently hovering item.
	 * 
	 * Reference to the world item actor that the mouse is currently
	 * hovering over during input operations.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_WorldItemActor> HoveringItem;

	/**
	 * @brief Weak reference to the currently dragging item.
	 * 
	 * Reference to the world item actor that is currently being
	 * dragged during drag and drop operations.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_WorldItemActor> DraggingItem;

	/**
	 * @brief Weak reference to the last hit actor.
	 * 
	 * Reference to the last actor that was hit during trace operations.
	 * Used for input processing and interaction detection.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> LastHitActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint RelativeCoordinates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	float ValidRotation{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	float CurrentRotation{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FVector ValidWorldLocation{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint ValidCoordinates{-1, -1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint CurrentMouseCoordinates{-1, -1};

	// Only for visuals
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_Slot> CurrentMouseSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_3DInventoryComponent> ValidInventory3D;

	// Used for default return to origin cases
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_3DInventoryComponent> Default3DInventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Input Handler|Properties", meta = (AllowPrivateAccess = "true"))
	bool bDebugDraw{false};
	
	// Max distance for interaction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Input Handler|Properties", meta = (AllowPrivateAccess = "true"))
	double TraceLength{500.0};

	// Trace channel enabled for interactions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Input Handler|Properties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<ECollisionChannel>> TraceChannels;

	/**
	 * @brief Default constructor for the inventory input handler.
	 * 
	 * Initializes the component with default values and prepares it
	 * for input handling operations.
	 */
	UDieg_InventoryInputHandler();

	/**
	 * @brief Delegate fired when the inventory is opened.
	 * 
	 * This delegate is triggered when the inventory interface is opened.
	 * Bind to this delegate to respond to inventory opening events.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FInventoryOpened OnInventoryOpened;

	/**
	 * @brief Delegate fired when the inventory is closed.
	 * 
	 * This delegate is triggered when the inventory interface is closed.
	 * Bind to this delegate to respond to inventory closing events.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FInventoryClosed OnInventoryClosed;

	/**
	 * @brief Delegate fired when dragging an item over an inventory.
	 * 
	 * This delegate is triggered when dragging an item over a 3D inventory.
	 * Provides information about the drag operation and target inventory.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDragHoverInventory OnDragHoverInventory;

	/**
	 * @brief Delegate fired when dragging an item away from an inventory.
	 * 
	 * This delegate is triggered when dragging an item away from a 3D inventory.
	 * Provides information about the drag operation and source inventory.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDragUnHoverInventory OnDragUnHoverInventory;

	/**
	 * @brief Delegate fired when hovering over an inventory.
	 * 
	 * This delegate is triggered when the mouse hovers over a 3D inventory.
	 * Provides information about the hovered inventory component.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FHoverInventory OnHoverInventory;

	/**
	 * @brief Delegate fired when unhovering an inventory.
	 * 
	 * This delegate is triggered when the mouse stops hovering over a 3D inventory.
	 * Provides information about the unhovered inventory component.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FUnHoverInventory OnUnHoverInventory;

	/**
	 * @brief Delegate fired when starting to drag an item from the world.
	 * 
	 * This delegate is triggered when starting to drag an item from the 3D world.
	 * Provides information about the dragged item and drag operation.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartDragWorld OnStartDragWorld;

	/**
	 * @brief Delegate fired when starting to drag an item from an inventory.
	 * 
	 * This delegate is triggered when starting to drag an item from a 3D inventory.
	 * Provides information about the dragged item and source inventory.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartDragInventory OnStartDragInventory;

	/**
	 * @brief Delegate fired when starting to hover over a slot.
	 * 
	 * This delegate is triggered when the mouse starts hovering over an inventory slot.
	 * Provides information about the hovered slot and inventory.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartHoverSlot OnStartHoverSlot;

	/**
	 * @brief Delegate fired when stopping hover over a slot.
	 * 
	 * This delegate is triggered when the mouse stops hovering over an inventory slot.
	 * Provides information about the unhovered slot and inventory.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartUnHoverSlot OnStartUnHoverSlot;

	/**
	 * @brief Delegate fired when updating drag slot information.
	 * 
	 * This delegate is triggered during drag operations to update slot information.
	 * Provides current drag state and slot coordinates.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FUpdateDragSlot OnUpdateDragSlot;

	/**
	 * @brief Delegate fired when dropping an item in the world.
	 * 
	 * This delegate is triggered when dropping a dragged item in the 3D world.
	 * Provides information about the dropped item and drop location.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDropWorld OnDropWorld;

	/**
	 * @brief Delegate fired when merging items.
	 * 
	 * This delegate is triggered when two items are merged together.
	 * Provides information about the merged items and result.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FMergeItem OnMergeItem;

	/**
	 * @brief Delegate fired when an item is consumed during merge.
	 * 
	 * This delegate is triggered when an item is consumed as part of a merge operation.
	 * Provides information about the consumed item and merge context.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FConsumedItemInMerge OnConsumedItemInMerge;

	/**
	 * @brief Delegate fired when resetting drag from world.
	 * 
	 * This delegate is triggered when resetting a drag operation from the world.
	 * Provides information about the reset operation and item.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FResetDragWorld OnResetDragWorld;

	/**
	 * @brief Delegate fired when resetting drag from inventory.
	 * 
	 * This delegate is triggered when resetting a drag operation from an inventory.
	 * Provides information about the reset operation and source inventory.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FResetDragInventory OnResetDragInventory;

	/**
	 * @brief Delegate fired when dropping an item in an inventory.
	 * 
	 * This delegate is triggered when dropping a dragged item into a 3D inventory.
	 * Provides information about the dropped item and target inventory.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDropInInventory OnDropInInventory;

	/**
	 * @brief Delegate fired when hovering over an item.
	 * 
	 * This delegate is triggered when the mouse hovers over a world item actor.
	 * Provides information about the hovered item and input handler.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FHoverItem OnHoverItem;

	/**
	 * @brief Delegate fired when unhovering an item.
	 * 
	 * This delegate is triggered when the mouse stops hovering over a world item actor.
	 * Provides information about the unhovered item and input handler.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FUnHoverItem OnUnHoverItem;

	/**
	 * @brief Delegate fired when rotating an item.
	 * 
	 * This delegate is triggered when an item is rotated during drag operations.
	 * Provides information about the rotated item and new rotation angle.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FRotateItem OnRotateItem;
protected:
	/**
	 * @brief Begin play for the component.
	 * 
	 * Called when the game starts. Initializes the input handler
	 * and sets up input bindings and component references.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Initialize the input mapping context subsystem.
	 * 
	 * Sets up the enhanced input subsystem and adds the default
	 * input mapping context for inventory operations.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void InitializeImcSubsystem();
	
	/**
	 * @brief Toggle the inventory state.
	 * 
	 * Switches between opening and closing the inventory interface.
	 * Updates the inventory state and triggers appropriate delegates.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void ToggleInventory();
	
	/**
	 * @brief Open the user interface.
	 * 
	 * Opens the inventory user interface and updates the inventory state.
	 * Called when the inventory should be displayed to the player.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void OpenUserInterface();
	
	/**
	 * @brief Close the user interface.
	 * 
	 * Closes the inventory user interface and updates the inventory state.
	 * Called when the inventory should be hidden from the player.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void CloseUserInterface();
	
	/**
	 * @brief Perform a line trace from the mouse position.
	 * 
	 * Casts a ray from the mouse position into the 3D world to detect
	 * objects that can be interacted with.
	 * 
	 * @param HitResult Output parameter containing hit information
	 * @return True if something was hit, false otherwise
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	bool LineTraceFromMouse(FHitResult& HitResult) const;
	
	/**
	 * @brief Start hovering over an inventory component.
	 * 
	 * Called when the mouse starts hovering over a 3D inventory component.
	 * Updates hover state and triggers appropriate delegates.
	 * 
	 * @param NewHoveringInventory The inventory component being hovered
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StartHoverInventory(UDieg_3DInventoryComponent* NewHoveringInventory);
	
	/**
	 * @brief Stop hovering over an inventory component.
	 * 
	 * Called when the mouse stops hovering over a 3D inventory component.
	 * Clears hover state and triggers appropriate delegates.
	 * 
	 * @param HoveringInventory The inventory component being unhovered
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StopHoverInventory(UDieg_3DInventoryComponent* HoveringInventory);
	
	/**
	 * @brief Start hovering over a world item actor.
	 * 
	 * Called when the mouse starts hovering over a world item actor.
	 * Updates hover state and triggers appropriate delegates.
	 * 
	 * @param NewHoveringWorldItem The item actor being hovered
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StartHoverItem(ADieg_WorldItemActor* NewHoveringWorldItem);
	
	/**
	 * @brief Stop hovering over a world item actor.
	 * 
	 * Called when the mouse stops hovering over a world item actor.
	 * Clears hover state and triggers appropriate delegates.
	 * 
	 * @param HoveringWorldItem The item actor being unhovered
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StopHoverItem(ADieg_WorldItemActor* HoveringWorldItem);
	
	/**
	 * @brief Handle trace hit results.
	 * 
	 * Processes the results of a trace operation and determines
	 * what actions to take based on what was hit.
	 * 
	 * @param HitResult The hit result from the trace
	 * @param bIsBlockingHit Whether the hit was blocking
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void HandleTraceHit(const FHitResult& HitResult, bool bIsBlockingHit);

	/**
	 * @brief Check if currently dragging an item.
	 * 
	 * Returns whether the input handler is currently in a drag operation.
	 * 
	 * @return True if dragging an item, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	bool IsDraggingItem() const { return DraggingItem.IsValid(); }
	
	/**
	 * @brief Check if currently hovering over an inventory.
	 * 
	 * Returns whether the mouse is currently hovering over a 3D inventory component.
	 * 
	 * @return True if hovering over an inventory, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	bool IsInInventory() const { return HoveringInventoryComponent3D.IsValid(); }
	
	/**
	 * @brief Check if an item is in an inventory.
	 * 
	 * Determines whether the specified item is currently in a 3D inventory component.
	 * 
	 * @param ItemIn The item to check
	 * @param Inventory3DOut Output parameter for the inventory containing the item
	 * @return True if the item is in an inventory, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	bool IsItemInInventory(const ADieg_WorldItemActor* ItemIn, UDieg_3DInventoryComponent*& Inventory3DOut);
	
	/**
	 * @brief Attempt to start dragging an item.
	 * 
	 * Initiates a drag operation with the currently hovered item.
	 * Called when the drag input action is triggered.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void TryDragItem();
	
	/**
	 * @brief Attempt to drop the currently dragged item.
	 * 
	 * Completes a drag operation by dropping the item at the current location.
	 * Called when the drag input action is released.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void TryDropItem();

	/**
	 * @brief Handle input for item rotation.
	 * 
	 * Processes input for rotating items during drag operations.
	 * Called when the rotate input action is triggered.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void HandleInputRotateItem();
	
	/**
	 * @brief Attempt to rotate the currently dragged item.
	 * 
	 * Rotates the currently dragged item by 90 degrees.
	 * Called during drag operations to change item orientation.
	 */
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void TryRotateItem();
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void InternalHandleInventorySlotHover(UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot);

	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void HandleHoveringInventorySlotHovered(const FGeometry& Geometry, const FPointerEvent& PointerEvent, UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot);
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void HandleHoveringInventorySlotUnHovered(const FPointerEvent& PointerEvent, UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot);
	
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StartDraggingItem();
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StopDraggingItem();
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	bool MergeItems(const TArray<FIntPoint>& RootSlotCoordinates);
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void RotateItem();
	// Connect dragging item to hovering inventory
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void ConnectItemToInventory() const;
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void DisconnectItemToInventory() const;

	// Where we clicked in local space (shape coordinates)
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	FIntPoint GetGrabCoordinates() const;
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	FIntPoint GetRotatedGrabCoordinates(bool UseValidated) const;
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	TArray<FIntPoint> GetRelevantCoordinates() const;
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	bool GetCurrentSlot(UDieg_Slot*& SlotOut) const;

public:
	/**
	 * @brief Tick the component.
	 * 
	 * Called every frame. Performs continuous input processing,
	 * trace operations, and updates the input handler state.
	 * 
	 * @param DeltaTime Time elapsed since last tick
	 * @param TickType Type of tick being performed
	 * @param ThisTickFunction The tick function for this component
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
