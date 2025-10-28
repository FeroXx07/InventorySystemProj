// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Inv_GridSlotWidget.generated.h"

class UInv_ItemInstance;

/**
 * Individual grid slot widget for inventory system
 * - Manages its own visual state
 * - Handles input events for its cell
 * - Emits events to parent grid
 * - Blueprint-customizable
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYS_API UInv_GridSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ============================================
	// SLOT DATA
	// ============================================
	
	UPROPERTY(BlueprintReadOnly, Category = "Slot|Data")
	FIntPoint GridPosition = FIntPoint::ZeroValue;

	UPROPERTY(BlueprintReadOnly, Category = "Slot|Data")
	bool bIsOccupied = false;

	UPROPERTY(BlueprintReadOnly, Category = "Slot|Data")
	bool bIsHighlighted = false;

	UPROPERTY(BlueprintReadOnly, Category = "Slot|Data")
	bool bIsHovered = false;

	UPROPERTY(BlueprintReadOnly, Category = "Slot|Data")
	TObjectPtr<UInv_ItemInstance> ContainedItem = nullptr;

	// ============================================
	// UI COMPONENTS (Blueprint Bindable)
	// ============================================
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> RootBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> HoverOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> HighlightOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StackCountText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> LockedIcon;

	// ============================================
	// VISUAL SETTINGS
	// ============================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Visuals")
	FLinearColor NormalColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Visuals")
	FLinearColor OccupiedColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.8f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Visuals")
	FLinearColor HoverColor = FLinearColor(0.2f, 0.2f, 0.4f, 0.6f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Visuals")
	FLinearColor HighlightValidColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Visuals")
	FLinearColor HighlightInvalidColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Visuals")
	FLinearColor LockedColor = FLinearColor(0.5f, 0.1f, 0.1f, 0.8f);

	// ============================================
	// SLOT SETTINGS
	// ============================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Settings")
	bool bIsLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Settings")
	bool bShowStackCount = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Settings")
	bool bShowTooltip = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Settings")
	bool bEnableAnimations = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Settings")
	float HoverAnimationDuration = 0.15f;

	// ============================================
	// EVENTS (Slot emits to Grid)
	// ============================================
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSlotClicked, UInv_GridSlotWidget*, Slot, FIntPoint, Position, bool, bLeftButton);
	UPROPERTY(BlueprintAssignable, Category = "Slot|Events")
	FOnSlotClicked OnSlotClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSlotRightClicked, UInv_GridSlotWidget*, Slot, FIntPoint, Position, UInv_ItemInstance*, Item);
	UPROPERTY(BlueprintAssignable, Category = "Slot|Events")
	FOnSlotRightClicked OnSlotRightClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotHovered, UInv_GridSlotWidget*, Slot, FIntPoint, Position);
	UPROPERTY(BlueprintAssignable, Category = "Slot|Events")
	FOnSlotHovered OnSlotHovered;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotUnhovered, UInv_GridSlotWidget*, Slot, FIntPoint, Position);
	UPROPERTY(BlueprintAssignable, Category = "Slot|Events")
	FOnSlotUnhovered OnSlotUnhovered;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSlotDragDetected, UInv_GridSlotWidget*, Slot, FIntPoint, Position, UInv_ItemInstance*, Item);
	UPROPERTY(BlueprintAssignable, Category = "Slot|Events")
	FOnSlotDragDetected OnSlotDragDetected;

	// ============================================
	// INITIALIZATION
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Slot")
	void Initialize(FIntPoint InGridPosition);

	// ============================================
	// UPDATE METHODS
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetOccupied(bool bOccupied);

	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetItem(UInv_ItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Slot")
	void ClearItem();

	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetHighlight(bool bHighlight, bool bValid);

	UFUNCTION(BlueprintCallable, Category = "Slot")
	void ClearHighlight();

	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetLocked(bool bLocked);

	UFUNCTION(BlueprintCallable, Category = "Slot")
	void UpdateStackCount(int32 Count);

	// ============================================
	// VISUAL UPDATES
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Slot")
	void RefreshVisuals();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Slot")
	void PlayHoverAnimation(bool bEnter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Slot")
	void PlayClickAnimation();

	// ============================================
	// QUERIES
	// ============================================
	
	UFUNCTION(BlueprintPure, Category = "Slot")
	bool IsOccupied() const { return bIsOccupied; }

	UFUNCTION(BlueprintPure, Category = "Slot")
	bool IsLocked() const { return bIsLocked; }

	UFUNCTION(BlueprintPure, Category = "Slot")
	UInv_ItemInstance* GetItem() const { return ContainedItem; }

	UFUNCTION(BlueprintPure, Category = "Slot")
	FIntPoint GetGridPosition() const { return GridPosition; }

protected:
	// ============================================
	// WIDGET OVERRIDES
	// ============================================
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// ============================================
	// INTERNAL HELPERS
	// ============================================
	
	void UpdateBackgroundColor();
	void UpdateItemIcon();
	void UpdateStackCountDisplay();
	void UpdateLockedState();
	void ShowHoverOverlay(bool bShow);
	void ShowHighlightOverlay(bool bShow, bool bValid);

	// Drag detection
	bool bMouseDown = false;
	FVector2D MouseDownPosition = FVector2D::ZeroVector;
	float DragDetectionThreshold = 5.0f;
};

