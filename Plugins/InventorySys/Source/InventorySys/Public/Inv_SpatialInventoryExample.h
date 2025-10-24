// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_SpatialInventoryExample.generated.h"

class UInv_InventoryComponent;
class UInv_InventoryContainer;
class UInv_ItemInstance;

/**
 * Example class demonstrating how to use the spatial inventory system
 * This class shows how to create containers, add items with diverse spatial shapes, and manage placement
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYS_API AInv_SpatialInventoryExample : public AActor
{
	GENERATED_BODY()
	
public:	
	AInv_SpatialInventoryExample();

protected:
	virtual void BeginPlay() override;

	// Inventory component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInv_InventoryComponent> InventoryComponent;

	// Example containers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers")
	FName MainContainerName = TEXT("MainContainer");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers")
	FName BackpackContainerName = TEXT("Backpack");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers")
	FName StorageContainerName = TEXT("Storage");

	// Container dimensions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers", meta = (ClampMin = "1"))
	int32 MainContainerWidth = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers", meta = (ClampMin = "1"))
	int32 MainContainerHeight = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers", meta = (ClampMin = "1"))
	int32 BackpackWidth = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers", meta = (ClampMin = "1"))
	int32 BackpackHeight = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers", meta = (ClampMin = "1"))
	int32 StorageWidth = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Containers", meta = (ClampMin = "1"))
	int32 StorageHeight = 8;

public:	
	virtual void Tick(float DeltaTime) override;

	// Container management
	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void InitializeInventorySystem();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void CreateExampleContainers();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	UInv_InventoryContainer* GetMainContainer() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	UInv_InventoryContainer* GetBackpackContainer() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	UInv_InventoryContainer* GetStorageContainer() const;

	// Item management examples
	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void AddExampleItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void CreateRealisticShapes();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void DemonstrateSpatialPlacement();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void TestItemRotation();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void TestContainerTransfer();

	// Utility functions
	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void PrintInventoryStatus();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void DebugPrintAllContainers();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void ClearAllContainers();

	// Spatial inventory examples
	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void TestSpatialOptimization();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void FillContainerWithVariedShapes();

protected:
	// Helper functions for creating example items
	UInv_ItemInstance* CreateExampleItem(const FString& ItemName, const TArray<FIntPoint>& Shape, int32 Quantity = 1);
	
	// Realistic shape creators
	void CreateSword();
	void CreateShield();
	void CreatePotion();
	void CreateArmor();
	void CreateTool();
	void CreateBook();
	void CreateKey();
	void CreateGem();

	// Container references for easy access
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Example")
	TObjectPtr<UInv_InventoryContainer> MainContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Example")
	TObjectPtr<UInv_InventoryContainer> BackpackContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Example")
	TObjectPtr<UInv_InventoryContainer> StorageContainer;
};
