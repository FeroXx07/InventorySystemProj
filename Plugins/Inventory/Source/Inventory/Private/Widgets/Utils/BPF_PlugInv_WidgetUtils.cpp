// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/BPF_PlugInv_WidgetUtils.h"

int32 UPlugInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}
