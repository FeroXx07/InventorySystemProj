// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagment/Components/AC_PlugInv_EquipmentComponent.h"

#include "EquipmentManagment/AC_PlugInv_EquipActor.h"
#include "GameFramework/Character.h"
#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"
#include "Items/O_PlugInv_InventoryItem.h"
#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"


// Sets default values for this component's properties
UPlugInv_EquipmentComponent::UPlugInv_EquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlugInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitPlayerController();
}

void UPlugInv_EquipmentComponent::InitPlayerController()
{
	if (OwningPlayerController = Cast<APlayerController>(GetOwner()); OwningPlayerController.IsValid())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
		{
			OwningSkeletalMesh = OwnerCharacter->GetMesh();
			OnPossessedPawnChange(nullptr, OwnerCharacter);
		}
		else
		{
			OwningPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChange);
		}
	}
}

// Called every frame
void UPlugInv_EquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlugInv_EquipmentComponent::OnItemEquipped(UPlugInv_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	
	if (!OwningPlayerController->HasAuthority()) return;
	
	FPlugInv_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FPlugInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlugInv_EquipmentFragment>();
	
	if (!EquipmentFragment) return;
	EquipmentFragment->OnEquip(OwningPlayerController.Get());

	if (!OwningSkeletalMesh.IsValid()) return;
	
	APlugInv_EquipActor* SpawnedEquipActor = SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get());
	EquippedActors.Add(SpawnedEquipActor);
}

void UPlugInv_EquipmentComponent::OnItemUnequipped(UPlugInv_InventoryItem* UnEquippedItem)
{
	if (!IsValid(UnEquippedItem)) return;
	
	if (!OwningPlayerController->HasAuthority()) return;
	
	FPlugInv_ItemManifest& ItemManifest = UnEquippedItem->GetItemManifestMutable();
	FPlugInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlugInv_EquipmentFragment>();
	
	if (!EquipmentFragment) return;
	EquipmentFragment->OnUnequip(OwningPlayerController.Get());

	RemoveEquippedActor(EquipmentFragment->GetEquipmentType());
}

void UPlugInv_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = UPlugInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get());
	if (!InventoryComponent.IsValid()) return;

	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &ThisClass::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this, &ThisClass::OnItemEquipped);
	}

	if (!InventoryComponent->OnItemUnequipped.IsAlreadyBound(this, &ThisClass::OnItemUnequipped))
	{
		InventoryComponent->OnItemUnequipped.AddDynamic(this, &ThisClass::OnItemUnequipped);
	}
}

APlugInv_EquipActor* UPlugInv_EquipmentComponent::SpawnEquippedActor(FPlugInv_EquipmentFragment* EquipmentFragment,
	const FPlugInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh)
{
	APlugInv_EquipActor* SpawnedEquipActor = EquipmentFragment->SpawnAttachedActor(AttachMesh);
	SpawnedEquipActor->SetEquipmentType(EquipmentFragment->GetEquipmentType());
	SpawnedEquipActor->SetOwner(GetOwner());
	EquipmentFragment->SetEquippedActor(SpawnedEquipActor);
	return SpawnedEquipActor;
}

APlugInv_EquipActor* UPlugInv_EquipmentComponent::FindEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	TObjectPtr<APlugInv_EquipActor>* FoundActor = EquippedActors.FindByPredicate(
		[&EquipmentTypeTag](const APlugInv_EquipActor* EquippedActor)
		{
			return EquippedActor->GetEquipmentType().MatchesTagExact(EquipmentTypeTag);
		});
	
	return FoundActor ? *FoundActor : nullptr;
}

void UPlugInv_EquipmentComponent::RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	if (APlugInv_EquipActor* EquippedActor = FindEquippedActor(EquipmentTypeTag); IsValid(EquippedActor))
	{
		EquippedActors.Remove(EquippedActor);
		EquippedActor->Destroy();
	}
}

void UPlugInv_EquipmentComponent::OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn)
{
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(NewPawn); IsValid(OwnerCharacter))
	{
		OwningSkeletalMesh = OwnerCharacter->GetMesh();
	}
	
	InitInventoryComponent();
}

