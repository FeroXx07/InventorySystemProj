// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_ItemDefinition.h"
#include "Engine/DataAsset.h"
#include "Inv_ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_ItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Inv|ItemDataAsset")
	TObjectPtr<UInv_ItemDefinition> ItemDefinition;
};
