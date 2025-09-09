// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Actors/Dieg_WorldItemActor.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Components/TextRenderComponent.h"
#include "Diegetic/Dieg_UtilityLibrary.h"
#include "Diegetic/Interfaces/Dieg_Interactor.h"
#include "Diegetic/UObjects/Dieg_ItemInstance.h"
#include "Diegetic/Widgets/Dieg_Slot.h"


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

	TextRendererComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render Component"));
	TextRendererComponent->SetupAttachment(StaticMeshComponent);
	TextRendererComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	TextRendererComponent->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	
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
		
		UpdateFromDataAsset(PrePopulateData.ItemDefinitionDataAsset);
		Quantity = PrePopulateData.Quantity;
		ItemInstance->Initialize(PrePopulateData.ItemDefinitionDataAsset, Quantity);	}
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

void ADieg_WorldItemActor::SetFromItemInstance_Implementation(UDieg_ItemInstance* Instance)
{
	if (IsValid(Instance))
	{
		// Duplicate the UObject instead of storing the same reference
		ItemInstance = DuplicateObject<UDieg_ItemInstance>(Instance, this);

		UpdateFromDataAsset(ItemInstance->GetItemDefinitionDataAsset());
	}
}

void ADieg_WorldItemActor::SetFromInventorySlot_Implementation(const FDieg_InventorySlot& InventorySlot)
{
	SetFromItemInstance(InventorySlot.ItemInstance);
	const FDieg_ItemDefinition& ItemDefinition = ItemInstance->GetItemDefinitionDataAsset()->ItemDefinition;
	CurrentRotation = InventorySlot.Rotation;
	HandleDragEnterInventory({0,0});
	AdjustRotation();
	ModifyTextQuantity();
	AdjustLocation(InventorySlot.Coordinates);
}

bool ADieg_WorldItemActor::SetMesh(const UDieg_ItemDefinitionDataAsset* InItemDataAsset) const
{
	// Check soft reference validation
	const TSoftObjectPtr<UStaticMesh> StaticMeshSoftRef = InItemDataAsset->ItemDefinition.WorldMesh;
	TObjectPtr<UStaticMesh> StaticMeshObject;

	// Returns true if the pointer has an asset path but the asset is not loaded yet.
	if (StaticMeshSoftRef.IsNull())
	{
		return false;
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
	return true;
}

void ADieg_WorldItemActor::UpdateFromDataAsset_Implementation(UDieg_ItemDefinitionDataAsset* InItemDataAsset)
{
	SetMesh(InItemDataAsset);

	TextRendererComponent->SetText(InItemDataAsset->ItemDefinition.Name);
}

void ADieg_WorldItemActor::AdjustText() const
{
	const FRotator Rotation = FRotator(90.0, -CurrentRotation, 0.0);
	TextRendererComponent->SetRelativeRotation(Rotation);

	const FDieg_ItemDefinition& ItemDefinition = ItemInstance->GetItemDefinition();
	FIntPoint ShapeRootOut = ItemDefinition.DefaultShapeRoot;
	TArray<FIntPoint> RotatedShape = UDieg_UtilityLibrary::Rotate2DArrayWithRoot(ItemDefinition.DefaultShape,
		CurrentRotation, ItemDefinition.DefaultShapeRoot, ShapeRootOut);
	const FIntPoint ShapeSpan = UDieg_UtilityLibrary::GetShapeSpan(RotatedShape);
	int32 MaxX = ShapeSpan.X;
	int32 MaxY = ShapeSpan.Y;

	const FIntPoint TextCoordinates = ShapeRootOut; //GetTextCoordinates(RotatedShape, EDieg_TextLocation::None);
	const UDieg_Slot* DefaultSlot = GetDefault<UDieg_Slot>();
	
	const float GridSize3D = DefaultSlot->GetGridSize3D();
	const float GridSize3DHalfNegative = GridSize3D * -0.5f;
	
	FIntPoint TextCoordinatesWithOffset = TextCoordinates * GridSize3D;
	TextCoordinatesWithOffset = TextCoordinatesWithOffset - GridSize3DHalfNegative;
	
	FVector Location = FVector(TextCoordinatesWithOffset.X, TextCoordinatesWithOffset.Y, TextRendererComponent->GetRelativeLocation().Z);
	TextRendererComponent->SetRelativeLocation(Location);
}

void ADieg_WorldItemActor::ModifyTextQuantity()
{
	TextRendererComponent->SetText(FText::AsNumber(ItemInstance->GetQuantity()));
}

FIntPoint ADieg_WorldItemActor::GetTextCoordinates(const TArray<FIntPoint>& Shape, EDieg_TextLocation TextLocation) const
{
	return FIntPoint::ZeroValue;
}

void ADieg_WorldItemActor::AdjustLocation(const FIntPoint& Coordinates)
{
	const UDieg_Slot* DefaultSlot = GetDefault<UDieg_Slot>();
	const float InventoryScale3D = DefaultSlot->GetInventoryScale3D();
	const float UnitScaled = InventoryScale3D * GetUnitScaled();
	const FVector Location = FVector(0.0f, Coordinates.X * UnitScaled * -1.0, Coordinates.Y * UnitScaled * -1.0);
	SetActorLocation(Location);
}

void ADieg_WorldItemActor::AdjustRotation() const
{
	const FRotator Rotation = FRotator(0, CurrentRotation, 0);
	StaticMeshComponent->SetRelativeRotation(Rotation);

	const FDieg_ItemDefinition& ItemDefinition = ItemInstance->GetItemDefinition();
	FIntPoint ShapeRootOut = ItemDefinition.DefaultShapeRoot;
	TArray<FIntPoint> RotatedShape = UDieg_UtilityLibrary::Rotate2DArrayWithRoot(ItemDefinition.DefaultShape,
		CurrentRotation, ItemDefinition.DefaultShapeRoot, ShapeRootOut);
	const FIntPoint ShapeSpan = UDieg_UtilityLibrary::GetShapeSpan(RotatedShape);

	// Position the mesh so that the root (0,0 after normalization) aligns with the actor's root
	// We need to account for both the shape span and the root position
	const float UnitScaled = GetUnitScaled();
	const FVector Multiplier = GetLocationMultiplier();
	
	// Calculate the mesh position using shape span (as before) but offset by the root position
	// to ensure the root aligns with the actor's root
	const FVector ShapeSpanOffset = FVector(ShapeSpan.Y * UnitScaled, -ShapeSpan.X * UnitScaled, 0.0) * Multiplier;
	const FVector RootOffset = FVector(
		ShapeRootOut.Y * UnitScaled, 
		-ShapeRootOut.X * UnitScaled, 
		0.0
	) * Multiplier;
	
	// Combine both offsets to position the mesh correctly
	StaticMeshComponent->SetRelativeLocation(ShapeSpanOffset - RootOffset);
}

void ADieg_WorldItemActor::AdjustForGrabPoint(const FVector2D& GrabPoint) const
{
	FVector2D LocalGrabPoint = GrabPoint;
	LocalGrabPoint = FVector2D{0.5, 0.5} * GetUnitScaled();

	// Convert 2D coordinates to 3D space. X and Y are not the same in both.
	FVector LocationGrabPoint = FVector{LocalGrabPoint.Y, -LocalGrabPoint.X, 0.0f};
	if (IsOwnedByInventory())
	{
		LocationGrabPoint.Z = OffsetZ;
	}
	
	StaticMeshComponent->AddRelativeLocation(LocationGrabPoint);
}

void ADieg_WorldItemActor::AdjustTransformForGrid() const
{
	TextRendererComponent->SetHiddenInGame(false,false);

	// In local zero rotation: Pitch is Y, Yaw is Z and Roll is X
	// So they are aligned with the grid plane or mesh
	const FRotator Rotation{-90.0f, 0.0, 0.0};
	Root->SetRelativeRotation(Rotation);

	const UDieg_Slot* DefaultSlot = GetDefault<UDieg_Slot>();
	const float InventoryScale3D = DefaultSlot->GetInventoryScale3D();
	Root->SetRelativeScale3D(FVector(InventoryScale3D));
}

void ADieg_WorldItemActor::AdjustTransformForWorld()
{
}

void ADieg_WorldItemActor::HandleDragEnterInventory(const FVector2D& GrabPoint)
{
	AdjustTransformForGrid();
	AdjustRotation(); 
	AdjustForGrabPoint(GrabPoint);
	AdjustText();
}

void ADieg_WorldItemActor::HandleDragLeaveInventory()
{
}

void ADieg_WorldItemActor::HandleStartDragInventory()
{
}

void ADieg_WorldItemActor::HandleStartDragWorld()
{
}

void ADieg_WorldItemActor::HandleStopDragInventory()
{
}

void ADieg_WorldItemActor::HandleStopDragWorld()
{
}

void ADieg_WorldItemActor::HandleResetDragInventory()
{
}

void ADieg_WorldItemActor::HandleResetDragWorld()
{
}

void ADieg_WorldItemActor::HandleRotateItemInventory()
{
}

void ADieg_WorldItemActor::HandleMergeItemInventory()
{
}

void ADieg_WorldItemActor::HandleConsumedItemInventory()
{
}

float ADieg_WorldItemActor::GetUnitScaled() const
{
	// Alternative -> with some math that uses the component size instead and dividing it by the grid size,
	// this should give you a better scale for any item instead of being connected to the Tetris blocks
	const UDieg_Slot* DefaultSlot = GetDefault<UDieg_Slot>();
	const float GridSize3D = DefaultSlot->GetGridSize3D();
	return StaticMeshComponent->GetRelativeScale3D().X * GridSize3D;
}

FVector ADieg_WorldItemActor::GetLocationMultiplier() const
{
	return FVector(UDieg_UtilityLibrary::GetOffsetBasedOnRotation(CurrentRotation));
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

bool ADieg_WorldItemActor::IsOwnedByInventory() const
{
	return IsValid(GetOwner());
}


