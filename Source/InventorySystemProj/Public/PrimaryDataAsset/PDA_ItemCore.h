// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFunctionLibrary/BPF_DataLibrary.h"
#include "Engine/DataAsset.h"
#include "PDA_ItemCore.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class INVENTORYSYSTEMPROJ_API UItemCore : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	FItem Item;
};

