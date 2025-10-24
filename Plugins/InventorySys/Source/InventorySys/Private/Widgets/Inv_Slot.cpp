// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inv_Slot.h"

#include "Components/Image.h"

void UInv_Slot::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UInv_Slot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (IsStateMutable())
	{
		SetState(EInv_SlotState::Hover);
	}
	
	OnHoverSlot.Broadcast(InGeometry, InMouseEvent, this);
}

void UInv_Slot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (IsStateMutable())
	{
		SetState(EInv_SlotState::None);
	}
	
	OnUnHoverSlot.Broadcast(InMouseEvent, this);
}

FReply UInv_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (IsStateMutable() && GetState() != EInv_SlotState::Occupied)
	{
		SetState(EInv_SlotState::Pressed);
	}
	
	OnPressedSlot.Broadcast(InGeometry, InMouseEvent, this);
	return FReply::Handled();
}

FReply UInv_Slot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (IsStateMutable() && GetState() != EInv_SlotState::Occupied)
	{
		SetState(EInv_SlotState::None);
	}
	
	OnReleasedSlot.Broadcast(InGeometry, InMouseEvent, this);
	return FReply::Handled();
}

void UInv_Slot::SetState(const EInv_SlotState InState, bool UpdateColor)
{
	this->State = InState;
	const FLinearColor ColorToSet = FillColor.FindChecked(InState);
	Image_Fill->SetColorAndOpacity(ColorToSet);
}

void UInv_Slot::InitializeDefaults_Implementation()
{
}

void UInv_Slot::InitializeWithGrid_Implementation(UInv_Grid* Grid)
{
}

