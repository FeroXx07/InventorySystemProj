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





