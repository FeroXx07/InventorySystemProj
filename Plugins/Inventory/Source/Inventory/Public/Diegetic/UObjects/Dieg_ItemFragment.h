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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Fragment|Item", meta = (AllowPrivateAccess = "true")) 
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;

	// Called when fragment is instanced
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual void OnInstanced(UDieg_ItemInstance* OwningInstance) {}
	
	// Called when item is equipped/activated
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual void OnItemActivated(UDieg_ItemInstance* OwningInstance) {}
	
	// Called when item is unequipped/deactivated
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual void OnItemDeactivated(UDieg_ItemInstance* OwningInstance) {}

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	const FGameplayTag& GetFragmentTag() const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	void SetFragmentTag(const FGameplayTag& Tag);

	// Virtual IsEqual method for child fragment comparison
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual bool IsEqual(const UDieg_ItemFragment* Other) const
	{
		if (!Other) return false;
		return FragmentTag == Other->FragmentTag;
	}
};
