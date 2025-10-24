// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inv_Grid.h"

#include "Inv_UtilityLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/GridPanel.h"
#include "Widgets/Inv_Slot.h"

void UInv_Grid::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!IsValid(SlotClass) || !IsDesignTime())
	{
		return;
	}
	
	CreateGrid(TotalSlots, MaxColumns);
}

void UInv_Grid::CreateGrid_Implementation(int32 InTotalSlots, int32 InMaxColumns)
{
	TotalSlots = InTotalSlots;
	MaxColumns = InMaxColumns;

	GridPanel->ClearChildren();
	SlotMap.Empty();
	
	TArray<FIntPoint> Slots = UInv_UtilityLibrary::GetSlotPointsFromTotalColumns(TotalSlots, MaxColumns);
	if (Slots.IsEmpty())
	{
		return;
	}
	for (const FIntPoint& SlotValue : Slots)
	{
		// LOG_DOUBLE_S(5.0f, FColor::Green, "Slot {0}", SlotValue);
		CreateSlot(SlotValue);
	}
}

UInv_Slot* UInv_Grid::CreateSlot_Implementation(const FIntPoint& InGridCoordinates)
{
	UInv_Slot* CreatedSlot = CreateWidget<UInv_Slot>(this, SlotClass);
	
	CreatedSlot->InitializeWithGrid(this);
	
	GridPanel->AddChildToGrid(CreatedSlot, InGridCoordinates.Y, InGridCoordinates.X);
	SlotMap.Add(InGridCoordinates, CreatedSlot);
	
	CreatedSlot->OnHoverSlot.AddDynamic(this, &ThisClass::ChildSlotHover);
	CreatedSlot->OnUnHoverSlot.AddDynamic(this, &ThisClass::ChildSlotUnHovered);
	CreatedSlot->OnPressedSlot.AddDynamic(this, &ThisClass::ChildSlotPressed);
	CreatedSlot->OnReleasedSlot.AddDynamic(this, &ThisClass::ChildSlotReleased);
	
	return CreatedSlot;
}

void UInv_Grid::UpdateHoveringSlots_Implementation(const TArray<FIntPoint>& SlotsCoordinatesToUpdate,
                                                   EInv_SlotState NewStatus, EInv_SlotState ResetStatus)
{
	// UPlugInv_DoubleLogger::Log(5.0f, TEXT("UpdateHoveringSlots. NewStatus: {1}, ResetStatus: {2}"),
	// 			FColor::Yellow, NewStatus, ResetStatus);
	TSet<FIntPoint> LocalSlotsCoordinatesToUpdate;
	LocalSlotsCoordinatesToUpdate.Append(SlotsCoordinatesToUpdate);

	// Difference: Result will contain all elements that are in Set A but are not found in Set B.
	// So in this case: Anything in LocalNewCoordinates that is NOT in SlotsHoveredSet will be cached.
	TArray<FIntPoint> DiffSlotsNew = LocalSlotsCoordinatesToUpdate.Difference(SlotsHoveredSet).Array();
	for (const FIntPoint& DiffSlotNew : DiffSlotsNew)
	{
		if (bool Contains = SlotMap.Contains(DiffSlotNew); TObjectPtr<UInv_Slot> FoundSlot = SlotMap.FindRef(DiffSlotNew))
		{
			FoundSlot->SetState(NewStatus);
		}
	}
	
	TArray<FIntPoint> DiffSlotsOld = SlotsHoveredSet.Difference(LocalSlotsCoordinatesToUpdate).Array();
	for (const FIntPoint& DiffSlotOld : DiffSlotsOld)
	{
		if (TObjectPtr<UInv_Slot>* FoundSlot = SlotMap.Find(DiffSlotOld))
		{
			FoundSlot->Get()->SetState(ResetStatus);
		}
	}
	
	SlotsHoveredSet = LocalSlotsCoordinatesToUpdate;
}

void UInv_Grid::ChildSlotHover_Implementation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UInv_Slot* GridSlot)
{
	OnGridSlotHover.Broadcast(InGeometry, InMouseEvent, GridSlot);
}

void UInv_Grid::ChildSlotUnHovered_Implementation(const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot)
{
	OnGridSlotUnHover.Broadcast(InMouseEvent, GridSlot);
}

void UInv_Grid::ChildSlotPressed_Implementation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UInv_Slot* GridSlot)
{
	OnGridSlotPressed.Broadcast(InGeometry, InMouseEvent, GridSlot);
}

void UInv_Grid::ChildSlotReleased_Implementation(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot)
{
	OnGridSlotReleased.Broadcast(InGeometry, InMouseEvent, GridSlot);
}
