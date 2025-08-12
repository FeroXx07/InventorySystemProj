// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemDescription/UW_PlugInv_ItemDescription.h"

#include "Components/SizeBox.h"

FVector2D UPlugInv_ItemDescription::GetBoxSize() const
{
	return SizeBox->GetDesiredSize();
}
