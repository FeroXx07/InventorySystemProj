// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_PlugInv_WidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Util function to get a one-dimensional position from a two-dimensional position (ex: grid).
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
};
