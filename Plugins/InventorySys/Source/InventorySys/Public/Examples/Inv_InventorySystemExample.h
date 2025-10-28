// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_InventorySystemExample.generated.h"

class UInv_InventoryComponent;
class UInv_InventoryController;
class UInv_SpatialGridWidget;
class UInv_ItemInstance;

/**
 * Example class demonstrating the clean 3-layer inventory architecture:
 * - Data Layer: InventoryComponent + Containers
 * - Logic Layer: InventoryController (owned by Component)
 * - UI Layer: SpatialGridWidget (binds to Controller)
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYS_API AInv_InventorySystemExample : public AActor
{
	GENERATED_BODY()
	
public:	
	AInv_InventorySystemExample();

protected:
	virtual void BeginPlay() override;

	// ============================================
	// CORE SYSTEM (Component owns Controller)
	// ============================================
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|System")
	TObjectPtr<UInv_InventoryComponent> InventoryComponent;

	// ============================================
	// UI COMPONENTS
	// ============================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|UI")
	TSubclassOf<UInv_SpatialGridWidget> GridWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|UI")
	TObjectPtr<UInv_SpatialGridWidget> GridWidget;

	// ============================================
	// SETTINGS
	// ============================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Settings")
	bool bAutoInitialize = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Settings")
	FName MainContainerName = TEXT("MainContainer");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Settings")
	int32 MainContainerWidth = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Settings")
	int32 MainContainerHeight = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Settings")
	bool bCreateExampleItems = false;

public:	
	virtual void Tick(float DeltaTime) override;

	// ============================================
	// SYSTEM MANAGEMENT
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void CreateUI();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void BindEvents();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void UnbindEvents();

	// ============================================
	// EXAMPLE OPERATIONS
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void CreateExampleItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void TestItemPlacement();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void TestItemRotation();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void TestItemMovement();

	// ============================================
	// EVENT HANDLERS (Listen to Controller)
	// ============================================
	
	UFUNCTION()
	void OnItemPlaced(UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation);

	UFUNCTION()
	void OnItemRemoved(UInv_ItemInstance* Item);

	UFUNCTION()
	void OnItemMoved(UInv_ItemInstance* Item, const FIntPoint& NewPosition, float NewRotation);

	UFUNCTION()
	void OnDragStarted(UInv_ItemInstance* Item, const FIntPoint& StartPosition);

	UFUNCTION()
	void OnDragEnded(UInv_ItemInstance* Item, bool bSuccess);

	// ============================================
	// UTILITY FUNCTIONS
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void PrintSystemStatus();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Example")
	void ClearAllItems();

protected:
	// Helper functions
	UInv_ItemInstance* CreateTestItem(const FString& ItemName, const TArray<FIntPoint>& Shape);
};
