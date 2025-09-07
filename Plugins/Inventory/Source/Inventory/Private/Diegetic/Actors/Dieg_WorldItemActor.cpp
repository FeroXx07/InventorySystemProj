// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Actors/Dieg_WorldItemActor.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Diegetic/Interfaces/Dieg_Interactor.h"
#include "Diegetic/UObjects/Dieg_ItemInstance.h"


// Sets default values
ADieg_WorldItemActor::ADieg_WorldItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create the root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(Root);

    ItemInstance = CreateDefaultSubobject<UDieg_ItemInstance>(TEXT("ItemInstance"));}

void ADieg_WorldItemActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		// Runtime: External initialization
		
	}
	else
	{
		// Editor
		if (!IsValid(PrePopulateData.ItemDefinitionDataAsset))
		{
			UPlugInv_DoubleLogger::LogError("Null Data Asset");
			return;
		}
		
		SetFromDataAsset(PrePopulateData.ItemDefinitionDataAsset);
		Quantity = PrePopulateData.Quantity;
		ItemInstance->Initialize(PrePopulateData.ItemDefinitionDataAsset, Quantity);
	}

}

// Called when the game starts or when spawned
void ADieg_WorldItemActor::BeginPlay()
{
	Super::BeginPlay();
}

const TObjectPtr<UDieg_ItemInstance>& ADieg_WorldItemActor::GetItemInstance() const
{
	return ItemInstance;
}

TObjectPtr<UDieg_ItemInstance>& ADieg_WorldItemActor::GetItemInstanceMutable()
{
	return ItemInstance;
}

void ADieg_WorldItemActor::SetItemInstance_Implementation(UDieg_ItemInstance* Instance)
{
	if (IsValid(Instance))
	{
		ItemInstance = Instance;
		SetFromDataAsset(ItemInstance->GetItemDefinitionDataAsset());
	}
}
void ADieg_WorldItemActor::SetFromDataAsset_Implementation(UDieg_ItemDefinitionDataAsset* InItemDataAsset)
{
	// Check soft reference validation
	const TSoftObjectPtr<UStaticMesh> StaticMeshSoftRef = InItemDataAsset->ItemDefinition.WorldMesh;
	TObjectPtr<UStaticMesh> StaticMeshObject;

	// Returns true if the pointer has an asset path but the asset is not loaded yet.
	if (StaticMeshSoftRef.IsNull())
	{
		return;
	}

	// Since it is loading synchronously: IsValid() and not async: IsPending().
	if (StaticMeshSoftRef.IsValid())
	{
		StaticMeshObject = StaticMeshSoftRef.Get();
	}
	else
	{
		UPlugInv_DoubleLogger::Log("ADieg_WorldItemActor::InitializeFromDataAsset Loading Mesh");
		StaticMeshObject = StaticMeshSoftRef.LoadSynchronous();
	}

	// Set the static mesh to desired from item data
	StaticMeshComponent->SetStaticMesh(StaticMeshObject);
}

// Called every frame
void ADieg_WorldItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADieg_WorldItemActor::OnInteract_Implementation(UObject* Interactor)
{
	IDieg_Interactable::OnInteract_Implementation(Interactor);
	if (Interactor->GetClass()->ImplementsInterface(UDieg_Interactor::StaticClass()))
	{
		IDieg_Interactor::Execute_NotifyInteractionStarted(Interactor,this);
		UPlugInv_DoubleLogger::Log("ADieg_WorldItemActor::OnInteract_Implementation");
		IDieg_Interactor::Execute_NotifyInteractionEnded(Interactor,this);
	}
}


