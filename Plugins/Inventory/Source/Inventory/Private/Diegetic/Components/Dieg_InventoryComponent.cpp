// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Components/Dieg_InventoryComponent.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Algo/ForEach.h"
#include "Diegetic/Dieg_UtilityLibrary.h"
#include "Diegetic/UObjects/Dieg_ItemInstance.h"
#include "Diegetic/UStructs/Dieg_InventorySlot.h"


// Sets default values for this component's properties
UDieg_InventoryComponent::UDieg_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDieg_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeSlots(this->TotalSlots, this->MaxColumns, this->SlotTags);

	if (ItemRotationPriority != 0 && ItemRotationPriority != 90 && ItemRotationPriority != 180 && ItemRotationPriority != -90)
	{
		ItemRotationPriority = 0;
	}
}


TArray<FDieg_InventorySlot*> UDieg_InventoryComponent::GetRootSlotsMutable()
{
	TArray<FDieg_InventorySlot*> RootSlots;
	for (FDieg_InventorySlot& Slot : InventorySlots)
	{
		if (Slot.IsRootSlot())
			RootSlots.Add(&Slot);
	}
	// TArray<FDieg_InventorySlot*> RootSlots;
	//
	// // Lambda as predicate
	// Algo::ForEach(InventorySlots, [&RootSlots](FDieg_InventorySlot& Slot)
	// {
	// 	if (Slot.IsRootSlot())
	// 	{
	// 		RootSlots.Add(&Slot);
	// 	}
	// });

	return RootSlots;
}

// Called every frame
void UDieg_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Initializes inventory slots and pre-populates items if needed
void UDieg_InventoryComponent::InitializeSlots(const int32 NumSlots, const int32 NumColumns, const FGameplayTagContainer& Tags)
{
	LOG_DOUBLE_S(10.0, FColor::Turquoise, "InitializeSlots in {0}. NumSlots: {1}, NumColumns: {2}", this->GetName(), NumSlots, NumColumns);
	
	// Generate grid coordinates
	TArray<FIntPoint> Slots = UDieg_UtilityLibrary::GetSlotPoints(NumSlots, NumColumns);
	const int32 Size = Slots.Num();
	InventorySlots.Empty();
	InventorySlots.Reserve(Size);
	SlotsOccupation.Empty();
	SlotsOccupation.Reserve(Size);

	// Fill inventory with empty slots
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		const FIntPoint& SlotCoordinates = Slots[i];
		FDieg_InventorySlot Slot;
		Slot.Initialize(SlotCoordinates, Tags);
		InventorySlots.EmplaceAt(i, Slot);
		SlotsOccupation.Emplace(SlotCoordinates, false);
	}

	if (PrePopulateData.IsEmpty())
	{
		return;
	}
	
	LOG_DOUBLE_S(10.0, FColor::Turquoise, "InitializeSlots in {0}. Prepopulating", this->GetName());

	// Prepopulate inventory from saved data
	for (const FDieg_PrePopulate& Data : PrePopulateData)
	{
		UDieg_ItemInstance* NewItem = MakeInstanceFromPrePopulateData(Data);
		int32 Remaining = INT32_MAX;
		TryAddItem(NewItem, Remaining);
	}
}

// Attempts to add an item to the inventory
bool UDieg_InventoryComponent::TryAddItem(UDieg_ItemInstance* ItemToAdd, int32& Remaining)
{
	if (!IsValid(ItemToAdd))
	{
		return false;
	}

	const int32 CurrentQuantity = ItemToAdd->GetQuantity();
	const int32 MaxQuantity =  ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.StackSizeMax;
	int32 ToAdd = FMath::Clamp(CurrentQuantity, 1, MaxQuantity);

	LOG_DOUBLE_S(10.0, FColor::Turquoise, "TryAddItem in {0}. CurrentQuantity: {1}, MaxQuantity: {2}, ToAdd: {3}",
		this->GetName(), CurrentQuantity, MaxQuantity, ToAdd);

	// First try stacking on existing items of same type
	TSet<FDieg_InventorySlot*> FoundRootInstances = FindRootSlotByItemType(ItemToAdd);
	if (!FoundRootInstances.IsEmpty())
	{
		for (FDieg_InventorySlot* RootSlots : FoundRootInstances)
		{
			if (ToAdd <= 0) break;
			ToAdd -= AddQuantityToSlot(RootSlots->ItemInstance, ToAdd);
		}
	}

	// Stacking fulfilled quantity
	if (ToAdd <= 0)
	{
		Remaining = 0;
		return true;
	}

	// Attempt to place in new slots
	const TArray<FIntPoint> Shape = ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShape;
	const FIntPoint ShapeRoot = ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShapeRoot;
	for (const FDieg_InventorySlot& Slot : InventorySlots)
	{
		int32 RotationUsed = 0;
		if (CanAddItemToSlot(Slot.Coordinates, Shape, ShapeRoot, RotationUsed))
		{
			AddItemToInventory(ItemToAdd, Slot.Coordinates, RotationUsed);
			Remaining = 0;
			return true;
		}
	}

	// Cannot place remaining quantity
	Remaining = ToAdd;
	ItemToAdd->SetQuantity(Remaining);
	return false;
}

bool UDieg_InventoryComponent::TryRemoveItem(UDieg_ItemInstance* ItemToRemove)
{
	RemoveItemFromInventory(ItemToRemove);
	return true;
}

// Checks if an item can be added (stacking or new slots)
bool UDieg_InventoryComponent::CanAddItem(const UDieg_ItemInstance* ItemToAdd)
{
	if (!IsValid(ItemToAdd))
	{
		return false;
	}

	const int32 Quantity = FMath::Clamp(ItemToAdd->GetQuantity(), 1, ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.StackSizeMax);

	// Check stackable slots first
	TSet<FDieg_InventorySlot*> FoundRootInstances = FindRootSlotByItemType(ItemToAdd);
	for (FDieg_InventorySlot* RootSlot : FoundRootInstances)
	{
		if (!RootSlot || !IsValid(RootSlot->ItemInstance)) continue;

		const int32 Current = RootSlot->ItemInstance->GetQuantity();
		const int32 Max = RootSlot->ItemInstance->GetItemDefinitionDataAsset()->ItemDefinition.StackSizeMax;

		if (Current < Max)
		{
			return true; // Can stack
		}
	}

	// Check for available slots for new placement
	const TArray<FIntPoint> Shape = ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShape;
	const FIntPoint ShapeRoot = ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShapeRoot;

	for (const FDieg_InventorySlot& Slot : InventorySlots)
	{
		int32 RotationUsed = 0;
		if (CanAddItemToSlot(Slot.Coordinates, Shape, ShapeRoot, RotationUsed))
		{
			return true; // Found available placement
		}
	}

	return false; // No stacking or placement possible
}

// Attempts to add quantity to a root slot (stackable)
int32 UDieg_InventoryComponent::AddQuantityToSlot(const UDieg_ItemInstance* ItemToAdd, const int32 QuantityIn)
{
	if (!ItemToAdd) return 0;

	for (const FDieg_InventorySlot& Slot : InventorySlots)
	{
		if (Slot.IsRootSlot() && Slot.IsOccupied() && Slot.ItemInstance->CanStackWith(ItemToAdd))
		{
			const int32 MaxStack = Slot.ItemInstance->GetItemDefinitionDataAsset()->ItemDefinition.StackSizeMax;
			const int32 Added = FMath::Min(MaxStack - Slot.ItemInstance->GetQuantity(), QuantityIn);
			if (Added > 0)
			{
				Slot.ItemInstance->SetQuantity(Slot.ItemInstance->GetQuantity() + Added);
				return Added;
			}
		}
	}

	return 0;
}

// Returns true if slot coordinates are out of inventory bounds
bool UDieg_InventoryComponent::IsSlotPointOutOfBounds(const FIntPoint& SlotPoint)
{
	if (SlotsOccupation.Find(SlotPoint))
	{
		return false;
	}
	return true;
}

// Returns true if slot is occupied
bool UDieg_InventoryComponent::IsSlotPointOccupied(const FIntPoint& SlotPoint)
{
	return *SlotsOccupation.Find(SlotPoint); 
}


// Finds root slots matching the same item type
TSet<FDieg_InventorySlot*> UDieg_InventoryComponent::FindRootSlotByItemType(const UDieg_ItemInstance* ItemToCheck)
{
	TSet<FDieg_InventorySlot*> InventorySlotsFound;
	for (FDieg_InventorySlot& InventorySlot : InventorySlots)
	{
		if (InventorySlot.IsOccupied() && InventorySlot.IsRootSlot() && InventorySlot.ItemInstance->IsEqual(ItemToCheck))
		{
			InventorySlotsFound.Add(&InventorySlot);
		}
	}
	return InventorySlotsFound;	
}

// Finds root slots containing a specific instance
TSet<FDieg_InventorySlot*> UDieg_InventoryComponent::FindRootSlotByInstance(const UDieg_ItemInstance* ItemToCheck)
{
	TSet<FDieg_InventorySlot*> InventorySlotsFound;
	for (FDieg_InventorySlot& InventorySlot : InventorySlots)
	{
		if (InventorySlot.IsOccupied() && InventorySlot.IsRootSlot() && InventorySlot.ItemInstance == ItemToCheck)
		{
			InventorySlotsFound.Add(&InventorySlot);
		}
	}
	return InventorySlotsFound;	
}

// Checks if an item shape can fit starting from given slot
bool UDieg_InventoryComponent::CanAddItemToSlot(const FIntPoint& SlotCoordinates,
                                                const TArray<FIntPoint>& ItemShape,
                                                const FIntPoint& ItemShapeRoot,
                                                int32& RotationUsedOut)
{
	// Test default rotation first
	FIntPoint DefaultCoordsRoot;
	const TArray<FIntPoint> DefaultCoords = GetRelevantCoordinates(SlotCoordinates, ItemShape, ItemShapeRoot, ItemRotationPriority, DefaultCoordsRoot);
	if (AreSlotsAvailable(DefaultCoords, nullptr))
	{
		RotationUsedOut = ItemRotationPriority;
		return true;
	}

	// Test other rotations
	const TArray<int32> PossibleRotations = { 0, 90, 180, -90 };
	for (int32 TestAngle : PossibleRotations)
	{
		if (TestAngle == ItemRotationPriority) continue;

		FIntPoint TestCoordsRoot;
		TArray<FIntPoint> TestCoords = GetRelevantCoordinates(SlotCoordinates, ItemShape, ItemShapeRoot, TestAngle, TestCoordsRoot);
		if (AreSlotsAvailable(TestCoords, nullptr))
		{
			RotationUsedOut = TestAngle;
			return true;
		}
	}

	return false;
}


// Checks if all given slots are available (optionally ignoring some)
bool UDieg_InventoryComponent::AreSlotsAvailable(const TArray<FIntPoint>& InputShape, const TArray<FIntPoint>* Ignore)
{
	for (const FIntPoint& Point : InputShape)
	{
		if (Ignore && Ignore->Contains(Point)) continue;

		if (IsSlotPointOutOfBounds(Point)) return false;

		if (IsSlotPointOccupied(Point)) return false; // Slot already occupied
	}

	return true;
}

// Places an item into inventory and sets root/rotation
const FDieg_InventorySlot* UDieg_InventoryComponent::AddItemToInventory(UDieg_ItemInstance* ItemToAdd, const FIntPoint& SlotCoordinates, const float RotationUsed)
{
	// Get rotated coordinates and root
	const TArray<FIntPoint> Shape = ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShape;
	const FIntPoint ShapeRoot = ItemToAdd->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShapeRoot;
	FIntPoint RotatedShapeRoot;
	TArray<FIntPoint> RotatedShapeCoordinates = GetRelevantCoordinates(SlotCoordinates, Shape, ShapeRoot, RotationUsed, RotatedShapeRoot);

	// Safety check: cannot place
	int32 AssertRotation = 0;
	if (!CanAddItemToSlot(SlotCoordinates, Shape, ShapeRoot, AssertRotation))
	{
		checkNoEntry();
		return nullptr;
	}

	// Fill all slots with item instance
	for (const FIntPoint& Coord : RotatedShapeCoordinates)
	{
		if (FDieg_InventorySlot* Slot = InventorySlots.FindByPredicate([&](const FDieg_InventorySlot& S) { return S.Coordinates == Coord; }))
		{
			Slot->ItemInstance = ItemToAdd;
			Slot->Rotation = RotationUsed;
			Slot->RootSlot = RotatedShapeRoot;
			SlotsOccupation.FindChecked(Coord) = true;
		}
	}

	// Return the root slot
	return InventorySlots.FindByPredicate([&](const FDieg_InventorySlot& S) { return S.IsRootSlot(); });
}

const FDieg_InventorySlot* UDieg_InventoryComponent::RemoveItemFromInventory(UDieg_ItemInstance* ItemToRemove)
{
	if (!IsValid(ItemToRemove))
	{
		return nullptr;
	}

	FDieg_InventorySlot* RootSlot = nullptr;

	// Iterate all slots and clear those that contain this item
	for (FDieg_InventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemInstance == ItemToRemove)
		{
			// Capture root slot for return
			if (Slot.IsRootSlot())
			{
				RootSlot = &Slot;
			}

			// Clear slot
			Slot.ItemInstance = nullptr;
			Slot.Rotation = 0.0f;
			Slot.RootSlot = FIntPoint(-1, -1);
			SlotsOccupation.FindChecked(Slot.Coordinates) = false;
		}
	}

	return RootSlot; // Return root slot if found, else nullptr
}


// Returns all coordinates occupied by an item relative to a slot and rotation
TArray<FIntPoint> UDieg_InventoryComponent::GetRelevantCoordinates(const FIntPoint& SlotCoordinates, const TArray<FIntPoint>& Shape, const FIntPoint& ShapeRoot, float Rotation, FIntPoint& RootSlotOut)
{
	TArray<FIntPoint> RotatedShape = UDieg_UtilityLibrary::Rotate2DArrayWithRoot(Shape, Rotation, ShapeRoot, RootSlotOut);
	for (FIntPoint& RotPoint : RotatedShape)
	{
		RotPoint += SlotCoordinates; // offset to placement
	}
	RootSlotOut += SlotCoordinates; // adjust root
	return RotatedShape;
}

TArray<int32> UDieg_InventoryComponent::GetRelevantItems(const TArray<FIntPoint>& ShapeCoordinates,
	const UDieg_ItemInstance* ItemInstance)
{
	TArray<int32> Result;

	// Convert input shape coordinates to a set for fast intersection checks
	TSet<FIntPoint> InputCoordinates;
	InputCoordinates.Append(ShapeCoordinates);

	// Iterate all inventory slots
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		const int32 CurrentIndex = i;
		FDieg_InventorySlot& CurrentSlot = InventorySlots[CurrentIndex];
		
		// Skip non-root slots; only root slots represent unique items
		if (!CurrentSlot.IsRootSlot())
		{
			continue;
		}

		// Compute all coordinates occupied by this item instance in inventory
		FIntPoint CurrentSlotRoot;
		TSet<FIntPoint> CurrentInventoryItemCoordinates;
		TArray<FIntPoint> CurrentRelevantCoordinates = GetRelevantCoordinates(
			CurrentSlot.RootSlot,
			CurrentSlot.ItemInstance->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShape,
			CurrentSlot.ItemInstance->GetItemDefinitionDataAsset()->ItemDefinition.DefaultShapeRoot, 
			CurrentSlot.Rotation, 
			CurrentSlotRoot
		);
		CurrentInventoryItemCoordinates.Append(CurrentRelevantCoordinates);

		// Check if this item instance matches the one we're searching for,
		// has available quantity space, and overlaps input coordinates
		bool bIsSameAndHasQuantitySpace = false;
		if (CurrentSlot.ItemInstance->IsEqual(ItemInstance) 
			&& CurrentSlot.ItemInstance->GetQuantity() != ItemInstance->GetQuantity()
			&& InputCoordinates.Intersect(CurrentInventoryItemCoordinates).IsEmpty() == false)
		{
			bIsSameAndHasQuantitySpace = true;
		}

		// If conditions met, add the slot index to result
		if (bIsSameAndHasQuantitySpace)
		{
			Result.Add(CurrentIndex);
		}
	}
	
	return Result; // Return all slot indices with matching items and overlapping coordinates
}


// Converts a pre-populate struct into an item instance
UDieg_ItemInstance* UDieg_InventoryComponent::MakeInstanceFromPrePopulateData(const FDieg_PrePopulate& PrePopData)
{
	UDieg_ItemInstance* ItemInstance = NewObject<UDieg_ItemInstance>(this, TEXT("ItemInstance_FromPrepopulate"));
	ItemInstance->Initialize(PrePopData.ItemDefinitionDataAsset, PrePopData.Quantity);
	return ItemInstance;
}