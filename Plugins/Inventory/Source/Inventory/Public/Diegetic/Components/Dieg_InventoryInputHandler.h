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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler", meta = (AllowPrivateAccess = "true"))
	float TraceTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler", meta = (AllowPrivateAccess = "true"))
	float TraceInterval = 0.05f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler", meta = (AllowPrivateAccess = "true"))
	bool bDebugLogs{false};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bIsInventoryOpen{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADieg_Briefcase> BriefCaseActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Strong References", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADieg_Briefcase> BriefcaseActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Briefcase Spawn", meta = (AllowPrivateAccess = "true"))
	FVector RelativeSpawnLocation{0.0f, 0.0, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Briefcase Spawn", meta = (AllowPrivateAccess = "true"))
	FRotator RelativeSpawnRotation{0.0f, 0.0, 0.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Actions", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleInventoryIA;

	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Actions", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateIA;

	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Actions", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DragIA;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Inventory Input Handler|Input Mapping Contexts", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultIMCs;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_PlayerController> OwningPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UCameraComponent> PlayerCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_3DInventoryComponent> HoveringInventoryComponent3D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_WorldItemActor> HoveringItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Inventory Input Handler|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_WorldItemActor> DraggingItem;

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

	// Sets default values for this component's properties
	UDieg_InventoryInputHandler();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FInventoryOpened OnInventoryOpened;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FInventoryClosed OnInventoryClosed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDragHoverInventory OnDragHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDragUnHoverInventory OnDragUnHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FHoverInventory OnHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FUnHoverInventory OnUnHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartDragWorld OnStartDragWorld;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartDragInventory OnStartDragInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartHoverSlot OnStartHoverSlot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FStartUnHoverSlot OnStartUnHoverSlot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FUpdateDragSlot OnUpdateDragSlot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDropWorld OnDropWorld;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FMergeItem OnMergeItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FConsumedItemInMerge OnConsumedItemInMerge;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FResetDragWorld OnResetDragWorld;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FResetDragInventory OnResetDragInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FDropInInventory OnDropInInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FHoverItem OnHoverItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FUnHoverItem OnUnHoverItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	FRotateItem OnRotateItem;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void InitializeImcSubsystem();
	
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void ToggleInventory();
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void OpenUserInterface();
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void CloseUserInterface();
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	bool LineTraceFromMouse(FHitResult& HitResult) const;
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StartHoverInventory(UDieg_3DInventoryComponent* NewHoveringInventory);
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StopHoverInventory(UDieg_3DInventoryComponent* HoveringInventory);
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StartHoverItem(ADieg_WorldItemActor* NewHoveringWorldItem);
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void StopHoverItem(ADieg_WorldItemActor* HoveringWorldItem);
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void HandleTraceHit(const FHitResult& HitResult, bool bIsBlockingHit);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	bool IsDraggingItem() const { return DraggingItem.IsValid(); }
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	bool IsInInventory() const { return HoveringInventoryComponent3D.IsValid(); }
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Inventory Input Handler")
	bool IsItemInInventory(const ADieg_WorldItemActor* ItemIn, UDieg_3DInventoryComponent*& Inventory3DOut);
	
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void TryDragItem();
	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void TryDropItem();

	UFUNCTION(Category = "Game|Dieg|Inventory Input Handler")
	void HandleInputRotateItem();
	
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
