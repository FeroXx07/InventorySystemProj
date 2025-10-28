// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Inv_InventoryComponent.h"
#include "Controllers/Inv_InventoryController.h"
#include "UObjects/Inv_InventoryContainer.h"
#include "UObjects/Inv_ItemInstance.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // Controller handles logic, no need to tick
	bIsInitialized = false;
}

// ============================================
// LIFECYCLE
// ============================================

void UInv_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (bIsInitialized)
	{
		return;
	}

	// Create Controller with THIS as outer (for proper garbage collection)
	Controller = NewObject<UInv_InventoryController>(this, TEXT("InventoryController"));
	Controller->Initialize(this);

	// Create default container if needed
	if (bCreateDefaultContainer)
	{
		CreateContainer(DefaultContainerName, FText::FromName(DefaultContainerName), 
						DefaultContainerWidth, DefaultContainerHeight, InventoryTags);
		
		// Set as current container in controller
		if (InventoryContainers.Num() > 0)
		{
			Controller->SetCurrentContainer(InventoryContainers[0]);
		}
	}

	bIsInitialized = true;
}

void UInv_InventoryComponent::UninitializeComponent()
{
	// Cleanup controller
	if (Controller)
	{
		Controller->Shutdown();
		Controller = nullptr;
	}

	// Cleanup containers
	InventoryContainers.Empty();
	
	bIsInitialized = false;

	Super::UninitializeComponent();
}

void UInv_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// ============================================
// FORWARDING API (Optional convenience methods)
// ============================================

bool UInv_InventoryComponent::TryPickupItem(UInv_ItemInstance* Item)
{
	if (!Controller)
	{
		return false;
	}
	return Controller->TryPickupItem(Item);
}

bool UInv_InventoryComponent::TryPlaceItem(UInv_ItemInstance* Item, FIntPoint Position, float Rotation)
{
	if (!Controller)
	{
		return false;
	}
	return Controller->TryPlaceItem(Item, Position, Rotation);
}

bool UInv_InventoryComponent::TryRemoveItem(UInv_ItemInstance* Item)
{
	if (!Controller)
	{
		return false;
	}
	return Controller->TryRemoveItem(Item);
}

// ============================================
// CONTAINER MANAGEMENT
// ============================================

UInv_InventoryContainer* UInv_InventoryComponent::CreateContainer(FName ContainerName, FText DisplayName, int32 Width, int32 Height, const FGameplayTagContainer& Tags)
{
	// Check if container already exists
	if (GetContainer(ContainerName) != nullptr)
	{
		return nullptr;
	}

	// Create new container with THIS as outer (for proper GC)
	UInv_InventoryContainer* NewContainer = NewObject<UInv_InventoryContainer>(this);
	if (NewContainer)
	{
		NewContainer->Initialize(ContainerName, DisplayName, Width, Height, Tags);
		InventoryContainers.Add(NewContainer);
	}

	return NewContainer;
}

bool UInv_InventoryComponent::RemoveContainer(FName ContainerName)
{
	for (int32 i = InventoryContainers.Num() - 1; i >= 0; --i)
	{
		if (InventoryContainers[i] && InventoryContainers[i]->ContainerName == ContainerName)
		{
			InventoryContainers.RemoveAt(i);
			return true;
		}
	}
	return false;
}

UInv_InventoryContainer* UInv_InventoryComponent::GetContainer(FName ContainerName) const
{
	for (UInv_InventoryContainer* Container : InventoryContainers)
	{
		if (Container && Container->ContainerName == ContainerName)
		{
			return Container;
		}
	}
	return nullptr;
}

UInv_InventoryContainer* UInv_InventoryComponent::GetContainerAtIndex(int32 Index) const
{
	if (InventoryContainers.IsValidIndex(Index))
	{
		return InventoryContainers[Index];
	}
	return nullptr;
}

int32 UInv_InventoryComponent::GetContainerCount() const
{
	return InventoryContainers.Num();
}

TArray<FName> UInv_InventoryComponent::GetContainerNames() const
{
	TArray<FName> Names;
	for (UInv_InventoryContainer* Container : InventoryContainers)
	{
		if (Container)
		{
			Names.Add(Container->ContainerName);
		}
	}
	return Names;
}

// ============================================
// ITEM OPERATIONS
// ============================================

bool UInv_InventoryComponent::TryAddItemToContainer(UInv_ItemInstance* ItemToAdd, FName ContainerName, FIntPoint GridPosition, float Rotation)
{
	UInv_InventoryContainer* Container = GetContainer(ContainerName);
	if (!Container || !ItemToAdd)
	{
		return false;
	}

	return Container->TryPlaceItem(ItemToAdd, GridPosition, Rotation);
}

bool UInv_InventoryComponent::TryAddItemToAnyContainer(UInv_ItemInstance* ItemToAdd, FName& OutContainerName, FIntPoint& OutGridPosition, float Rotation)
{
	if (!ItemToAdd)
	{
		return false;
	}

	// Try different rotations
	TArray<float> RotationsToTry = {Rotation, Rotation + 90.0f, Rotation + 180.0f, Rotation + 270.0f};

	// Try to find a suitable position in any container
	for (UInv_InventoryContainer* Container : InventoryContainers)
	{
		if (!Container || Container->IsLocked())
		{
			continue;
		}

		// Try each rotation
		for (float TestRotation : RotationsToTry)
		{
			// Try each position
			for (int32 Y = 0; Y < Container->GridHeight; ++Y)
			{
				for (int32 X = 0; X < Container->GridWidth; ++X)
				{
					FIntPoint Position(X, Y);
					if (Container->CanPlaceItemAt(ItemToAdd, Position, TestRotation))
					{
						if (Container->TryPlaceItem(ItemToAdd, Position, TestRotation))
						{
							OutContainerName = Container->ContainerName;
							OutGridPosition = Position;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool UInv_InventoryComponent::TryRemoveItemFromContainer(UInv_ItemInstance* ItemToRemove, FName ContainerName)
{
	UInv_InventoryContainer* Container = GetContainer(ContainerName);
	if (!Container || !ItemToRemove)
	{
		return false;
	}

	return Container->TryRemoveItem(ItemToRemove);
}

bool UInv_InventoryComponent::TryMoveItemBetweenContainers(UInv_ItemInstance* Item, FName FromContainer, FName ToContainer, FIntPoint NewPosition, float NewRotation)
{
	UInv_InventoryContainer* SourceContainer = GetContainer(FromContainer);
	UInv_InventoryContainer* TargetContainer = GetContainer(ToContainer);

	if (!SourceContainer || !TargetContainer || !Item)
	{
		return false;
	}

	// Store original placement for rollback
	FInv_ItemPlacement* OriginalPlacement = SourceContainer->FindItemPlacement(Item);
	if (!OriginalPlacement)
	{
		return false;
	}

	FIntPoint OriginalPosition = OriginalPlacement->GridPosition;
	float OriginalRotation = OriginalPlacement->Rotation;

	// Remove from source container
	if (!SourceContainer->TryRemoveItem(Item))
	{
		return false;
	}

	// Try to place in target container
	if (TargetContainer->TryPlaceItem(Item, NewPosition, NewRotation))
	{
		return true;
	}
	else
	{
		// Rollback - restore to source container
		SourceContainer->TryPlaceItem(Item, OriginalPosition, OriginalRotation);
		return false;
	}
}

// ============================================
// QUERIES
// ============================================

bool UInv_InventoryComponent::CanPlaceItemInContainer(UInv_ItemInstance* ItemToAdd, FName ContainerName, FIntPoint GridPosition, float Rotation) const
{
	UInv_InventoryContainer* Container = GetContainer(ContainerName);
	if (!Container || !ItemToAdd)
	{
		return false;
	}

	return Container->CanPlaceItemAt(ItemToAdd, GridPosition, Rotation);
}

bool UInv_InventoryComponent::CanPlaceItemInAnyContainer(UInv_ItemInstance* ItemToAdd, FName& OutContainerName, FIntPoint& OutGridPosition, float Rotation) const
{
	if (!ItemToAdd)
	{
		return false;
	}

	// Check all containers for available space
	for (UInv_InventoryContainer* Container : InventoryContainers)
	{
		if (!Container || Container->IsLocked())
		{
			continue;
		}

		for (int32 Y = 0; Y < Container->GridHeight; ++Y)
		{
			for (int32 X = 0; X < Container->GridWidth; ++X)
			{
				FIntPoint Position(X, Y);
				if (Container->CanPlaceItemAt(ItemToAdd, Position, Rotation))
				{
					OutContainerName = Container->ContainerName;
					OutGridPosition = Position;
					return true;
				}
			}
		}
	}

	return false;
}

bool UInv_InventoryComponent::DoesInventoryContainItem(UInv_ItemInstance* ItemToCheck) const
{
	if (!ItemToCheck)
	{
		return false;
	}

	// Check all containers
	for (UInv_InventoryContainer* Container : InventoryContainers)
	{
		if (Container && Container->ContainsItem(ItemToCheck))
		{
			return true;
		}
	}

	return false;
}
