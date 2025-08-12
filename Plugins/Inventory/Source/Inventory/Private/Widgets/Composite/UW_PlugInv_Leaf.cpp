// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/UW_PlugInv_Leaf.h"

void UPlugInv_Leaf::ApplyFunction(FUncType Function)
{
	Function(this);
}
