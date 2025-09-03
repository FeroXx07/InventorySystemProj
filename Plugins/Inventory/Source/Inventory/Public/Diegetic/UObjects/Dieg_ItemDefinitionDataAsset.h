// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Diegetic/UStructs/Dieg_ItemDefinition.h"
#include "Engine/DataAsset.h"
#include "Dieg_ItemDefinitionDataAsset.generated.h"

class UDieg_ItemFragment;
/**
 * 
 */
UCLASS()
class INVENTORY_API UDieg_ItemDefinitionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext) override;
public:
	UPROPERTY(EditAnywhere)
	FDieg_ItemDefinition ItemDefinition;

	static void SetItemDefinitionRoot(FDieg_ItemDefinition& ItemDef);
};
