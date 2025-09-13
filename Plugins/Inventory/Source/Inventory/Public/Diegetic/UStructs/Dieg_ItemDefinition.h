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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Display") 
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Display") 
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Categorization") 
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Visuals") 
	TSoftObjectPtr<UStaticMesh> WorldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Visuals") 
	TSoftObjectPtr<UTexture2D> Icon2d;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Item Definition|Diegetic Inventory")
	TArray<FIntPoint> DefaultShape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Item Definition|Diegetic Inventory")
	FIntPoint DefaultShapeRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Stacking", meta=(ClampMin="1")) 
	int32 StackSizeMax = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Game|Dieg|Item Definition|Functionality")
	TArray<TObjectPtr<UDieg_ItemFragment>> Fragments;
};

