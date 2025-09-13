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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Item Definition Data Asset|Definition", meta = (AllowPrivateAccess = "true"))
	FDieg_ItemDefinition ItemDefinition;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext) override;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Definition Data Asset")
	static void SetItemDefinitionShapeRoot(FDieg_ItemDefinition& ItemDef);
};
