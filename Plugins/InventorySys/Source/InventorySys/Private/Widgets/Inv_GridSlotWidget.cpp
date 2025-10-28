// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inv_GridSlotWidget.h"
#include "UObjects/Inv_ItemInstance.h"
#include "UObjects/Inv_ItemDefinition.h"
#include "UObjects/Inv_ItemDataAsset.h"
#include "Components/BorderSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// ============================================
// LIFECYCLE
// ============================================

void UInv_GridSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize visuals
	RefreshVisuals();
}

void UInv_GridSlotWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

// ============================================
// INITIALIZATION
// ============================================

void UInv_GridSlotWidget::Initialize(FIntPoint InGridPosition)
{
	GridPosition = InGridPosition;
	bIsOccupied = false;
	bIsHighlighted = false;
	bIsHovered = false;
	ContainedItem = nullptr;
	
	RefreshVisuals();
}

// ============================================
// UPDATE METHODS
// ============================================

void UInv_GridSlotWidget::SetOccupied(bool bOccupied)
{
	if (bIsOccupied != bOccupied)
	{
		bIsOccupied = bOccupied;
		UpdateBackgroundColor();
	}
}

void UInv_GridSlotWidget::SetItem(UInv_ItemInstance* Item)
{
	ContainedItem = Item;
	bIsOccupied = (Item != nullptr);
	
	UpdateItemIcon();
	UpdateStackCountDisplay();
	UpdateBackgroundColor();
}

void UInv_GridSlotWidget::ClearItem()
{
	SetItem(nullptr);
}

void UInv_GridSlotWidget::SetHighlight(bool bHighlight, bool bValid)
{
	bIsHighlighted = bHighlight;
	ShowHighlightOverlay(bHighlight, bValid);
}

void UInv_GridSlotWidget::ClearHighlight()
{
	SetHighlight(false, false);
}

void UInv_GridSlotWidget::SetLocked(bool bLocked)
{
	if (bIsLocked != bLocked)
	{
		bIsLocked = bLocked;
		UpdateLockedState();
		UpdateBackgroundColor();
	}
}

void UInv_GridSlotWidget::UpdateStackCount(int32 Count)
{
	if (ContainedItem)
	{
		ContainedItem->SetQuantity(Count);
		UpdateStackCountDisplay();
	}
}

// ============================================
// VISUAL UPDATES
// ============================================

void UInv_GridSlotWidget::RefreshVisuals()
{
	UpdateBackgroundColor();
	UpdateItemIcon();
	UpdateStackCountDisplay();
	UpdateLockedState();
}

void UInv_GridSlotWidget::PlayHoverAnimation_Implementation(bool bEnter)
{
	// Can be overridden in Blueprint for custom animations
	if (!bEnableAnimations)
	{
		return;
	}
	
	// Default behavior - just show/hide overlay
	ShowHoverOverlay(bEnter);
}

void UInv_GridSlotWidget::PlayClickAnimation_Implementation()
{
	// Can be overridden in Blueprint for custom animations
	if (!bEnableAnimations)
	{
		return;
	}
	
	// Default: Quick scale pulse (can be done via Blueprint animation)
}

// ============================================
// INPUT HANDLING
// ============================================

FReply UInv_GridSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsLocked)
	{
		return FReply::Handled();
	}

	bMouseDown = true;
	MouseDownPosition = InMouseEvent.GetScreenSpacePosition();

	bool bLeftButton = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	bool bRightButton = InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);

	if (bRightButton && ContainedItem)
	{
		// Right click on occupied slot
		OnSlotRightClicked.Broadcast(this, GridPosition, ContainedItem);
		return FReply::Handled();
	}
	else if (bLeftButton)
	{
		// Left click
		OnSlotClicked.Broadcast(this, GridPosition, true);
		
		// Play click animation
		PlayClickAnimation();
		
		// Detect drag for occupied slots
		if (ContainedItem)
		{
			return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		}
		
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UInv_GridSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMouseDown = false;
	return FReply::Handled();
}

void UInv_GridSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	bIsHovered = true;
	
	// Show hover overlay
	ShowHoverOverlay(true);
	
	// Play hover animation
	PlayHoverAnimation(true);
	
	// Emit hover event
	OnSlotHovered.Broadcast(this, GridPosition);
}

void UInv_GridSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	bIsHovered = false;
	bMouseDown = false;
	
	// Hide hover overlay
	ShowHoverOverlay(false);
	
	// Play hover exit animation
	PlayHoverAnimation(false);
	
	// Emit unhover event
	OnSlotUnhovered.Broadcast(this, GridPosition);
}

void UInv_GridSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	if (ContainedItem && !bIsLocked)
	{
		// Emit drag detected event
		OnSlotDragDetected.Broadcast(this, GridPosition, ContainedItem);
	}
}

// ============================================
// INTERNAL HELPERS
// ============================================

void UInv_GridSlotWidget::UpdateBackgroundColor()
{
	if (!BackgroundImage)
	{
		return;
	}

	FLinearColor TargetColor;
	
	if (bIsLocked)
	{
		TargetColor = LockedColor;
	}
	else if (bIsOccupied)
	{
		TargetColor = OccupiedColor;
	}
	else
	{
		TargetColor = NormalColor;
	}

	BackgroundImage->SetColorAndOpacity(TargetColor);
}

void UInv_GridSlotWidget::UpdateItemIcon()
{
	if (!ItemIcon)
	{
		return;
	}

	if (ContainedItem)
	{
		const UInv_ItemDefinition* ItemDef = ContainedItem->GetItemDefinition();
		if (ItemDef)
		{
			// Try to load icon texture
			TSoftObjectPtr<UTexture2D> IconSoftPtr = ItemDef->Icon2d;
			
			if (IconSoftPtr.IsValid())
			{
				UTexture2D* IconTexture = IconSoftPtr.Get();
				if (IconTexture)
				{
					ItemIcon->SetBrushFromTexture(IconTexture);
					ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
					return;
				}
			}
			else if (!IconSoftPtr.IsNull())
			{
				// Try to load synchronously (use async loading in production)
				UTexture2D* IconTexture = IconSoftPtr.LoadSynchronous();
				if (IconTexture)
				{
					ItemIcon->SetBrushFromTexture(IconTexture);
					ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
					return;
				}
			}
		}
		
		// No icon found - show default
		ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemIcon->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		// No item - hide icon
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_GridSlotWidget::UpdateStackCountDisplay()
{
	if (!StackCountText || !bShowStackCount)
	{
		if (StackCountText)
		{
			StackCountText->SetVisibility(ESlateVisibility::Collapsed);
		}
		return;
	}

	if (ContainedItem)
	{
		int32 Quantity = ContainedItem->GetQuantity();
		
		if (Quantity > 1)
		{
			StackCountText->SetText(FText::AsNumber(Quantity));
			StackCountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			StackCountText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		StackCountText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_GridSlotWidget::UpdateLockedState()
{
	if (!LockedIcon)
	{
		return;
	}

	if (bIsLocked)
	{
		LockedIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		LockedIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_GridSlotWidget::ShowHoverOverlay(bool bShow)
{
	if (!HoverOverlay)
	{
		return;
	}

	if (bShow && !bIsLocked)
	{
		HoverOverlay->SetColorAndOpacity(HoverColor);
		HoverOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		HoverOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_GridSlotWidget::ShowHighlightOverlay(bool bShow, bool bValid)
{
	if (!HighlightOverlay)
	{
		return;
	}

	if (bShow)
	{
		FLinearColor HighlightColor = bValid ? HighlightValidColor : HighlightInvalidColor;
		HighlightOverlay->SetColorAndOpacity(HighlightColor);
		HighlightOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		HighlightOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}


