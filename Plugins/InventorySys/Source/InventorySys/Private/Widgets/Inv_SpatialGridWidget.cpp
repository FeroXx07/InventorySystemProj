// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inv_SpatialGridWidget.h"
#include "Widgets/Inv_GridSlotWidget.h"
#include "Components/Inv_InventoryComponent.h"
#include "Controllers/Inv_InventoryController.h"
#include "UObjects/Inv_InventoryContainer.h"
#include "UObjects/Inv_ItemInstance.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

// ============================================
// LIFECYCLE
// ============================================

void UInv_SpatialGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInv_SpatialGridWidget::NativeDestruct()
{
	// Widget cleans up its own bindings
	UnbindFromController();
	
	Super::NativeDestruct();
}

// ============================================
// INITIALIZATION
// ============================================

void UInv_SpatialGridWidget::SetInventoryComponent(UInv_InventoryComponent* InComponent)
{
	// Cleanup old bindings
	UnbindFromController();

	// Set new references
	InventoryComponent = InComponent;

	if (InventoryComponent)
	{
		// Cache references
		Controller = InventoryComponent->GetController();
		CurrentContainer = Controller ? Controller->GetCurrentContainer() : nullptr;

		// Bind to controller events
		BindToController();

		// Set grid size from container
		if (CurrentContainer)
		{
			SetGridSize(CurrentContainer->GridWidth, CurrentContainer->GridHeight);
		}

		// Create grid with slots
		CreateGrid();
		RenderGrid();
	}
}

void UInv_SpatialGridWidget::SetGridSize(int32 Width, int32 Height)
{
	GridWidth = Width;
	GridHeight = Height;
}

// ============================================
// CONTROLLER BINDING (Widget binds to Controller)
// ============================================

void UInv_SpatialGridWidget::BindToController()
{
	if (!Controller)
	{
		return;
	}

	// Widget listens to Controller events
	Controller->OnItemPlaced.AddDynamic(this, &UInv_SpatialGridWidget::OnItemPlaced);
	Controller->OnItemRemoved.AddDynamic(this, &UInv_SpatialGridWidget::OnItemRemoved);
	Controller->OnItemMoved.AddDynamic(this, &UInv_SpatialGridWidget::OnItemMoved);
	Controller->OnDragStarted.AddDynamic(this, &UInv_SpatialGridWidget::OnDragStarted);
	Controller->OnDragEnded.AddDynamic(this, &UInv_SpatialGridWidget::OnDragEnded);
	Controller->OnDragValidityChanged.AddDynamic(this, &UInv_SpatialGridWidget::OnDragValidityChanged);
	Controller->OnItemSelected.AddDynamic(this, &UInv_SpatialGridWidget::OnItemSelected);
	Controller->OnItemDeselected.AddDynamic(this, &UInv_SpatialGridWidget::OnItemDeselected);
	Controller->OnContainerChanged.AddDynamic(this, &UInv_SpatialGridWidget::OnContainerChanged);
}

void UInv_SpatialGridWidget::UnbindFromController()
{
	if (!Controller)
	{
		return;
	}

	// Widget removes its bindings
	Controller->OnItemPlaced.RemoveDynamic(this, &UInv_SpatialGridWidget::OnItemPlaced);
	Controller->OnItemRemoved.RemoveDynamic(this, &UInv_SpatialGridWidget::OnItemRemoved);
	Controller->OnItemMoved.RemoveDynamic(this, &UInv_SpatialGridWidget::OnItemMoved);
	Controller->OnDragStarted.RemoveDynamic(this, &UInv_SpatialGridWidget::OnDragStarted);
	Controller->OnDragEnded.RemoveDynamic(this, &UInv_SpatialGridWidget::OnDragEnded);
	Controller->OnDragValidityChanged.RemoveDynamic(this, &UInv_SpatialGridWidget::OnDragValidityChanged);
	Controller->OnItemSelected.RemoveDynamic(this, &UInv_SpatialGridWidget::OnItemSelected);
	Controller->OnItemDeselected.RemoveDynamic(this, &UInv_SpatialGridWidget::OnItemDeselected);
	Controller->OnContainerChanged.RemoveDynamic(this, &UInv_SpatialGridWidget::OnContainerChanged);
}

// ============================================
// CONTROLLER EVENT HANDLERS (Controller → Widget)
// ============================================

void UInv_SpatialGridWidget::OnItemPlaced(UInv_ItemInstance* Item, FIntPoint Position, float Rotation)
{
	// Re-render entire grid
	RenderGrid();
}

void UInv_SpatialGridWidget::OnItemRemoved(UInv_ItemInstance* Item)
{
	// Re-render entire grid
	RenderGrid();
}

void UInv_SpatialGridWidget::OnItemMoved(UInv_ItemInstance* Item, FIntPoint NewPosition, float NewRotation)
{
	// Re-render entire grid
	RenderGrid();
}

void UInv_SpatialGridWidget::OnDragStarted(UInv_ItemInstance* Item, FIntPoint StartPosition)
{
	// Visual feedback: item is being dragged
	RenderGrid();
}

void UInv_SpatialGridWidget::OnDragEnded(UInv_ItemInstance* Item, bool bSuccess)
{
	// Clear highlights and re-render
	ClearHighlights();
	RenderGrid();
}

void UInv_SpatialGridWidget::OnDragValidityChanged(FIntPoint Position, bool bValid)
{
	// Update drag preview
	if (Controller && Controller->IsDragging())
	{
		TArray<FIntPoint> Shape = Controller->GetDraggedItemShapeAtPosition(Position);
		HighlightSlots(Shape, bValid);
	}
}

void UInv_SpatialGridWidget::OnItemSelected(UInv_ItemInstance* Item, FIntPoint Position)
{
	// Highlight selected item
	RenderGrid();
}

void UInv_SpatialGridWidget::OnItemDeselected(UInv_ItemInstance* Item)
{
	// Remove selection highlight
	RenderGrid();
}

void UInv_SpatialGridWidget::OnContainerChanged(UInv_InventoryContainer* OldContainer, UInv_InventoryContainer* NewContainer)
{
	CurrentContainer = NewContainer;
	
	if (CurrentContainer)
	{
		SetGridSize(CurrentContainer->GridWidth, CurrentContainer->GridHeight);
		CreateGrid();
		RenderGrid();
	}
}

// ============================================
// SLOT EVENT HANDLERS (Slots → Grid → Controller)
// ============================================

void UInv_SpatialGridWidget::BindSlotEvents(UInv_GridSlotWidget* Slot)
{
	if (!Slot)
	{
		return;
	}

	// Grid listens to slot events
	Slot->OnSlotClicked.AddDynamic(this, &UInv_SpatialGridWidget::OnSlotClicked);
	Slot->OnSlotRightClicked.AddDynamic(this, &UInv_SpatialGridWidget::OnSlotRightClicked);
	Slot->OnSlotHovered.AddDynamic(this, &UInv_SpatialGridWidget::OnSlotHovered);
	Slot->OnSlotUnhovered.AddDynamic(this, &UInv_SpatialGridWidget::OnSlotUnhovered);
	Slot->OnSlotDragDetected.AddDynamic(this, &UInv_SpatialGridWidget::OnSlotDragDetected);
}

void UInv_SpatialGridWidget::OnSlotClicked(UInv_GridSlotWidget* Slot, FIntPoint Position, bool bLeftButton)
{
	if (!Controller)
	{
		return;
	}

	// Forward to controller
	Controller->HandleCellClicked(Position.X, Position.Y, bLeftButton, false);
}

void UInv_SpatialGridWidget::OnSlotRightClicked(UInv_GridSlotWidget* Slot, FIntPoint Position, UInv_ItemInstance* Item)
{
	if (!Controller)
	{
		return;
	}

	// Forward to controller
	Controller->HandleCellRightClicked(Position.X, Position.Y);
}

void UInv_SpatialGridWidget::OnSlotHovered(UInv_GridSlotWidget* Slot, FIntPoint Position)
{
	if (!Controller)
	{
		return;
	}

	HoveredCell = Position;

	// Forward to controller
	Controller->HandleMouseMoved(Position);
}

void UInv_SpatialGridWidget::OnSlotUnhovered(UInv_GridSlotWidget* Slot, FIntPoint Position)
{
	if (HoveredCell == Position)
	{
		HoveredCell = FIntPoint(-1, -1);
	}
}

void UInv_SpatialGridWidget::OnSlotDragDetected(UInv_GridSlotWidget* Slot, FIntPoint Position, UInv_ItemInstance* Item)
{
	if (!Controller || !Item)
	{
		return;
	}

	// Start drag in controller
	Controller->StartDrag(Item, Position);
}

// ============================================
// INPUT HANDLING (Fallback if slots don't handle it)
// ============================================

FReply UInv_SpatialGridWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Slots handle most input, this is just fallback
	return FReply::Unhandled();
}

FReply UInv_SpatialGridWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

FReply UInv_SpatialGridWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

FReply UInv_SpatialGridWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!Controller)
	{
		return FReply::Unhandled();
	}

	float WheelDelta = InMouseEvent.GetWheelDelta();

	// Grid handles rotation
	Controller->HandleRotationRequested(WheelDelta * 90.0f);

	return FReply::Handled();
}

void UInv_SpatialGridWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	HoveredCell = FIntPoint(-1, -1);
	ClearHighlights();
}

// ============================================
// GRID MANAGEMENT
// ============================================

void UInv_SpatialGridWidget::CreateGrid()
{
	if (!GridPanel || !SlotWidgetClass)
	{
		return;
	}

	// Clear existing grid
	GridPanel->ClearChildren();
	SlotWidgets.Empty();
	SlotWidgets.SetNum(GridHeight);

	// Set slot padding for the entire grid
	GridPanel->SetSlotPadding(SlotPadding);

	// Create slot widgets
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		SlotWidgets[Y].SetNum(GridWidth);
		
		for (int32 X = 0; X < GridWidth; ++X)
		{
			CreateSlot(X, Y);
		}
	}
}

void UInv_SpatialGridWidget::UpdateGrid()
{
	RenderGrid();
}

void UInv_SpatialGridWidget::ClearGrid()
{
	if (GridPanel)
	{
		GridPanel->ClearChildren();
	}
	
	SlotWidgets.Empty();
}

void UInv_SpatialGridWidget::RenderGrid()
{
	if (!CurrentContainer)
	{
		return;
	}

	// Update each slot based on container state
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			UpdateSlot(X, Y);
		}
	}
}

void UInv_SpatialGridWidget::UpdateItemVisualization()
{
	// Items are rendered within slots
	RenderGrid();
}

void UInv_SpatialGridWidget::ClearHighlights()
{
	// Clear highlights on all slots
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			UInv_GridSlotWidget* Slot = GetSlotAt(FIntPoint(X, Y));
			if (Slot)
			{
				Slot->ClearHighlight();
			}
		}
	}
}

// ============================================
// INTERNAL HELPERS
// ============================================

void UInv_SpatialGridWidget::CreateSlot(int32 X, int32 Y)
{
	if (!GridPanel || !SlotWidgetClass)
	{
		return;
	}

	// Create slot widget
	UInv_GridSlotWidget* SlotWidget = CreateWidget<UInv_GridSlotWidget>(this, SlotWidgetClass);
	
	if (SlotWidget)
	{
		// Initialize slot
		SlotWidget->Initialize(FIntPoint(X, Y));
		
		// Bind slot events
		BindSlotEvents(SlotWidget);
		
		// Add to grid panel
		UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(SlotWidget, Y, X);
		
		if (GridSlot)
		{
			// Configure slot sizing
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
		
		// Store reference
		SlotWidgets[Y][X] = SlotWidget;
	}
}

void UInv_SpatialGridWidget::UpdateSlot(int32 X, int32 Y)
{
	if (!CurrentContainer)
	{
		return;
	}

	UInv_GridSlotWidget* Slot = GetSlotAt(FIntPoint(X, Y));
	if (!Slot)
	{
		return;
	}

	FIntPoint Position(X, Y);
	
	// Check if position is occupied (read directly from Container - no duplication!)
	bool bOccupied = CurrentContainer->IsGridPositionOccupied(Position);
	Slot->SetOccupied(bOccupied);

	// Find item at this position (if any)
	UInv_ItemInstance* ItemAtPosition = nullptr;
	for (const FInv_ItemPlacement& Placement : CurrentContainer->ItemPlacements)
	{
		if (Placement.GridPosition == Position)
		{
			ItemAtPosition = Placement.ItemInstance;
			break;
		}
	}

	// Update slot with item
	Slot->SetItem(ItemAtPosition);
}

void UInv_SpatialGridWidget::HighlightSlots(const TArray<FIntPoint>& Positions, bool bValid)
{
	// First, clear all highlights
	ClearHighlights();

	// Then highlight specified positions
	for (const FIntPoint& Position : Positions)
	{
		UInv_GridSlotWidget* Slot = GetSlotAt(Position);
		if (Slot)
		{
			Slot->SetHighlight(true, bValid);
		}
	}
}

UInv_GridSlotWidget* UInv_SpatialGridWidget::GetSlotAt(FIntPoint Position) const
{
	if (SlotWidgets.IsValidIndex(Position.Y) && SlotWidgets[Position.Y].IsValidIndex(Position.X))
	{
		return SlotWidgets[Position.Y][Position.X];
	}
	return nullptr;
}

// ============================================
// UTILITY FUNCTIONS
// ============================================

FIntPoint UInv_SpatialGridWidget::GetGridPositionFromMouse(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const
{
	FVector2D LocalPosition = Geometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	
	// Account for padding between slots
	float EffectiveSlotWidth = CellSize + SlotPadding.X;
	float EffectiveSlotHeight = CellSize + SlotPadding.Y;
	
	int32 X = FMath::FloorToInt(LocalPosition.X / EffectiveSlotWidth);
	int32 Y = FMath::FloorToInt(LocalPosition.Y / EffectiveSlotHeight);

	return FIntPoint(X, Y);
}

FVector2D UInv_SpatialGridWidget::GetWidgetPositionFromGrid(FIntPoint GridPosition) const
{
	// Account for padding between slots
	float EffectiveSlotWidth = CellSize + SlotPadding.X;
	float EffectiveSlotHeight = CellSize + SlotPadding.Y;
	
	return FVector2D(GridPosition.X * EffectiveSlotWidth, GridPosition.Y * EffectiveSlotHeight);
}

bool UInv_SpatialGridWidget::IsPositionInGrid(FIntPoint Position) const
{
	return Position.X >= 0 && Position.X < GridWidth && Position.Y >= 0 && Position.Y < GridHeight;
}
