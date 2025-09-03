// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_ItemFragment.generated.h"

class UDieg_ItemInstance;
/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable, Abstract)
class INVENTORY_API UDieg_ItemFragment : public UObject
{
	GENERATED_BODY()

public:
	// Called when fragment is instanced
	virtual void OnInstanced(UDieg_ItemInstance* OwningInstance) {}
	
	// Called when item is equipped/activated
	virtual void OnItemActivated(UDieg_ItemInstance* OwningInstance) {}
	
	// Called when item is unequipped/deactivated
	virtual void OnItemDeactivated(UDieg_ItemInstance* OwningInstance) {}

	const FGameplayTag& GetFragmentTag() const;
	void SetFragmentTag(const FGameplayTag& Tag);

	// Virtual IsEqual method for child fragment comparison
	virtual bool IsEqual(const UDieg_ItemFragment* Other) const
	{
		if (!Other) return false;
		return FragmentTag == Other->FragmentTag;
	}
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item" /*, Replicated */) 
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};
