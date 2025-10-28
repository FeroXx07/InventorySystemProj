// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "UStructs/Inv_ItemPlacement.h"
#include "Inv_SpatialGridWidget.generated.h"

class UInv_InventoryComponent;
class UInv_InventoryController;
class UInv_InventoryContainer;
class UInv_ItemInstance;
class UInv_GridSlotWidget;
class UUniformGridPanel;

/**
 * Spatial grid widget for inventory visualization
 * - Uses UInv_GridSlotWidget for individual cells
 * - Binds to Controller for events (Controller doesn't know about Widget)
 * - Reads from Container directly (no data duplication)
 * - Emits input events to Controller
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYS_API UInv_SpatialGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ============================================
	// REFERENCES (Widget knows about system)
	// ============================================
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Grid")
	TObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Grid")
	TObjectPtr<UInv_InventoryController> Controller;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Grid")
	TObjectPtr<UInv_InventoryContainer> CurrentContainer;

	// ============================================
	// UI COMPONENTS (Bindable widgets)
	// ============================================
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Grid", meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> MainCanvas;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Grid", meta = (BindWidgetOptional))
	TObjectPtr<UUniformGridPanel> GridPanel;

	// ============================================
	// SLOT WIDGETS
	// ============================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Grid|Settings")
	TSubclassOf<UInv_GridSlotWidget> SlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Grid")
	TArray<TArray<TObjectPtr<UInv_GridSlotWidget>>> SlotWidgets;

	// ============================================
	// GRID SETTINGS
	// ============================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Grid|Settings")
	float CellSize = 64.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Grid|Settings")
	FVector2D SlotPadding = FVector2D(2.0f, 2.0f);

	// ============================================
	// WIDGET STATE
	// ============================================
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Grid")
	FIntPoint HoveredCell = FIntPoint(-1, -1);

	// ============================================
	// INITIALIZATION
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void SetInventoryComponent(UInv_InventoryComponent* InComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void SetGridSize(int32 Width, int32 Height);

	// ============================================
	// GRID MANAGEMENT
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void CreateGrid();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void UpdateGrid();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void ClearGrid();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void RenderGrid();

	// ============================================
	// ITEM VISUALIZATION
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void UpdateItemVisualization();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	void ClearHighlights();

	// ============================================
	// UTILITY
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	FIntPoint GetGridPositionFromMouse(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	FVector2D GetWidgetPositionFromGrid(FIntPoint GridPosition) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Grid")
	bool IsPositionInGrid(FIntPoint Position) const;

protected:
	// ============================================
	// LIFECYCLE
	// ============================================
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// ============================================
	// INPUT HANDLING (Widget emits to Controller)
	// ============================================
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	// ============================================
	// CONTROLLER EVENT BINDINGS (Widget listens to Controller)
	// ============================================
	
	void BindToController();
	void UnbindFromController();

	UFUNCTION()
	void OnItemPlaced(UInv_ItemInstance* Item, FIntPoint Position, float Rotation);

	UFUNCTION()
	void OnItemRemoved(UInv_ItemInstance* Item);

	UFUNCTION()
	void OnItemMoved(UInv_ItemInstance* Item, FIntPoint NewPosition, float NewRotation);

	UFUNCTION()
	void OnDragStarted(UInv_ItemInstance* Item, FIntPoint StartPosition);

	UFUNCTION()
	void OnDragEnded(UInv_ItemInstance* Item, bool bSuccess);

	UFUNCTION()
	void OnDragValidityChanged(FIntPoint Position, bool bValid);

	UFUNCTION()
	void OnItemSelected(UInv_ItemInstance* Item, FIntPoint Position);

	UFUNCTION()
	void OnItemDeselected(UInv_ItemInstance* Item);

	UFUNCTION()
	void OnContainerChanged(UInv_InventoryContainer* OldContainer, UInv_InventoryContainer* NewContainer);

	// ============================================
	// SLOT EVENT BINDINGS (Slots emit to Grid)
	// ============================================
	
	void BindSlotEvents(UInv_GridSlotWidget* Slot);
	
	UFUNCTION()
	void OnSlotClicked(UInv_GridSlotWidget* Slot, FIntPoint Position, bool bLeftButton);

	UFUNCTION()
	void OnSlotRightClicked(UInv_GridSlotWidget* Slot, FIntPoint Position, UInv_ItemInstance* Item);

	UFUNCTION()
	void OnSlotHovered(UInv_GridSlotWidget* Slot, FIntPoint Position);

	UFUNCTION()
	void OnSlotUnhovered(UInv_GridSlotWidget* Slot, FIntPoint Position);

	UFUNCTION()
	void OnSlotDragDetected(UInv_GridSlotWidget* Slot, FIntPoint Position, UInv_ItemInstance* Item);

	// ============================================
	// INTERNAL HELPERS
	// ============================================
	
	void CreateSlot(int32 X, int32 Y);
	void UpdateSlot(int32 X, int32 Y);
	void HighlightSlots(const TArray<FIntPoint>& Positions, bool bValid);
	UInv_GridSlotWidget* GetSlotAt(FIntPoint Position) const;

	int32 GridWidth = 10;
	int32 GridHeight = 10;
};
