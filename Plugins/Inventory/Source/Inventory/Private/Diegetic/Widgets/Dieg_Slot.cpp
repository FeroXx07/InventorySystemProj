// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Widgets/Dieg_Slot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Diegetic/Widgets/Dieg_Grid.h"


void UDieg_Slot::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitializeDefaults();
}

void UDieg_Slot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnHoverSlot.Broadcast(InGeometry, InMouseEvent, this);
	
	if (IsNotLocked() && !IsAppearanceLocked())
	{
		SetStatusAndColor(EDieg_SlotStatus::Hover, true);
	}
}

void UDieg_Slot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	OnUnHoverSlot.Broadcast(InMouseEvent, this);
	
	if (IsNotLocked() && !IsAppearanceLocked())
	{
		SetStatusAndColor(EDieg_SlotStatus::None, true);
	}
}

FReply UDieg_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnPressedSlot.Broadcast(InGeometry, InMouseEvent, this);
	// return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();
}

FReply UDieg_Slot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnReleasedSlot.Broadcast(InGeometry, InMouseEvent, this);
	// return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();}

void UDieg_Slot::SetParentGrid(const TObjectPtr<UDieg_Grid>& Grid)
{
	ParentGrid = Grid;
}

void UDieg_Slot::InitializeDefaults_Implementation()
{
	// Set SizeBox values from defaults
	if (IsValid(SizeBox_Root))
	{
		SizeBox_Root->SetWidthOverride(SlotSize.X);
		SizeBox_Root->SetHeightOverride(SlotSize.Y);
	}

	constexpr bool ChangeAppearance = true;
	SetStatusAndColor(SlotStatus, ChangeAppearance);
}

FIntPoint UDieg_Slot::GetCoordinatesInGrid_Implementation()
{
	const UGridSlot* GridSlot = UWidgetLayoutLibrary::SlotAsGridSlot(this);
	// X: Columns, Y: Rows
	LastUpdatedCoordinate = FIntPoint(GridSlot->GetColumn(), GridSlot->GetRow());
	return LastUpdatedCoordinate;
}

void UDieg_Slot::SetStatusAndColor_Implementation(EDieg_SlotStatus Status, bool bChangeAppearance)
{
	this->SlotStatus = Status;
	const FLinearColor ColorToSet = SlotFillColor.FindChecked(Status);
	Image_Fill->SetColorAndOpacity(ColorToSet);
}
