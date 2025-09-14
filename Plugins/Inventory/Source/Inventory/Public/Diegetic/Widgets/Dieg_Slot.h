// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Diegetic/Dieg_DataLibrary.h"
#include "Dieg_Slot.generated.h"

class UCanvasPanel;
enum class EDieg_SlotStatus : uint8;
class UDieg_Grid;
class UImage;
class USizeBox;

/**
 * @brief Delegate fired when the slot is hovered.
 * 
 * @param InGeometry The geometry of the slot widget
 * @param InMouseEvent The mouse event that triggered the hover
 * @param Slot The slot that was hovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHoverSlot, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when the slot is no longer hovered.
 * 
 * @param InMouseEvent The mouse event that triggered the unhover
 * @param Slot The slot that was unhovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnHoverSlot, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when the slot is pressed.
 * 
 * @param InGeometry The geometry of the slot widget
 * @param InMouseEvent The mouse event that triggered the press
 * @param Slot The slot that was pressed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPressedSlot, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief Delegate fired when the slot is released.
 * 
 * @param InGeometry The geometry of the slot widget
 * @param InMouseEvent The mouse event that triggered the release
 * @param Slot The slot that was released
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReleasedSlot, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * @brief Individual slot widget for the diegetic inventory grid system.
 * 
 * UDieg_Slot represents a single slot in the inventory grid UI. It handles
 * visual representation, user interaction, and state management for individual
 * inventory slots. Each slot can display different visual states based on
 * its current status and can respond to mouse events.
 * 
 * The slot widget provides visual feedback for item placement, hover states,
 * and interaction feedback. It works in conjunction with the grid widget
 * to provide a complete inventory interface.
 * 
 * @note This widget is designed to be used within a UDieg_Grid and requires
 * proper setup with parent grid references.
 * 
 * @see UDieg_Grid
 * @see EDieg_SlotStatus
 * @see FDieg_InventorySlot
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_Slot : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Root size box that controls the overall slot dimensions.
	 * 
	 * This should be bound to a USizeBox in the widget blueprint.
	 * It controls the size of the entire slot widget.
	 * 
	 * @see USizeBox
	 * @see SlotSize
	 */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_Root;

	/**
	 * @brief Canvas panel for the root content area.
	 * 
	 * Optional root canvas panel that can contain additional UI elements.
	 * This should be bound to a UCanvasPanel in the widget blueprint.
	 * 
	 * @see UCanvasPanel
	 */
	UPROPERTY(meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CanvasPanel_Root;
	
	/**
	 * @brief Overlay size box for additional UI elements.
	 * 
	 * Optional overlay container that can be used for additional visual elements.
	 * This should be bound to a USizeBox in the widget blueprint.
	 * 
	 * @see USizeBox
	 */
	UPROPERTY(meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> Overlay_Root;
	
	/**
	 * @brief Image widget for the slot fill/background.
	 * 
	 * This should be bound to a UImage in the widget blueprint.
	 * It displays the background color based on the slot status.
	 * 
	 * @see UImage
	 * @see SlotFillColor
	 * @see SetStatusAndColor
	 */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_Fill;

	/**
	 * @brief Image widget for the slot outline/border.
	 * 
	 * This should be bound to a UImage in the widget blueprint.
	 * It displays the border/outline of the slot.
	 * 
	 * @see UImage
	 */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_Outline;

	/**
	 * @brief Weak reference to the parent grid widget.
	 * 
	 * Points to the UDieg_Grid that contains this slot. Used for
	 * communication and event forwarding.
	 * 
	 * @see UDieg_Grid
	 * @see SetParentGrid
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_Grid> ParentGrid;

	/**
	 * @brief Current status of the slot.
	 * 
	 * Determines the visual appearance and behavior of the slot.
	 * Different statuses can have different colors and interaction states.
	 * 
	 * @see EDieg_SlotStatus
	 * @see SetStatusAndColor
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	EDieg_SlotStatus SlotStatus{EDieg_SlotStatus::None};
	
	/**
	 * @brief Flag indicating whether the slot's appearance is locked.
	 * 
	 * When true, the slot's visual appearance cannot be changed.
	 * Used for preventing visual updates during certain operations.
	 * 
	 * @see SetAppearanceLocked
	 * @see IsAppearanceLocked
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bIsLockedAppearance;

	/**
	 * @brief Size of the slot in 2D UI space.
	 * 
	 * Defines the dimensions of the slot widget in the UI.
	 * This affects the visual size of the slot in the grid.
	 * 
	 * @see FVector2D
	 * @see SizeBox_Root
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	FVector2D SlotSize{100.0f, 100.0f};

	/**
	 * @brief Map of slot statuses to their corresponding fill colors.
	 * 
	 * Defines the visual appearance for each slot status.
	 * The fill color is applied to the Image_Fill widget.
	 * 
	 * @see EDieg_SlotStatus
	 * @see FLinearColor
	 * @see Image_Fill
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<EDieg_SlotStatus, FLinearColor> SlotFillColor;

	/**
	 * @brief Scale factor for items when displayed in 3D space.
	 * 
	 * Used to convert 2D UI coordinates to 3D world space.
	 * This affects how items are positioned and scaled in the 3D world.
	 * 
	 * @see GridSize3D
	 * @see GetInventoryScale3D
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	float InventoryScale3D{5.0f};

	/**
	 * @brief Size of the grid slot in 3D world space.
	 * 
	 * Defines the actual size of the slot in 3D world units.
	 * Used for 3D positioning and collision detection.
	 * 
	 * @see InventoryScale3D
	 * @see GetGridSize3D
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	float GridSize3D{20.0f};

	/**
	 * @brief Last updated grid coordinates for this slot.
	 * 
	 * Tracks the grid coordinates that were last used to update this slot.
	 * Used for optimization and change detection.
	 * 
	 * @see FIntPoint
	 * @see GetCoordinatesInGrid
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Slot|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint LastUpdatedCoordinate;

	/**
	 * @brief Called before the widget is constructed.
	 * 
	 * Performs initial setup and validation of the slot widget.
	 */
	virtual void NativePreConstruct() override;
	
	/**
	 * @brief Called when the mouse enters the slot widget.
	 * 
	 * Handles mouse enter events and forwards them to the appropriate delegates.
	 * 
	 * @param InGeometry The geometry of the slot widget
	 * @param InMouseEvent The mouse event that triggered the enter
	 * 
	 * @see OnHoverSlot
	 */
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	/**
	 * @brief Called when the mouse leaves the slot widget.
	 * 
	 * Handles mouse leave events and forwards them to the appropriate delegates.
	 * 
	 * @param InMouseEvent The mouse event that triggered the leave
	 * 
	 * @see OnUnHoverSlot
	 */
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	/**
	 * @brief Called when a mouse button is pressed on the slot widget.
	 * 
	 * Handles mouse button down events and forwards them to the appropriate delegates.
	 * 
	 * @param InGeometry The geometry of the slot widget
	 * @param InMouseEvent The mouse event that triggered the press
	 * @return Reply indicating whether the event was handled
	 * 
	 * @see OnPressedSlot
	 */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	/**
	 * @brief Called when a mouse button is released on the slot widget.
	 * 
	 * Handles mouse button up events and forwards them to the appropriate delegates.
	 * 
	 * @param InGeometry The geometry of the slot widget
	 * @param InMouseEvent The mouse event that triggered the release
	 * @return Reply indicating whether the event was handled
	 * 
	 * @see OnReleasedSlot
	 */
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	/**
	 * @brief Initializes default values for the slot widget.
	 * 
	 * This Blueprint-native event sets up default values for the slot.
	 * Override this in Blueprint to customize the initialization process.
	 * 
	 * @see SlotFillColor
	 * @see SlotSize
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|Slot")
	void InitializeDefaults();
	
	/**
	 * @brief Gets the coordinates of this slot in the grid.
	 * 
	 * This Blueprint-native event returns the grid coordinates of this slot.
	 * Override this in Blueprint to customize coordinate calculation.
	 * 
	 * @return The grid coordinates of this slot
	 * 
	 * @see LastUpdatedCoordinate
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|Slot")
	FIntPoint GetCoordinatesInGrid();
	
	/**
	 * @brief Sets the status and updates the visual appearance of the slot.
	 * 
	 * This Blueprint-native event changes the slot's status and updates its
	 * visual appearance accordingly. Override this in Blueprint to customize
	 * the visual update process.
	 * 
	 * @param Status The new status to set
	 * @param bChangeAppearance Whether to update the visual appearance
	 * 
	 * @see SlotStatus
	 * @see SlotFillColor
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|Slot")
	void SetStatusAndColor(EDieg_SlotStatus Status, bool bChangeAppearance = true);

	/**
	 * @brief Checks if the slot is not locked.
	 * 
	 * @return true if the slot is not locked, false otherwise
	 * 
	 * @see SlotStatus
	 * @see EDieg_SlotStatus::Locked
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Slot")
	bool IsNotLocked() const { return SlotStatus != EDieg_SlotStatus::Locked; }

	/**
	 * @brief Checks if the slot's appearance is locked.
	 * 
	 * @return true if the appearance is locked, false otherwise
	 * 
	 * @see bIsLockedAppearance
	 * @see SetAppearanceLocked
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Slot")
	bool IsAppearanceLocked() const { return bIsLockedAppearance; }

	/**
	 * @brief Sets whether the slot's appearance is locked.
	 * 
	 * @param Value The new locked state
	 * 
	 * @see bIsLockedAppearance
	 * @see IsAppearanceLocked
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Slot")
	void SetAppearanceLocked(bool Value) { bIsLockedAppearance = Value; }

	/**
	 * @brief Sets the parent grid widget for this slot.
	 * 
	 * @param Grid The grid widget that contains this slot
	 * 
	 * @see ParentGrid
	 * @see UDieg_Grid
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Slot")
	void SetParentGrid(UDieg_Grid* Grid);

	/**
	 * @brief Gets the 3D inventory scale factor.
	 * 
	 * @return The inventory scale factor
	 * 
	 * @see InventoryScale3D
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Slot")
	float GetInventoryScale3D() const { return InventoryScale3D; }
	
	/**
	 * @brief Gets the 3D grid size.
	 * 
	 * @return The grid size in 3D world units
	 * 
	 * @see GridSize3D
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Slot")
	float GetGridSize3D() const { return GridSize3D; }

	/**
	 * @brief Delegate fired when the slot is hovered.
	 * 
	 * Bind to this delegate to respond to slot hover events.
	 * 
	 * @see FHoverSlot
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Slot")
	FHoverSlot OnHoverSlot;
	
	/**
	 * @brief Delegate fired when the slot is no longer hovered.
	 * 
	 * Bind to this delegate to respond to slot unhover events.
	 * 
	 * @see FUnHoverSlot
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Slot")
	FUnHoverSlot OnUnHoverSlot;
	
	/**
	 * @brief Delegate fired when the slot is pressed.
	 * 
	 * Bind to this delegate to respond to slot press events.
	 * 
	 * @see FPressedSlot
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Slot")
	FPressedSlot OnPressedSlot;
	
	/**
	 * @brief Delegate fired when the slot is released.
	 * 
	 * Bind to this delegate to respond to slot release events.
	 * 
	 * @see FReleasedSlot
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Slot")
	FReleasedSlot OnReleasedSlot;
};
