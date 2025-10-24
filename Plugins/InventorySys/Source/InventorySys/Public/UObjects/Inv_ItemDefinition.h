// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Inv_ItemDefinition.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_ItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Inv|ItemDefinition") 
	FText Name = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Inv|ItemDefinition") 
	FText Description = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Inv|ItemDefinition") 
	FGameplayTag ItemTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Inv|ItemDefinition") 
	TSoftObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Inv|ItemDefinition") 
	TSoftObjectPtr<UTexture2D> Icon2d;
	
	// Spatial shape definition for Tetris-style placement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|ItemDefinition|Spatial")
	TArray<FIntPoint> DefaultShape;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|ItemDefinition|Spatial")
	bool bCanRotate = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|ItemDefinition|Spatial", meta = (ClampMin = "1"))
	int32 ShapeWidth = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|ItemDefinition|Spatial", meta = (ClampMin = "1"))
	int32 ShapeHeight = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|ItemDefinition|Spatial")
	FColor ShapeColor = FColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Inv|ItemDefinition", meta=(ClampMin="1")) 
	int32 StackSizeMax = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Game|Inv|ItemDefinition")
	TArray<TObjectPtr<UObject>> Fragments;

	// Spatial utility functions
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemDefinition|Spatial")
	TArray<FIntPoint> GetShapeAtRotation(float Rotation) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemDefinition|Spatial")
	TArray<FIntPoint> GetShapeAtPosition(const FIntPoint& Position, float Rotation = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemDefinition|Spatial")
	bool IsValidShape() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemDefinition|Spatial")
	void AutoCalculateShapeDimensions();

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|ItemDefinition|Spatial")
	FIntPoint GetShapeBounds() const;
};
