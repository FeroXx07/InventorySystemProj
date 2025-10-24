// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Inv_ItemInstance.generated.h"

class UInv_ItemDefinition;
class UInv_ItemDataAsset;
/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_ItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|ItemInstance", meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<UInv_ItemDataAsset> ItemDefinitionDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Inv|ItemInstance", meta = (AllowPrivateAccess = "true")) 
	TArray<TObjectPtr<UObject>> ItemFragments;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Inv|ItemInstance", meta=(ClampMin="1", AllowPrivateAccess = "true")) 
	int32 Quantity = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Inv|ItemInstance" , meta = (AllowPrivateAccess = "true")) 
	FGameplayTagContainer ItemTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|ItemInstance", meta = (AllowPrivateAccess = "true")) 
	bool bIsInitialized{false};
	
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	void Initialize(UInv_ItemDataAsset* InItemDataAsset, int32 InQuantity = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	const UInv_ItemDataAsset* GetItemDefinitionDataAsset() const;
	
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	const UInv_ItemDefinition* GetItemDefinition() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	int32 GetQuantity() const { return Quantity; };
	
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	void SetQuantity(int32 InQuantity) {Quantity = InQuantity;};
	
	// template <typename T> T* GetFragment() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	const FGameplayTagContainer& GetTags() const { return ItemTags; }
	
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	bool IsEqual(const UInv_ItemInstance* OtherItemInstance) const;
	
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemInstance")
	bool CanStackWith(const UInv_ItemInstance* OtherItemInstance) const;
};
