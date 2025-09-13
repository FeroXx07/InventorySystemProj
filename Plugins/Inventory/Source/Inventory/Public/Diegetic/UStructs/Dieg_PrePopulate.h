// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dieg_PrePopulate.generated.h"

class UDieg_ItemDefinitionDataAsset;
/**
 * 
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FDieg_PrePopulate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Pre Populate|Pre Populate Item" /*, Replicated */) 
	TObjectPtr<UDieg_ItemDefinitionDataAsset> ItemDefinitionDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Pre Populate|Pre Populate Item" /*, Replicated */) 
	int32 Quantity{1};
};
