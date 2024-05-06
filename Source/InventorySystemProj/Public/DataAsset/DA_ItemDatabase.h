// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimaryDataAsset/PDA_ItemCore.h"
#include "Engine/DataAsset.h"
#include "DA_ItemDatabase.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMPROJ_API UItemDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostInitProperties() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<TObjectPtr<UItemCore>> DataAssets;

private:
	void PopulateDatabaseArray();
};
