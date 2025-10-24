// Fill out your copyright notice in the Description page of Project Settings.

#include "Inv_SpatialInventoryExample.h"
#include "Components/Inv_InventoryComponent.h"
#include "UObjects/Inv_InventoryContainer.h"
#include "UObjects/Inv_ItemInstance.h"
#include "UObjects/Inv_ItemDefinition.h"
#include "UObjects/Inv_ItemDataAsset.h"
#include "Inv_SpatialUtilityLibrary.h"
#include "Engine/Engine.h"

AInv_SpatialInventoryExample::AInv_SpatialInventoryExample()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create inventory component
	InventoryComponent = CreateDefaultSubobject<UInv_InventoryComponent>(TEXT("InventoryComponent"));
}

void AInv_SpatialInventoryExample::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize the inventory system
	InitializeInventorySystem();
}

void AInv_SpatialInventoryExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInv_SpatialInventoryExample::InitializeInventorySystem()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Initialize the spatial inventory system
	InventoryComponent->InitializeSpatialInventory();
	
	// Create example containers
	CreateExampleContainers();
	
	UE_LOG(LogTemp, Warning, TEXT("Spatial Inventory System Initialized"));
}

void AInv_SpatialInventoryExample::CreateExampleContainers()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Create main container
	MainContainer = InventoryComponent->CreateContainer(
		MainContainerName,
		FText::FromString(TEXT("Main Inventory")),
		MainContainerWidth,
		MainContainerHeight
	);

	// Create backpack container
	BackpackContainer = InventoryComponent->CreateContainer(
		BackpackContainerName,
		FText::FromString(TEXT("Backpack")),
		BackpackWidth,
		BackpackHeight
	);

	// Create storage container
	StorageContainer = InventoryComponent->CreateContainer(
		StorageContainerName,
		FText::FromString(TEXT("Storage Chest")),
		StorageWidth,
		StorageHeight
	);

	UE_LOG(LogTemp, Warning, TEXT("Created %d containers"), InventoryComponent->GetContainerCount());
}

UInv_InventoryContainer* AInv_SpatialInventoryExample::GetMainContainer() const
{
	return InventoryComponent ? InventoryComponent->GetContainer(MainContainerName) : nullptr;
}

UInv_InventoryContainer* AInv_SpatialInventoryExample::GetBackpackContainer() const
{
	return InventoryComponent ? InventoryComponent->GetContainer(BackpackContainerName) : nullptr;
}

UInv_InventoryContainer* AInv_SpatialInventoryExample::GetStorageContainer() const
{
	return InventoryComponent ? InventoryComponent->GetContainer(StorageContainerName) : nullptr;
}

void AInv_SpatialInventoryExample::AddExampleItems()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Create realistic shapes
	CreateRealisticShapes();
	
	UE_LOG(LogTemp, Warning, TEXT("Added example spatial items"));
}

void AInv_SpatialInventoryExample::CreateRealisticShapes()
{
	// Create various realistic item shapes
	CreateSword();
	CreateShield();
	CreatePotion();
	CreateArmor();
	CreateTool();
	CreateBook();
	CreateKey();
	CreateGem();
}

void AInv_SpatialInventoryExample::DemonstrateSpatialPlacement()
{
	if (!MainContainer)
	{
		return;
	}

	// Get all available positions
	TArray<FIntPoint> AvailablePositions = UInv_SpatialUtilityLibrary::GetAvailablePositions(MainContainer);
	
	UE_LOG(LogTemp, Warning, TEXT("Available positions: %d"), AvailablePositions.Num());
	
	// Print container status
	PrintInventoryStatus();
}

void AInv_SpatialInventoryExample::TestItemRotation()
{
	if (!MainContainer)
	{
		return;
	}

	// Create a sword item
	UInv_ItemInstance* SwordItem = CreateExampleItem(TEXT("Sword"), {
		FIntPoint(0, 0),
		FIntPoint(0, 1),
		FIntPoint(0, 2),
		FIntPoint(1, 1)
	});

	if (SwordItem)
	{
		// Test placement at different rotations
		for (float Rotation = 0.0f; Rotation < 360.0f; Rotation += 90.0f)
		{
			FIntPoint TestPosition(0, 0);
			bool bCanPlace = UInv_SpatialUtilityLibrary::CanPlaceItemAt(MainContainer, SwordItem, TestPosition, Rotation);
			
			UE_LOG(LogTemp, Warning, TEXT("Sword at rotation %.1f: %s"), Rotation, bCanPlace ? TEXT("Can Place") : TEXT("Cannot Place"));
		}
	}
}

void AInv_SpatialInventoryExample::TestContainerTransfer()
{
	if (!MainContainer || !BackpackContainer)
	{
		return;
	}

	// Create a simple item
	UInv_ItemInstance* TestItem = CreateExampleItem(TEXT("Test Item"), {
		FIntPoint(0, 0),
		FIntPoint(1, 0)
	});

	if (TestItem)
	{
		// Try to place in main container
		bool bPlacedInMain = InventoryComponent->TryAddItemToContainer(TestItem, MainContainerName, FIntPoint(0, 0));
		
		if (bPlacedInMain)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item placed in main container"));
			
			// Try to move to backpack
			bool bMovedToBackpack = InventoryComponent->TryMoveItemBetweenContainers(
				TestItem, MainContainerName, BackpackContainerName, FIntPoint(0, 0)
			);
			
			if (bMovedToBackpack)
			{
				UE_LOG(LogTemp, Warning, TEXT("Item moved to backpack"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to move item to backpack"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to place item in main container"));
		}
	}
}

void AInv_SpatialInventoryExample::PrintInventoryStatus()
{
	if (!InventoryComponent)
	{
		return;
	}

	TArray<FName> ContainerNames = InventoryComponent->GetContainerNames();
	
	UE_LOG(LogTemp, Warning, TEXT("=== Inventory Status ==="));
	UE_LOG(LogTemp, Warning, TEXT("Total Containers: %d"), ContainerNames.Num());
	
	for (const FName& ContainerName : ContainerNames)
	{
		UInv_InventoryContainer* Container = InventoryComponent->GetContainer(ContainerName);
		if (Container)
		{
			float FillPercentage = Container->GetFillPercentage();
			int32 AvailableSlots = Container->GetAvailableSlotCount();
			int32 TotalSlots = Container->GetTotalSlotCount();
			
			UE_LOG(LogTemp, Warning, TEXT("Container: %s - Fill: %.1f%% (%d/%d slots)"), 
				*ContainerName.ToString(), FillPercentage * 100.0f, TotalSlots - AvailableSlots, TotalSlots);
		}
	}
}

void AInv_SpatialInventoryExample::DebugPrintAllContainers()
{
	if (!InventoryComponent)
	{
		return;
	}

	TArray<FName> ContainerNames = InventoryComponent->GetContainerNames();
	
	for (const FName& ContainerName : ContainerNames)
	{
		UInv_InventoryContainer* Container = InventoryComponent->GetContainer(ContainerName);
		if (Container)
		{
			UInv_SpatialUtilityLibrary::DebugPrintGrid(Container);
		}
	}
}

void AInv_SpatialInventoryExample::ClearAllContainers()
{
	if (!InventoryComponent)
	{
		return;
	}

	TArray<FName> ContainerNames = InventoryComponent->GetContainerNames();
	
	for (const FName& ContainerName : ContainerNames)
	{
		UInv_InventoryContainer* Container = InventoryComponent->GetContainer(ContainerName);
		if (Container)
		{
			Container->ClearGrid();
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("All containers cleared"));
}

void AInv_SpatialInventoryExample::TestSpatialOptimization()
{
	if (!MainContainer)
	{
		return;
	}

	// Test spatial optimization features
	int32 FillPercentage = UInv_SpatialUtilityLibrary::GetContainerFillPercentage(MainContainer);
	TArray<FIntPoint> EmptyRegions = UInv_SpatialUtilityLibrary::GetEmptyRegions(MainContainer, 4);
	
	UE_LOG(LogTemp, Warning, TEXT("Container fill percentage: %d%%"), FillPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Empty regions of size 4+: %d"), EmptyRegions.Num());
}

void AInv_SpatialInventoryExample::FillContainerWithVariedShapes()
{
	if (!MainContainer)
	{
		return;
	}

	// Fill container with various realistic shapes
	CreateRealisticShapes();
	
	UE_LOG(LogTemp, Warning, TEXT("Container filled with varied realistic shapes"));
}

UInv_ItemInstance* AInv_SpatialInventoryExample::CreateExampleItem(const FString& ItemName, const TArray<FIntPoint>& Shape, int32 Quantity)
{
	// Create item data asset
	UInv_ItemDataAsset* ItemDataAsset = NewObject<UInv_ItemDataAsset>();
	if (!ItemDataAsset)
	{
		return nullptr;
	}

	// Create item definition
	UInv_ItemDefinition* ItemDefinition = NewObject<UInv_ItemDefinition>();
	if (!ItemDefinition)
	{
		return nullptr;
	}

	// Set up item definition
	ItemDefinition->Name = FText::FromString(ItemName);
	ItemDefinition->Description = FText::FromString(FString::Printf(TEXT("A %s shaped item"), *ItemName));
	ItemDefinition->DefaultShape = Shape;
	ItemDefinition->StackSizeMax = 1;
	ItemDefinition->bCanRotate = true;
	ItemDefinition->AutoCalculateShapeDimensions();

	// Set up item data asset
	ItemDataAsset->ItemDefinition = ItemDefinition;

	// Create item instance
	UInv_ItemInstance* ItemInstance = NewObject<UInv_ItemInstance>();
	if (!ItemInstance)
	{
		return nullptr;
	}

	ItemInstance->Initialize(ItemDataAsset, Quantity);
	
	return ItemInstance;
}

void AInv_SpatialInventoryExample::CreateSword()
{
	// Sword shape: long vertical with crossguard
	TArray<FIntPoint> SwordShape = {
		FIntPoint(1, 0), // Crossguard
		FIntPoint(0, 1), // Blade
		FIntPoint(1, 1), // Blade
		FIntPoint(2, 1), // Crossguard
		FIntPoint(1, 2), // Blade
		FIntPoint(1, 3), // Blade
		FIntPoint(1, 4)  // Blade
	};

	UInv_ItemInstance* SwordItem = CreateExampleItem(TEXT("Sword"), SwordShape);
	if (SwordItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, SwordItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(SwordItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Sword placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}

void AInv_SpatialInventoryExample::CreateShield()
{
	// Shield shape: large circular-ish shape
	TArray<FIntPoint> ShieldShape = {
		FIntPoint(1, 0),
		FIntPoint(0, 1),
		FIntPoint(1, 1),
		FIntPoint(2, 1),
		FIntPoint(0, 2),
		FIntPoint(1, 2),
		FIntPoint(2, 2)
	};

	UInv_ItemInstance* ShieldItem = CreateExampleItem(TEXT("Shield"), ShieldShape);
	if (ShieldItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, ShieldItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(ShieldItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Shield placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}

void AInv_SpatialInventoryExample::CreatePotion()
{
	// Potion shape: small bottle
	TArray<FIntPoint> PotionShape = {
		FIntPoint(0, 0),
		FIntPoint(1, 0),
		FIntPoint(0, 1),
		FIntPoint(1, 1)
	};

	UInv_ItemInstance* PotionItem = CreateExampleItem(TEXT("Potion"), PotionShape);
	if (PotionItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, PotionItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(PotionItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Potion placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}

void AInv_SpatialInventoryExample::CreateArmor()
{
	// Armor shape: L-shaped chest piece
	TArray<FIntPoint> ArmorShape = {
		FIntPoint(0, 0),
		FIntPoint(1, 0),
		FIntPoint(2, 0),
		FIntPoint(0, 1),
		FIntPoint(1, 1),
		FIntPoint(2, 1),
		FIntPoint(1, 2)
	};

	UInv_ItemInstance* ArmorItem = CreateExampleItem(TEXT("Armor"), ArmorShape);
	if (ArmorItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, ArmorItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(ArmorItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Armor placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}

void AInv_SpatialInventoryExample::CreateTool()
{
	// Tool shape: hammer-like
	TArray<FIntPoint> ToolShape = {
		FIntPoint(0, 0),
		FIntPoint(1, 0),
		FIntPoint(0, 1),
		FIntPoint(1, 1),
		FIntPoint(1, 2)
	};

	UInv_ItemInstance* ToolItem = CreateExampleItem(TEXT("Tool"), ToolShape);
	if (ToolItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, ToolItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(ToolItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Tool placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}

void AInv_SpatialInventoryExample::CreateBook()
{
	// Book shape: rectangular
	TArray<FIntPoint> BookShape = {
		FIntPoint(0, 0),
		FIntPoint(1, 0),
		FIntPoint(0, 1),
		FIntPoint(1, 1)
	};

	UInv_ItemInstance* BookItem = CreateExampleItem(TEXT("Book"), BookShape);
	if (BookItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, BookItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(BookItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Book placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}

void AInv_SpatialInventoryExample::CreateKey()
{
	// Key shape: small L-shape
	TArray<FIntPoint> KeyShape = {
		FIntPoint(0, 0),
		FIntPoint(1, 0),
		FIntPoint(1, 1)
	};

	UInv_ItemInstance* KeyItem = CreateExampleItem(TEXT("Key"), KeyShape);
	if (KeyItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, KeyItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(KeyItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Key placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}

void AInv_SpatialInventoryExample::CreateGem()
{
	// Gem shape: small diamond
	TArray<FIntPoint> GemShape = {
		FIntPoint(1, 0),
		FIntPoint(0, 1),
		FIntPoint(1, 1),
		FIntPoint(2, 1),
		FIntPoint(1, 2)
	};

	UInv_ItemInstance* GemItem = CreateExampleItem(TEXT("Gem"), GemShape);
	if (GemItem && MainContainer)
	{
		UInv_SpatialUtilityLibrary::FPlacementOptions Options;
		UInv_SpatialUtilityLibrary::FPlacementResult Result = UInv_SpatialUtilityLibrary::FindBestPlacement(MainContainer, GemItem, Options);
		
		if (Result.bCanPlace)
		{
			MainContainer->TryPlaceItem(GemItem, Result.BestPosition, Result.BestRotation);
			UE_LOG(LogTemp, Warning, TEXT("Gem placed at (%d, %d) with rotation %.1f"), 
				Result.BestPosition.X, Result.BestPosition.Y, Result.BestRotation);
		}
	}
}
