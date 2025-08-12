// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlugInv_CompositeBase.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_CompositeBase : public UUserWidget
{
	GENERATED_BODY()
public:
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag& Tag) { FragmentTag = Tag; }
	virtual void Collapse();
	void Expand();

	using FUncType = TFunction<void(UPlugInv_CompositeBase*)>;
	virtual void ApplyFunction(FUncType Function) {}
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag;
};
