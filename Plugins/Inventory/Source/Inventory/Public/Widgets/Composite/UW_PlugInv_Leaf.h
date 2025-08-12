// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_PlugInv_CompositeBase.h"
#include "UW_PlugInv_Leaf.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_Leaf : public UPlugInv_CompositeBase
{
	GENERATED_BODY()
public:

	// Current implementation is data telling the widget what to do and behave.
	// Better implementation would be fetch data (Manifest) and query for what the widget needs and apply behaviour.
	virtual void ApplyFunction(FUncType Function) override;
};
