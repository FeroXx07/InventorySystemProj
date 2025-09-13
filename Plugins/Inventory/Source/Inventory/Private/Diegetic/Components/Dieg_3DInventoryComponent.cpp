// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Components/Dieg_3DInventoryComponent.h"

#include "Components/WidgetComponent.h"
#include "Diegetic/Actors/Dieg_WorldItemActor.h"
#include "Diegetic/Components/Dieg_InventoryComponent.h"
#include "Diegetic/Widgets/Dieg_Grid.h"


// Sets default values for this component's properties
UDieg_3DInventoryComponent::UDieg_3DInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDieg_3DInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PreInitialize();
}


// Called every frame
void UDieg_3DInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDieg_3DInventoryComponent::PreInitialize()
{
	// Cache references
	const TObjectPtr<AActor> Owner = GetOwner();
	if (const TObjectPtr<UWidgetComponent> WidgetComp = Owner->FindComponentByClass<UWidgetComponent>())
	{
		WidgetComponentRef = WidgetComp;
		WidgetComponentRef->InitWidget();
		GridWidget = Cast<UDieg_Grid>(WidgetComponentRef.Get()->GetWidget());
	}

	// Bound handlers to delegates if not bound previously
	if (IsValid(GridWidget))
	{
		if (!GridWidget->OnGridSlotHover.IsAlreadyBound(this, &ThisClass::UDieg_3DInventoryComponent::WidgetSlotHover))
		{
			GridWidget->OnGridSlotHover.AddDynamic(this, &ThisClass::UDieg_3DInventoryComponent::WidgetSlotHover);
		}

		if (!GridWidget->OnGridSlotUnHover.IsAlreadyBound(this, &ThisClass::UDieg_3DInventoryComponent::WidgetSlotUnHovered))
		{
			GridWidget->OnGridSlotUnHover.AddDynamic(this, &ThisClass::UDieg_3DInventoryComponent::WidgetSlotUnHovered);
		}
	}
	
	// There are actors that can contain both 3D inventory and inventory components.
	// Chest: 3D Inventory Comp + Inventory Comp
	// Player: Inventory Comp; Uses another actor that contains 3D Inventory Comp to display (eg: Briefcase, Bag).
	// Briefcase: 3D Inventory Comp; Uses another actor's Inventory Comp to display what it contains.
	if (const TObjectPtr<UDieg_InventoryComponent> InvComponent = Owner->FindComponentByClass<UDieg_InventoryComponent>())
	{
		InventoryComponentRef = InvComponent;
	}
	else
	{
		// If Inventory component is not found on owner actor, request external actors to bind it, through this delegate.
		if (OnInventoryExternalLinkRequest.IsBound())
		{
			UDieg_InventoryComponent* RawRef = InventoryComponentRef.Get();
			OnInventoryExternalLinkRequest.Broadcast(RawRef);
			InventoryComponentRef = RawRef; // sync back into TObjectPtr
		}
	}

	if (IsValid(InventoryComponentRef))
	{
		if (!InventoryComponentRef->OnInventoryInitialized.IsAlreadyBound(this, &ThisClass::Initialize))
		{
			InventoryComponentRef->OnInventoryInitialized.AddDynamic(this, &ThisClass::Initialize);
		}
	}
}

void UDieg_3DInventoryComponent::Initialize()
{
	if (IsValid(InventoryComponentRef) && WidgetComponentRef.IsValid() && IsValid(GridWidget))
	{
		Populate3D();
	}
}

void UDieg_3DInventoryComponent::Populate3D()
{
	if (!IsValid(ItemClass))
	{
		return;
	}
	
	GridWidget->CreateEmptyGrid(InventoryComponentRef->GetTotalSlots(), InventoryComponentRef->GetMaxColumns());
	const FVector ZeroLocation = FVector::ZeroVector;
	const FRotator ZeroRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	const TArray<FDieg_InventorySlot*> InventoryRootSlots = InventoryComponentRef->GetRootSlotsMutable();
	for (const FDieg_InventorySlot* RootSlot : InventoryRootSlots)
	{
		ADieg_WorldItemActor* ItemActor = GetWorld()->SpawnActor<ADieg_WorldItemActor>(ItemClass, ZeroLocation, ZeroRotation, SpawnParams);
		ItemActor->SetFromInventorySlot(*RootSlot);
		Items.Add(ItemActor);
		ItemActor->AttachToComponent(WidgetComponentRef.Get(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void UDieg_3DInventoryComponent::DeInitializeInventory()
{
	for (const TWeakObjectPtr<ADieg_WorldItemActor>& ItemActor : Items)
	{
		if (IsValid(ItemActor.Get()))
		{
			ItemActor->Destroy();
		}
	}

	Items.Empty(); // clears all weak pointers
}

void UDieg_3DInventoryComponent::AddItemToInventory(ADieg_WorldItemActor* ItemActor)
{
	if (!IsValid(ItemActor) || !IsValid(InventoryComponentRef))
	{
		return;
	}

	int32 Remaining = INT32_MAX;
	InventoryComponentRef->TryAddItem(ItemActor->GetItemInstanceMutable(), Remaining);
	Items.Add(ItemActor);
}

void UDieg_3DInventoryComponent::AddItemToInventorySlot(ADieg_WorldItemActor* ItemActor, const FIntPoint& SlotCoordinates, int32 RotationUsed)
{
	if (!IsValid(ItemActor) || !IsValid(InventoryComponentRef))
	{
		return;
	}

	if (InventoryComponentRef->CanAddItemToSlot(SlotCoordinates, ItemActor->GetItemInstance(), RotationUsed))
	{
		int32 Remaining = INT32_MAX;
		InventoryComponentRef->AddItemToInventory(ItemActor->GetItemInstanceMutable(), SlotCoordinates, RotationUsed);
		Items.Add(ItemActor);
	}
}

void UDieg_3DInventoryComponent::RemoveItemFromInventory(ADieg_WorldItemActor* ItemActor)
{
	if (!IsValid(ItemActor) || !IsValid(InventoryComponentRef))
	{
		return;
	}

	InventoryComponentRef->TryRemoveItem(ItemActor->GetItemInstance());
	Items.Remove(ItemActor);
}

void UDieg_3DInventoryComponent::WidgetSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDieg_Slot* GridSlot)
{
	On3DWidgetSlotHovered.Broadcast(InGeometry, InMouseEvent, this, GridSlot);
}

void UDieg_3DInventoryComponent::WidgetSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot)
{
	On3DWidgetSlotUnHovered.Broadcast(InMouseEvent, this, GridSlot);
}

