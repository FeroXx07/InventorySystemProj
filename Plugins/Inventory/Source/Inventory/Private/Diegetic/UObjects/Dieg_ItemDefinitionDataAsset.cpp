// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/UObjects/Dieg_ItemDefinitionDataAsset.h"

#include "UObject/ObjectSaveContext.h"

void UDieg_ItemDefinitionDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetItemDefinitionShapeRoot(ItemDefinition);
}

void UDieg_ItemDefinitionDataAsset::PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext)
{
	Super::PostSaveRoot(ObjectSaveContext);

	SetItemDefinitionShapeRoot(ItemDefinition);
}

void UDieg_ItemDefinitionDataAsset::SetItemDefinitionShapeRoot(FDieg_ItemDefinition& ItemDef)
{
	if (!ItemDef.DefaultShape.IsEmpty())
	{
		ItemDef.DefaultShapeRoot = ItemDef.DefaultShape[0];
	}
}
