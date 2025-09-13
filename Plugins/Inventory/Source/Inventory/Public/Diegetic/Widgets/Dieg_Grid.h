// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dieg_Grid.generated.h"

enum class EDieg_SlotStatus : uint8;
class UCanvasPanel;
class UDieg_Slot;
class UGridPanel;

/**
 * @brief Delegate fired when a slot in the grid is hovered.
 * 
 * @param InGeometry The geometry of the slot widget
 * @param InMouseEvent The mouse event that triggered the hover
 * @param Slot The slot that was hovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotHover, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when a slot in the grid is no longer hovered.
 * 
 * @param InMouseEvent The mouse event that triggered the unhover
 * @param Slot The slot that was unhovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotUnHover, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when a slot in the grid is pressed.
 * 
 * @param InGeometry The geometry of the slot widget
 * @param InMouseEvent The mouse event that triggered the press
 * @param Slot The slot that was pressed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotPressed, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when a slot in the grid is released.
 * 
 * @param InGeometry The geometry of the slot widget
 * @param InMouseEvent The mouse event that triggered the release
 * @param Slot The slot that was released
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotReleased, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief UI widget that displays a grid of inventory slots for the diegetic inventory system.
 * 
 * UDieg_Grid is the main UI component that renders the inventory grid in the user interface.
 * It manages a collection of UDieg_Slot widgets arranged in a grid layout and handles
 * user interaction events such as hovering, clicking, and dragging.
 * 
 * The grid widget provides visual feedback for item placement, slot highlighting, and
 * drag-and-drop operations. It works in conjunction with the inventory component to
 * display the current state of the inventory and handle user input.
 * 
 * @note This widget is designed to work with the diegetic inventory system and requires
 * proper setup with slot classes and grid dimensions.
 * 
 * @see UDieg_Slot
 * @see UDieg_InventoryComponent
 * @see UDieg_3DInventoryComponent
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_Grid : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief The grid panel widget that contains all the slot widgets.
	 * 
	 * This is the root container for the slot widgets. It should be bound
	 * to a UGridPanel in the widget blueprint.
	 * 
	 * @see UGridPanel
	 */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UGridPanel> GridPanel;

	/**
	 * @brief Total number of slots in the grid.
	 * 
	 * Defines the maximum number of slots that can be displayed in the grid.
	 * This should match the inventory component's total slot count.
	 * 
	 * @see MaxColumns
	 * @see UDieg_InventoryComponent::TotalSlots
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	int32 TotalSlots{35};

	/**
	 * @brief Number of columns in the grid layout.
	 * 
	 * Determines how many slots are displayed per row. The number of rows
	 * is calculated as TotalSlots / MaxColumns.
	 * 
	 * @see TotalSlots
	 * @see UDieg_InventoryComponent::MaxColumns
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxColumns{10};

	/**
	 * @brief The class to use for creating slot widgets.
	 * 
	 * This should be set to a UDieg_Slot subclass that defines the visual
	 * appearance and behavior of individual slots.
	 * 
	 * @see UDieg_Slot
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDieg_Slot> SlotClass;
	
	/**
	 * @brief Map of grid coordinates to slot widget instances.
	 * 
	 * This map stores all the slot widgets indexed by their grid coordinates.
	 * It's used for efficient lookup and management of individual slots.
	 * 
	 * @see UDieg_Slot
	 * @see GetSlotMap
	 * @see GetSlotMapBP
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, TObjectPtr<UDieg_Slot>> SlotMap;

	/**
	 * @brief Set of coordinates that are currently being hovered.
	 * 
	 * Tracks which slots are currently under the mouse cursor for visual
	 * feedback and interaction purposes.
	 * 
	 * @see UpdateHoveringSlots
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSet<FIntPoint> SlotsHoveredSet;

	/**
	 * @brief Enable debug visualization in the design-time editor.
	 * 
	 * When enabled, shows additional debug information in the editor
	 * to help with grid setup and debugging.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bDebugDesignTimeEditor{false};

	/**
	 * @brief Called before the widget is constructed.
	 * 
	 * Performs initial setup and validation of the grid widget.
	 */
	virtual void NativePreConstruct() override;
	
	/**
	 * @brief Creates an empty grid with the specified dimensions.
	 * 
	 * This Blueprint-native event creates all the slot widgets and arranges
	 * them in the grid layout. Override this in Blueprint to customize
	 * the grid creation process.
	 * 
	 * @param TotalSlots_ The total number of slots to create
	 * @param MaxColumns_ The number of columns in the grid
	 * 
	 * @see CreateSlot
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|Grid")
	void CreateEmptyGrid(int32 TotalSlots_, int32 MaxColumns_);
	
	/**
	 * @brief Gets the slot map (C++ only).
	 * 
	 * Returns a reference to the slot map for efficient access and modification.
	 * 
	 * @return Reference to the slot map
	 * 
	 * @see GetSlotMapBP
	 * @see SlotMap
	 */
	TMap<FIntPoint, UDieg_Slot*>& GetSlotMap();
	
	/**
	 * @brief Gets the slot map (Blueprint compatible).
	 * 
	 * Blueprint-compatible version that returns the slot map by value.
	 * 
	 * @return Copy of the slot map
	 * 
	 * @see GetSlotMap
	 * @see SlotMap
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Grid")
	TMap<FIntPoint, UDieg_Slot*> GetSlotMapBP();

	/**
	 * @brief Delegate fired when a slot is hovered.
	 * 
	 * Bind to this delegate to respond to slot hover events.
	 * 
	 * @see FGridSlotHover
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotHover OnGridSlotHover;
	
	/**
	 * @brief Delegate fired when a slot is no longer hovered.
	 * 
	 * Bind to this delegate to respond to slot unhover events.
	 * 
	 * @see FGridSlotUnHover
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotUnHover OnGridSlotUnHover;
	
	/**
	 * @brief Delegate fired when a slot is pressed.
	 * 
	 * Bind to this delegate to respond to slot press events.
	 * 
	 * @see FGridSlotPressed
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotPressed OnGridSlotPressed;
	
	/**
	 * @brief Delegate fired when a slot is released.
	 * 
	 * Bind to this delegate to respond to slot release events.
	 * 
	 * @see FGridSlotReleased
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotReleased OnGridSlotReleased;

	/**
	 * @brief Updates the visual state of hovering slots.
	 * 
	 * Changes the appearance of the specified slots to indicate hover state
	 * and resets other slots to their normal appearance.
	 * 
	 * @param NewCoordinates Array of coordinates to set to the new status
	 * @param NewStatus The status to apply to the new coordinates
	 * @param ResetStatus The status to apply to previously hovered slots
	 * 
	 * @see SlotsHoveredSet
	 * @see EDieg_SlotStatus
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Grid")
	void UpdateHoveringSlots(const TArray<FIntPoint>& NewCoordinates, EDieg_SlotStatus NewStatus, EDieg_SlotStatus ResetStatus);

	/**
	 * @brief Modifies the appearance of all slots in the grid.
	 * 
	 * Applies a uniform appearance change to all slots, useful for
	 * indicating locked states or other global visual changes.
	 * 
	 * @param IsAppearanceLocked Whether the appearance should be locked
	 * @param Override Whether to override the current appearance
	 * @param OverrideStatus The status to apply if overriding
	 * 
	 * @see EDieg_SlotStatus
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Grid")
	void ModifyAllSlotsAppearance(bool IsAppearanceLocked, bool Override, EDieg_SlotStatus OverrideStatus);

private:
	/**
	 * @brief Creates a new slot widget at the specified coordinates.
	 * 
	 * Factory method that creates and initializes a new slot widget
	 * using the configured slot class.
	 * 
	 * @param Point The grid coordinates for the new slot
	 * @return Pointer to the created slot widget, or nullptr if creation failed
	 * 
	 * @see SlotClass
	 * @see CreateEmptyGrid
	 */
	UFUNCTION(Category = "Game|Dieg|Grid")
	UDieg_Slot* CreateSlot(const FIntPoint& Point);

	/**
	 * @brief Handles hover events from child slot widgets.
	 * 
	 * Called when a child slot widget is hovered. Forwards the event
	 * to the grid's hover delegate.
	 * 
	 * @param InGeometry The geometry of the slot widget
	 * @param InMouseEvent The mouse event that triggered the hover
	 * @param GridSlot The slot that was hovered
	 * 
	 * @see OnGridSlotHover
	 * @see ChildSlotUnHovered
	 */
	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	/**
	 * @brief Handles unhover events from child slot widgets.
	 * 
	 * Called when a child slot widget is no longer hovered. Forwards the event
	 * to the grid's unhover delegate.
	 * 
	 * @param InMouseEvent The mouse event that triggered the unhover
	 * @param GridSlot The slot that was unhovered
	 * 
	 * @see OnGridSlotUnHover
	 * @see ChildSlotHover
	 */
	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	/**
	 * @brief Handles press events from child slot widgets.
	 * 
	 * Called when a child slot widget is pressed. Forwards the event
	 * to the grid's press delegate.
	 * 
	 * @param InGeometry The geometry of the slot widget
	 * @param InMouseEvent The mouse event that triggered the press
	 * @param GridSlot The slot that was pressed
	 * 
	 * @see OnGridSlotPressed
	 * @see ChildSlotReleased
	 */
	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	/**
	 * @brief Handles release events from child slot widgets.
	 * 
	 * Called when a child slot widget is released. Forwards the event
	 * to the grid's release delegate.
	 * 
	 * @param InGeometry The geometry of the slot widget
	 * @param InMouseEvent The mouse event that triggered the release
	 * @param GridSlot The slot that was released
	 * 
	 * @see OnGridSlotReleased
	 * @see ChildSlotPressed
	 */
	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);
};
