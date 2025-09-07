// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dieg_ItemDefinitionDataAsset.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_ItemInstance.generated.h"

class UDieg_ItemFragment;
class UDieg_ItemDefinitionDataAsset;
/**
 * 
 */

// EditInlineNew + DefaultToInstanced → tells UE this UObject can exist as an instanced subobject (same way components do).
UCLASS(EditInlineNew, DefaultToInstanced)
class INVENTORY_API UDieg_ItemInstance : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pre Populate Item" /*, Replicated */) 
	TObjectPtr<UDieg_ItemDefinitionDataAsset> ItemDefinitionDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item" /*, Replicated */) 
	TArray<TObjectPtr<UDieg_ItemFragment>> ItemFragments; // Instantiated fragments

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item", meta=(ClampMin="1") /*, Replicated */) 
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item" /*, Replicated */) 
	FGameplayTagContainer ItemTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item" /*, Replicated */) 
	bool bIsInitialized{false};
public:
	void Initialize(UDieg_ItemDefinitionDataAsset* Def, int32 InQuantity = 1); // Instantiates fragments

	UFUNCTION(BlueprintCallable)
	UDieg_ItemDefinitionDataAsset* GetItemDefinitionDataAsset() const;

	UFUNCTION(BlueprintCallable)
	const FDieg_ItemDefinition& GetItemDefinition() const;
	
	int32 GetQuantity() const { return Quantity; };
	void SetQuantity(int32 InQuantity) {Quantity = InQuantity;};
	template <typename T> T* GetFragment() const; // Gets a specific fragment instance
	const FGameplayTagContainer& GetTags() const { return ItemTags; }
	
	bool IsEqual(const UDieg_ItemInstance* ToCheck) const;
	bool CanStackWith(const UDieg_ItemInstance* ToCheck) const; // Compares fragment states
};

template <typename T>
T* UDieg_ItemInstance::GetFragment() const
{
	return nullptr;
}
