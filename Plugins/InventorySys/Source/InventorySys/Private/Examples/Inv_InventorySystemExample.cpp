// Fill out your copyright notice in the Description page of Project Settings.

#include "Examples/Inv_InventorySystemExample.h"
#include "Components/Inv_InventoryComponent.h"
#include "Controllers/Inv_InventoryController.h"
#include "Widgets/Inv_SpatialGridWidget.h"
#include "UObjects/Inv_ItemInstance.h"
#include "UObjects/Inv_ItemDefinition.h"
#include "UObjects/Inv_ItemDataAsset.h"
#include "UObjects/Inv_InventoryContainer.h"
#include "Engine/Engine.h"

AInv_InventorySystemExample::AInv_InventorySystemExample()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create inventory component (it will create controller automatically)
	InventoryComponent = CreateDefaultSubobject<UInv_InventoryComponent>(TEXT("InventoryComponent"));
	
	// Configure default settings
	InventoryComponent->DefaultContainerName = MainContainerName;
	InventoryComponent->DefaultContainerWidth = MainContainerWidth;
	InventoryComponent->DefaultContainerHeight = MainContainerHeight;
	InventoryComponent->bCreateDefaultContainer = true;
}

void AInv_InventorySystemExample::BeginPlay()
{
	Super::BeginPlay();

	// Component initializes automatically (creates controller)
	if (InventoryComponent && InventoryComponent->IsRegistered())
	{
		// Create UI
		CreateUI();
		
		// Bind events
		BindEvents();
		
		// Create example items if requested
		if (bCreateExampleItems)
		{
			CreateExampleItems();
		}
	}
}

void AInv_InventorySystemExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ============================================
// SYSTEM MANAGEMENT
// ============================================

void AInv_InventorySystemExample::CreateUI()
{
	if (!InventoryComponent || !GridWidgetClass)
	{
		return;
	}

	// Create grid widget
	GridWidget = CreateWidget<UInv_SpatialGridWidget>(GetWorld(), GridWidgetClass);
	
	if (GridWidget)
	{
		// Widget connects to component (will bind to controller automatically)
		GridWidget->SetInventoryComponent(InventoryComponent);
		
		// Add to viewport
		GridWidget->AddToViewport();
	}
}

void AInv_InventorySystemExample::BindEvents()
{
	if (!InventoryComponent)
	{
		return;
	}

	UInv_InventoryController* Controller = InventoryComponent->GetController();
	if (!Controller)
	{
		return;
	}

	// Bind to controller events
	Controller->OnItemPlaced.AddDynamic(this, &AInv_InventorySystemExample::OnItemPlaced);
	Controller->OnItemRemoved.AddDynamic(this, &AInv_InventorySystemExample::OnItemRemoved);
	Controller->OnItemMoved.AddDynamic(this, &AInv_InventorySystemExample::OnItemMoved);
	Controller->OnDragStarted.AddDynamic(this, &AInv_InventorySystemExample::OnDragStarted);
	Controller->OnDragEnded.AddDynamic(this, &AInv_InventorySystemExample::OnDragEnded);
}

void AInv_InventorySystemExample::UnbindEvents()
{
	if (!InventoryComponent)
	{
		return;
	}

	UInv_InventoryController* Controller = InventoryComponent->GetController();
	if (!Controller)
	{
		return;
	}

	// Unbind events
	Controller->OnItemPlaced.RemoveDynamic(this, &AInv_InventorySystemExample::OnItemPlaced);
	Controller->OnItemRemoved.RemoveDynamic(this, &AInv_InventorySystemExample::OnItemRemoved);
	Controller->OnItemMoved.RemoveDynamic(this, &AInv_InventorySystemExample::OnItemMoved);
	Controller->OnDragStarted.RemoveDynamic(this, &AInv_InventorySystemExample::OnDragStarted);
	Controller->OnDragEnded.RemoveDynamic(this, &AInv_InventorySystemExample::OnDragEnded);
}

// ============================================
// EXAMPLE OPERATIONS
// ============================================

void AInv_InventorySystemExample::CreateExampleItems()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Example: Create a sword (3x1 shape)
	TArray<FIntPoint> SwordShape = {
		FIntPoint(0, 0),
		FIntPoint(0, 1),
		FIntPoint(0, 2)
	};
	UInv_ItemInstance* Sword = CreateTestItem(TEXT("Sword"), SwordShape);

	// Example: Create a shield (2x2 shape)
	TArray<FIntPoint> ShieldShape = {
		FIntPoint(0, 0),
		FIntPoint(1, 0),
		FIntPoint(0, 1),
		FIntPoint(1, 1)
	};
	UInv_ItemInstance* Shield = CreateTestItem(TEXT("Shield"), ShieldShape);

	// Example: Create a potion (1x1 shape)
	TArray<FIntPoint> PotionShape = {
		FIntPoint(0, 0)
	};
	UInv_ItemInstance* Potion = CreateTestItem(TEXT("Potion"), PotionShape);

	// Try to add items to inventory using controller
	if (Sword)
	{
		InventoryComponent->TryPickupItem(Sword);
	}
	
	if (Shield)
	{
		InventoryComponent->TryPickupItem(Shield);
	}
	
	if (Potion)
	{
		InventoryComponent->TryPickupItem(Potion);
	}
}

void AInv_InventorySystemExample::TestItemPlacement()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Create a test item
	TArray<FIntPoint> TestShape = {FIntPoint(0, 0)};
	UInv_ItemInstance* TestItem = CreateTestItem(TEXT("TestItem"), TestShape);

	// Try to place it at specific position
	bool bSuccess = InventoryComponent->TryPlaceItem(TestItem, FIntPoint(5, 5), 0.0f);
	
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Item placed successfully at (5, 5)"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to place item at (5, 5)"));
	}
}

void AInv_InventorySystemExample::TestItemRotation()
{
	if (!InventoryComponent)
	{
		return;
	}

	UInv_InventoryController* Controller = InventoryComponent->GetController();
	if (!Controller)
	{
		return;
	}

	// Simulate rotation input
	Controller->HandleRotationRequested(90.0f);
}

void AInv_InventorySystemExample::TestItemMovement()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Get first container
	UInv_InventoryContainer* Container = InventoryComponent->GetContainerAtIndex(0);
	if (!Container || Container->ContainedItems.Num() == 0)
	{
		return;
	}

	// Get first item
	UInv_ItemInstance* FirstItem = Container->ContainedItems[0];
	if (!FirstItem)
	{
		return;
	}

	// Try to move it to a new position
	Container->TryMoveItem(FirstItem, FIntPoint(7, 7), 0.0f);
}

// ============================================
// EVENT HANDLERS
// ============================================

void AInv_InventorySystemExample::OnItemPlaced(UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation)
{
	if (Item)
	{
		UE_LOG(LogTemp, Log, TEXT("Item placed at (%d, %d) with rotation %.1f"), 
			   Position.X, Position.Y, Rotation);
	}
}

void AInv_InventorySystemExample::OnItemRemoved(UInv_ItemInstance* Item)
{
	if (Item)
	{
		UE_LOG(LogTemp, Log, TEXT("Item removed"));
	}
}

void AInv_InventorySystemExample::OnItemMoved(UInv_ItemInstance* Item, const FIntPoint& NewPosition, float NewRotation)
{
	if (Item)
	{
		UE_LOG(LogTemp, Log, TEXT("Item moved to (%d, %d) with rotation %.1f"), 
			   NewPosition.X, NewPosition.Y, NewRotation);
	}
}

void AInv_InventorySystemExample::OnDragStarted(UInv_ItemInstance* Item, const FIntPoint& StartPosition)
{
	if (Item)
	{
		UE_LOG(LogTemp, Log, TEXT("Drag started at (%d, %d)"), StartPosition.X, StartPosition.Y);
	}
}

void AInv_InventorySystemExample::OnDragEnded(UInv_ItemInstance* Item, bool bSuccess)
{
	if (Item)
	{
		UE_LOG(LogTemp, Log, TEXT("Drag ended - Success: %s"), bSuccess ? TEXT("true") : TEXT("false"));
	}
}

// ============================================
// UTILITY FUNCTIONS
// ============================================

void AInv_InventorySystemExample::PrintSystemStatus()
{
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("No inventory component"));
		return;
	}

	UInv_InventoryController* Controller = InventoryComponent->GetController();
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("No controller"));
		return;
	}

	UInv_InventoryContainer* Container = Controller->GetCurrentContainer();
	if (!Container)
	{
		UE_LOG(LogTemp, Warning, TEXT("No current container"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("=== Inventory System Status ==="));
	UE_LOG(LogTemp, Log, TEXT("Container: %s"), *Container->ContainerName.ToString());
	UE_LOG(LogTemp, Log, TEXT("Grid Size: %d x %d"), Container->GridWidth, Container->GridHeight);
	UE_LOG(LogTemp, Log, TEXT("Item Count: %d"), Container->ContainedItems.Num());
	UE_LOG(LogTemp, Log, TEXT("Available Slots: %d / %d"), 
		   Container->GetAvailableSlotCount(), Container->GetTotalSlotCount());
	UE_LOG(LogTemp, Log, TEXT("Fill Percentage: %.1f%%"), Container->GetFillPercentage() * 100.0f);
	UE_LOG(LogTemp, Log, TEXT("Is Dragging: %s"), Controller->IsDragging() ? TEXT("true") : TEXT("false"));
}

void AInv_InventorySystemExample::ClearAllItems()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Get first container
	UInv_InventoryContainer* Container = InventoryComponent->GetContainerAtIndex(0);
	if (!Container)
	{
		return;
	}

	// Clear all items
	Container->ClearGrid();
	
	UE_LOG(LogTemp, Log, TEXT("All items cleared"));
}

// ============================================
// HELPER FUNCTIONS
// ============================================

UInv_ItemInstance* AInv_InventorySystemExample::CreateTestItem(const FString& ItemName, const TArray<FIntPoint>& Shape)
{
	// Create item definition
	UInv_ItemDefinition* ItemDef = NewObject<UInv_ItemDefinition>(this);
	ItemDef->Name = FText::FromString(ItemName);
	ItemDef->Description = FText::FromString(FString::Printf(TEXT("Test item: %s"), *ItemName));
	ItemDef->DefaultShape = Shape;
	ItemDef->bCanRotate = true;

	// Create data asset
	UInv_ItemDataAsset* ItemData = NewObject<UInv_ItemDataAsset>(this);
	ItemData->ItemDefinition = ItemDef;

	// Create item instance
	UInv_ItemInstance* ItemInstance = NewObject<UInv_ItemInstance>(this);
	ItemInstance->Initialize(ItemData, 1);

	return ItemInstance;
}
