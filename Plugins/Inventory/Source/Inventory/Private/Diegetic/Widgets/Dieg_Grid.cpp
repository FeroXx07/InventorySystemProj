// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Widgets/Dieg_Grid.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Components/GridPanel.h"
#include "Diegetic/Dieg_UtilityLibrary.h"
#include "BPF_PlugInv_DoubleLogger.h"
#include "Diegetic/Widgets/Dieg_Slot.h"

void UDieg_Grid::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!IsValid(SlotClass) || !IsDesignTime())
	{
		return;
	}
	
	CreateEmptyGrid(TotalSlots, MaxColumns);
}

TMap<FIntPoint, TObjectPtr<UDieg_Slot>>& UDieg_Grid::GetSlotMap()
{
	return SlotMap;
}

void UDieg_Grid::CreateEmptyGrid_Implementation(int32 TotalSlots_, int32 MaxColumns_)
{
	TotalSlots = TotalSlots_;
	MaxColumns = MaxColumns_;

	GridPanel->ClearChildren();
	SlotMap.Empty();
	
	TArray<FIntPoint> Slots = UDieg_UtilityLibrary::GetSlotPoints(TotalSlots, MaxColumns);
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

UDieg_Slot* UDieg_Grid::CreateSlot(const FIntPoint& Point)
{
	UDieg_Slot* CreatedSlot = CreateWidget<UDieg_Slot>(this, SlotClass);
	
	CreatedSlot->SetParentGrid(this);
	GridPanel->AddChildToGrid(CreatedSlot, Point.Y, Point.X);
	SlotMap.Add(Point, CreatedSlot);

	CreatedSlot->OnHoverSlot.AddDynamic(this, &ThisClass::ChildSlotHover);
	CreatedSlot->OnUnHoverSlot.AddDynamic(this, &ThisClass::ChildSlotUnHovered);
	CreatedSlot->OnPressedSlot.AddDynamic(this, &ThisClass::ChildSlotPressed);
	CreatedSlot->OnReleasedSlot.AddDynamic(this, &ThisClass::ChildSlotReleased);
	
	return CreatedSlot;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDieg_Grid::ChildSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot)
{
	OnGridSlotHover.Broadcast(InGeometry, InMouseEvent, GridSlot);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDieg_Grid::ChildSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot)
{
	OnGridSlotUnHover.Broadcast(InMouseEvent, GridSlot);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDieg_Grid::ChildSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot)
{
	OnGridSlotPressed.Broadcast(InGeometry, InMouseEvent, GridSlot);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDieg_Grid::ChildSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot)
{
	OnGridSlotReleased.Broadcast(InGeometry, InMouseEvent, GridSlot);
}

void UDieg_Grid::UpdateHoveringSlots(const TArray<FIntPoint>& NewCoordinates, EDieg_SlotStatus NewStatus,
	EDieg_SlotStatus ResetStatus)
{
	// UPlugInv_DoubleLogger::Log(5.0f, TEXT("UpdateHoveringSlots. NewStatus: {1}, ResetStatus: {2}"),
	// 			FColor::Yellow, NewStatus, ResetStatus);
	TSet<FIntPoint> LocalNewCoordinates;
	LocalNewCoordinates.Append(NewCoordinates);

	// Difference: Result will contain all elements that are in Set A but are not found in Set B.
	// So in this case: Anything in LocalNewCoordinates that is NOT in SlotsHoveredSet will be cached.
	TArray<FIntPoint> DiffSlotsNew = LocalNewCoordinates.Difference(SlotsHoveredSet).Array();
	for (const FIntPoint& DiffSlotNew : DiffSlotsNew)
	{
		// Update state in the new ones
		if (bool Contains = SlotMap.Contains(DiffSlotNew); TObjectPtr<UDieg_Slot> FoundSlot = SlotMap.FindRef(DiffSlotNew))
		{
			FoundSlot->SetStatusAndColor(NewStatus);
		}
	}

	// Slots that we want to hide
	TArray<FIntPoint> DiffSlotsOld = SlotsHoveredSet.Difference(LocalNewCoordinates).Array();
	for (const FIntPoint& DiffSlotOld : DiffSlotsOld)
	{
		// Update state in the new ones
		if (TObjectPtr<UDieg_Slot>* FoundSlot = SlotMap.Find(DiffSlotOld))
		{
			FoundSlot->Get()->SetStatusAndColor(ResetStatus);
		}
	}
	
	SlotsHoveredSet = LocalNewCoordinates;
}

void UDieg_Grid::ModifyAllSlotsAppearance(bool IsAppearanceLocked, bool Override, EDieg_SlotStatus OverrideStatus)
{
	// UPlugInv_DoubleLogger::Log(5.0f, TEXT("ModifyAllSlotsAppearance. IsAppearanceLocked: {1}, Override: {2}, OverrideStatus: {3}"),
	// 			FColor::Yellow, Override, OverrideStatus);
	for (const TPair<FIntPoint, TObjectPtr<UDieg_Slot>>& Pair : SlotMap)
	{
		Pair.Value->SetAppearanceLocked(IsAppearanceLocked);

		if (Override)
		{
			Pair.Value->SetStatusAndColor(OverrideStatus);
		}
	}
}





