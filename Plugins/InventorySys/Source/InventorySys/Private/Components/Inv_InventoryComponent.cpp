// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Inv_InventoryComponent.h"
#include "UObjects/Inv_InventoryContainer.h"
#include "UObjects/Inv_ItemInstance.h"
#include "UObjects/Inv_ItemDefinition.h"
#include "Engine/Engine.h"


// Sets default values for this component's properties
UInv_InventoryComponent::UInv_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInv_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Spatial inventory initialization
void UInv_InventoryComponent::InitializeSpatialInventory()
{
	if (bIsInitialized)
	{
		return;
	}

	// Create default container
	CreateContainer(DefaultContainerName, FText::FromName(DefaultContainerName), 
					DefaultContainerWidth, DefaultContainerHeight, InventoryTags);

	bIsInitialized = true;
}

// Container management
UInv_InventoryContainer* UInv_InventoryComponent::CreateContainer(const FName& ContainerName, const FText& DisplayName, int32 Width, int32 Height, const FGameplayTagContainer& Tags)
{
	// Check if container already exists
	if (GetContainer(ContainerName) != nullptr)
	{
		return nullptr;
	}

	// Create new container
	UInv_InventoryContainer* NewContainer = NewObject<UInv_InventoryContainer>(this);
	if (NewContainer)
	{
		NewContainer->Initialize(ContainerName, DisplayName, Width, Height, Tags);
		InventoryContainers.Add(NewContainer);
	}

	return NewContainer;
}

bool UInv_InventoryComponent::RemoveContainer(const FName& ContainerName)
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

UInv_InventoryContainer* UInv_InventoryComponent::GetContainer(const FName& ContainerName) const
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

// Spatial item management
bool UInv_InventoryComponent::TryAddItemToContainer(UInv_ItemInstance* ItemToAdd, const FName& ContainerName, const FIntPoint& GridPosition, float Rotation)
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

	// Try to find a suitable position in any container
	for (UInv_InventoryContainer* Container : InventoryContainers)
	{
		if (!Container || Container->IsLocked())
		{
			continue;
		}

		// Try to find an available position
		TArray<FIntPoint> AvailablePositions = Container->GetAvailablePositions();
		for (const FIntPoint& Position : AvailablePositions)
		{
			if (Container->CanPlaceItemAt(ItemToAdd, Position, Rotation))
			{
				if (Container->TryPlaceItem(ItemToAdd, Position, Rotation))
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

bool UInv_InventoryComponent::TryRemoveItemFromContainer(UInv_ItemInstance* ItemToRemove, const FName& ContainerName)
{
	UInv_InventoryContainer* Container = GetContainer(ContainerName);
	if (!Container || !ItemToRemove)
	{
		return false;
	}

	return Container->TryRemoveItem(ItemToRemove);
}

bool UInv_InventoryComponent::TryMoveItemBetweenContainers(UInv_ItemInstance* Item, const FName& FromContainer, const FName& ToContainer, const FIntPoint& NewPosition, float NewRotation)
{
	UInv_InventoryContainer* SourceContainer = GetContainer(FromContainer);
	UInv_InventoryContainer* TargetContainer = GetContainer(ToContainer);

	if (!SourceContainer || !TargetContainer || !Item)
	{
		return false;
	}

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
		// If placement failed, try to restore to source container
		// This is simplified - in a full implementation, you'd want to store the original position
		return false;
	}
}

bool UInv_InventoryComponent::CanPlaceItemInContainer(const UInv_ItemInstance* ItemToAdd, const FName& ContainerName, const FIntPoint& GridPosition, float Rotation) const
{
	UInv_InventoryContainer* Container = GetContainer(ContainerName);
	if (!Container || !ItemToAdd)
	{
		return false;
	}

	return Container->CanPlaceItemAt(const_cast<UInv_ItemInstance*>(ItemToAdd), GridPosition, Rotation);
}

bool UInv_InventoryComponent::CanPlaceItemInAnyContainer(const UInv_ItemInstance* ItemToAdd, FName& OutContainerName, FIntPoint& OutGridPosition, float Rotation) const
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

		TArray<FIntPoint> AvailablePositions = Container->GetAvailablePositions();
		for (const FIntPoint& Position : AvailablePositions)
		{
			if (Container->CanPlaceItemAt(const_cast<UInv_ItemInstance*>(ItemToAdd), Position, Rotation))
			{
				OutContainerName = Container->ContainerName;
				OutGridPosition = Position;
				return true;
			}
		}
	}

	return false;
}

// Utility functions
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

bool UInv_InventoryComponent::DoesInventoryContainItem(const UInv_ItemInstance* ItemToCheck) const
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

