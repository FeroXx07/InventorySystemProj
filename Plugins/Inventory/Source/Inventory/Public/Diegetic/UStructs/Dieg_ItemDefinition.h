// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_ItemDefinition.generated.h"

class UDieg_ItemFragment;
/**
 * 
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FDieg_ItemDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Display") 
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Display") 
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Categorization") 
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals") 
	TSoftObjectPtr<UStaticMesh> WorldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals") 
	TSoftObjectPtr<UTexture2D> Icon2d;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diegetic Inventory")
	TArray<FIntPoint> DefaultShape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diegetic Inventory")
	FIntPoint DefaultShapeRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stacking", meta=(ClampMin="1")) 
	int32 StackSizeMax = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Functionality")
	TArray<TObjectPtr<UDieg_ItemFragment>> Fragments;
};

