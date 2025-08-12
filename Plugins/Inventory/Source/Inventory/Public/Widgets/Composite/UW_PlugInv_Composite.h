// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_PlugInv_CompositeBase.h"
#include "UW_PlugInv_Composite.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_Composite : public UPlugInv_CompositeBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void ApplyFunction(FUncType Function) override;
	virtual void Collapse() override;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UPlugInv_CompositeBase>> Children;
};
