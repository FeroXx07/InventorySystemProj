// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "I_PlugInv_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "AC_PlugInv_HighlightableStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_HighlightableStaticMesh : public UStaticMeshComponent, public IPlugInv_Highlightable
{
	GENERATED_BODY()

public:
	// Sets the overlay material to the highlight material.
	virtual void Highlight_Implementation() override;
	
	// Sets the overlay material to nullptr.
	virtual void UnHighlight_Implementation() override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
