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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Item Instance|Pre Populate Item", meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<UDieg_ItemDefinitionDataAsset> ItemDefinitionDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item", meta = (AllowPrivateAccess = "true")) 
	TArray<TObjectPtr<UDieg_ItemFragment>> ItemFragments; // Instantiated fragments

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item", meta=(ClampMin="1", AllowPrivateAccess = "true")) 
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item" , meta = (AllowPrivateAccess = "true")) 
	FGameplayTagContainer ItemTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item", meta = (AllowPrivateAccess = "true")) 
	bool bIsInitialized{false};

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	void Initialize(UDieg_ItemDefinitionDataAsset* Def, int32 InQuantity = 1); // Instantiates fragments

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	UDieg_ItemDefinitionDataAsset* GetItemDefinitionDataAsset() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	const FDieg_ItemDefinition& GetItemDefinition() const;
	
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	int32 GetQuantity() const { return Quantity; };
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	void SetQuantity(int32 InQuantity) {Quantity = InQuantity;};
	template <typename T> T* GetFragment() const; // Gets a specific fragment instance
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	const FGameplayTagContainer& GetTags() const { return ItemTags; }
	
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	bool IsEqual(const UDieg_ItemInstance* ToCheck) const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	bool CanStackWith(const UDieg_ItemInstance* ToCheck) const; // Compares fragment states
};

template <typename T>
T* UDieg_ItemInstance::GetFragment() const
{
	return nullptr;
}
