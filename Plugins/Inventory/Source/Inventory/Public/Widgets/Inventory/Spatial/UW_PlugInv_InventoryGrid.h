// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPF_PlugInv_DataLibrary.h"
#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"
#include "Widgets/Inventory/GridSlots/UW_PlugInv_GridSlot.h"

#include "UW_PlugInv_InventoryGrid.generated.h"

class UPlugInv_HoverItem;
struct FPlugInv_GridFragment;
class UPlugInv_SlottedItem;
struct FPlugInv_ItemManifest;
class UPlugInv_ItemComponent;
class UPlugInv_InventoryComponent;
class UCanvasPanel;
class UPlugInv_GridSlot;

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// Binded to the inventory component delegate OnItemAdded()
	UFUNCTION()
	void AddItem(UPlugInv_InventoryItem* Item);

	// Check if the category of the inventory and the item matches.
	bool MatchesCategory(const UPlugInv_InventoryItem* Item) const;

	// Returns availability result data for this grid.
	FPlugInv_SlotAvailabilityResult HasRoomForItem(const UPlugInv_ItemComponent* ItemComponent);
	
	// Item category of the grid.
	EPlugInv_ItemCategory GetItemCategory() const
	{
		return this->ItemCategory;
	}

	void ShowCursor();
	void HideCursor();
	
private:
	// Function to construct the actual grid.
	void ConstructGrid();
	
	// Overloads for HasRoomForItem
	FPlugInv_SlotAvailabilityResult HasRoomForItem(const UPlugInv_InventoryItem* InventoryItem);
	FPlugInv_SlotAvailabilityResult HasRoomForItem(const FPlugInv_ItemManifest& ItemManifest);
	
	// Add Item to multiple indices if needed
	void AddItemToIndices(const FPlugInv_SlotAvailabilityResult& Result, UPlugInv_InventoryItem* NewItem);
	
	// Add Item to single index
	void AddItemToIndex(UPlugInv_InventoryItem* NewItem, const int32 Index, bool bStackable, int32 StackAmount);
	
	// Create a slotted item widget
	UPlugInv_SlottedItem* CreateSlottedItem(UPlugInv_InventoryItem* NewItem, int32 Index, bool bStackable, int32 StackAmount, const FPlugInv_GridFragment*
	                                        GridFragment, const FPlugInv_ImageFragment* ImageFragment) const;

	// Adds the slotted item widget to the canvas panel
	void AddSlottedItemToCanvas(const int32 Index, const FPlugInv_GridFragment* GridFragment, UPlugInv_SlottedItem* SlottedItem) const;

	// Update grid slots
	void UpdateGridSlots(UPlugInv_InventoryItem* NewItem, int32 Index, bool bStackableItem, int32 StackAmount);
	
	// Draw size helper function
	FVector2D GetDrawSize(const FPlugInv_GridFragment* GridFragment) const;
	
	// Slotted image helper function
	void SetSlottedImage(const FPlugInv_GridFragment* GridFragment, const FPlugInv_ImageFragment* ImageFragment,const UPlugInv_SlottedItem* SlottedItem) const;

	// New item addition functions
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const TObjectPtr<UPlugInv_GridSlot>& GridSlot);
	bool IsInGridBounds(int32 StartIndex, const FIntPoint& ItemDimensions) const;
	bool DoesItemTypeMatch(TWeakObjectPtr<UPlugInv_InventoryItem> SubItem, const FGameplayTag& ItemTypeTag) const;
	bool CheckSlotConstraints(const FPlugInv_ItemManifest& ItemManifest, const UPlugInv_GridSlot* GridSlot, const UPlugInv_GridSlot* SubGridSlot, const
	                          TSet<int32>& CheckedIndices, TSet<int32>
	                          & OutTentativelyClaimedIndices) const;
	int32 GetSlotStackAmount(const TObjectPtr<UPlugInv_GridSlot>& GridSlot);

	// Hover, pickup, tiles updates functions...
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	void PickUp(UPlugInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	void AssignHoverItem(UPlugInv_InventoryItem* InventoryItem);
	void AssignHoverItem(UPlugInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);
	void RemoveItemFromGrid(UPlugInv_InventoryItem* InventoryItem, const int32 GridIndex);
	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	EPlugInv_TileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	void OnTileParametersUpdated(const FPlugInv_TileParameters& Parameters);
	FIntPoint CalculateStartingCoordinate(const FIntPoint& Coordinates, const FIntPoint& Dimensions, const EPlugInv_TileQuadrant Quadrant) const;
	FInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);
	bool CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location);
	void HighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EPlugInv_GridSlotState GridSlotState);
	void PutDownOnIndex(const int32 Index);
	void ClearHoverItem();
	UUserWidget* GetVisibleCursorWidget();
	UUserWidget* GetHiddenCursorWidget();
	bool IsHoverAndClickedSameTypeAndStackable(const UPlugInv_InventoryItem* ClickedInventoryItem) const;
	void SwapClickedWithHoverItem(UPlugInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	bool ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount, const int32 MaxStackSize) const;
	void SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);
	bool ShouldConsumeHoverItemStacks(const int32 HoveredStackCount, const int32 RoomInClickedSlot) const;
	void ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);
	bool ShouldFillInStack(const int32 RoomInClickedSlot, const int32 HoveredStackCount) const;
	void FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index);
	
	UFUNCTION()
	void AddStacks(const FPlugInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent);
	
	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);
	
	// Weak ref to the inventory component.
	TWeakObjectPtr<UPlugInv_InventoryComponent> InventoryComponent;

	// Type of the items that can be stored in this particular grid.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EPlugInv_ItemCategory ItemCategory;

	// Container of grid slots.
	UPROPERTY()
	TArray<TObjectPtr<UPlugInv_GridSlot>> GridSlots;

	// Type of the class of grid slot to create.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_GridSlot> GridSlotClass;

	// Slott widget class type
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_SlottedItem> SlottedItemClass;

	// Dictionary container of index and slotted item widgets
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<int32, TObjectPtr<UPlugInv_SlottedItem>> SlottedItemMap;

	// Rather, to add all the gridslots to the viewport, store them in a canvas panel instead.
	// Panel size changes also affect the grid slots.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	// Rows of the grid.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	// Columns of the grid.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;
	
	// Size of the tiles in the grid.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 TileSize;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_HoverItem> HoverItemClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UPlugInv_HoverItem> HoverItem;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> VisibleCursorWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> HiddenCursorWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> VisibleCursorWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> HiddenCursorWidget;

	FPlugInv_TileParameters TileParameters;
	FPlugInv_TileParameters LastTileParameters;

	// Index where an item would be placed if we click on the grid at a valid location
	int32 ItemDropIndex{INDEX_NONE};
	FInv_SpaceQueryResult CurrentQueryResult;
	bool bCurrentMouseWithinCanvas;
	bool bLastMouseWithinCanvas;
	int32 LastHighlightedIndex;
	FIntPoint LastHighlightedDimensions;
};


