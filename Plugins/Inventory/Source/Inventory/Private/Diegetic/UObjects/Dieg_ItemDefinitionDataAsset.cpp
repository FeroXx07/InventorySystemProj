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
		// Find the top-left most point as the root
		FIntPoint TopLeftMost = ItemDef.DefaultShape[0];
		
		for (const FIntPoint& Point : ItemDef.DefaultShape)
		{
			// Find the point with smallest Y (topmost), then smallest X (leftmost)
			if (Point.Y < TopLeftMost.Y || (Point.Y == TopLeftMost.Y && Point.X < TopLeftMost.X))
			{
				TopLeftMost = Point;
			}
		}
		
		ItemDef.DefaultShapeRoot = TopLeftMost;
	}
}
