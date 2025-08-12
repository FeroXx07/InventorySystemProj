// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/UW_PlugInv_CompositeBase.h"

void UPlugInv_CompositeBase::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UPlugInv_CompositeBase::Expand()
{
	SetVisibility(ESlateVisibility::Visible);
}
