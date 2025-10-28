// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Inv_InventoryController.generated.h"

class UInv_InventoryComponent;
class UInv_InventoryContainer;
class UInv_ItemInstance;

/**
 * Controller for inventory interactions - The middleman between UI and Data
 * - Translates UI events into game operations
 * - Tracks interaction state (dragging, rotation, selection)
 * - NO data duplication (references data layer only)
 * - UI-agnostic (doesn't know about widgets)
 */
UCLASS(BlueprintType)
class INVENTORYSYS_API UInv_InventoryController : public UObject
{
	GENERATED_BODY()

public:
	UInv_InventoryController();

	// ============================================
	// REFERENCES (NOT ownership)
	// ============================================
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Controller")
	TObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Controller")
	TObjectPtr<UInv_InventoryContainer> CurrentContainer;

	// ============================================
	// INTERACTION STATE (Controller's responsibility)
	// ============================================
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Controller|State")
	bool bIsDragging = false;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Controller|State")
	TObjectPtr<UInv_ItemInstance> DraggedItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Controller|State")
	FIntPoint DragStartPosition = FIntPoint::ZeroValue;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Controller|State")
	float CurrentRotation = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Controller|State")
	TObjectPtr<UInv_ItemInstance> SelectedItem = nullptr;

	// ============================================
	// SETTINGS
	// ============================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Controller|Settings")
	bool bAllowRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Controller|Settings")
	float RotationStep = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Controller|Settings")
	bool bAutoStack = true;

	// ============================================
	// EVENTS (Controller broadcasts, Widget listens)
	// ============================================
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemPlaced, UInv_ItemInstance*, Item, FIntPoint, Position, float, Rotation);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnItemPlaced OnItemPlaced;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UInv_ItemInstance*, Item);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnItemRemoved OnItemRemoved;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemMoved, UInv_ItemInstance*, Item, FIntPoint, NewPosition, float, NewRotation);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnItemMoved OnItemMoved;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDragStarted, UInv_ItemInstance*, Item, FIntPoint, StartPosition);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnDragStarted OnDragStarted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDragEnded, UInv_ItemInstance*, Item, bool, bSuccess);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnDragEnded OnDragEnded;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDragValidityChanged, FIntPoint, Position, bool, bValid);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnDragValidityChanged OnDragValidityChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSelected, UInv_ItemInstance*, Item, FIntPoint, Position);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnItemSelected OnItemSelected;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDeselected, UInv_ItemInstance*, Item);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnItemDeselected OnItemDeselected;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRotationChanged, UInv_ItemInstance*, Item, float, NewRotation);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnRotationChanged OnRotationChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContainerChanged, UInv_InventoryContainer*, OldContainer, UInv_InventoryContainer*, NewContainer);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Controller|Events")
	FOnContainerChanged OnContainerChanged;

	// ============================================
	// LIFECYCLE
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	void Initialize(UInv_InventoryComponent* InInventoryComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	void Shutdown();

	// ============================================
	// CONTAINER MANAGEMENT
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	void SetCurrentContainer(UInv_InventoryContainer* Container);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	void SetCurrentContainerByName(FName ContainerName);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	UInv_InventoryContainer* GetCurrentContainer() const { return CurrentContainer; }

	// ============================================
	// INPUT HANDLING (Widget calls these)
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Input")
	void HandleCellClicked(int32 X, int32 Y, bool bLeftButton, bool bRightButton = false);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Input")
	void HandleCellRightClicked(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Input")
	void HandleMouseMoved(FIntPoint GridPosition);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Input")
	void HandleRotationRequested(float RotationDelta);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Input")
	void HandleCancelRequested();

	// ============================================
	// ITEM OPERATIONS
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	bool TryPickupItem(UInv_ItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	bool TryPlaceItem(UInv_ItemInstance* Item, FIntPoint Position, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	bool TryPlaceItemAtBestPosition(UInv_ItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	bool TryRemoveItem(UInv_ItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller")
	bool TryMoveItem(UInv_ItemInstance* Item, FIntPoint NewPosition, float NewRotation);

	// ============================================
	// DRAG OPERATIONS
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Drag")
	void StartDrag(UInv_ItemInstance* Item, FIntPoint StartPosition);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Drag")
	void UpdateDrag(FIntPoint NewPosition);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Drag")
	bool EndDrag(FIntPoint Position);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Drag")
	void CancelDrag();

	// ============================================
	// SELECTION
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Selection")
	void SelectItem(UInv_ItemInstance* Item, FIntPoint Position);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Selection")
	void DeselectItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Selection")
	bool IsItemSelected(UInv_ItemInstance* Item) const;

	// ============================================
	// ROTATION
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Rotation")
	void RotateItem(float RotationDelta);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Rotation")
	void SetRotation(float NewRotation);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Rotation")
	float GetCurrentRotation() const { return CurrentRotation; }

	// ============================================
	// VALIDATION (Widget queries these)
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Validation")
	bool CanPlaceItemAt(UInv_ItemInstance* Item, FIntPoint Position, float Rotation) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Validation")
	bool CanPlaceDraggedItemAt(FIntPoint Position) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|Validation")
	TArray<FIntPoint> GetDraggedItemShapeAtPosition(FIntPoint Position) const;

	// ============================================
	// STATE QUERIES
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|State")
	bool IsDragging() const { return bIsDragging; }

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|State")
	UInv_ItemInstance* GetDraggedItem() const { return DraggedItem; }

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|State")
	UInv_ItemInstance* GetSelectedItem() const { return SelectedItem; }

	UFUNCTION(BlueprintCallable, Category = "Inventory|Controller|State")
	FIntPoint GetDragStartPosition() const { return DragStartPosition; }

protected:
	// Internal helpers
	void ClearDragState();
	void NotifyDragValidityChanged(FIntPoint Position, bool bValid);
	UInv_ItemInstance* GetItemAtPosition(FIntPoint Position) const;
};


