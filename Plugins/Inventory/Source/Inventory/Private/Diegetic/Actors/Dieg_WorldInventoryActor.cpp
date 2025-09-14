// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Actors/Dieg_WorldInventoryActor.h"

#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Diegetic/Dieg_UtilityLibrary.h"
#include "Diegetic/Components/Dieg_3DInventoryComponent.h"
#include "Diegetic/Components/Dieg_InventoryComponent.h"


// Sets default values
ADieg_WorldInventoryActor::ADieg_WorldInventoryActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	// StaticMeshComponent->SetupAttachment(Root);
	//
	// // Load default cube mesh from Engine Content
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	// if (CubeMesh.Succeeded())
	// {
	// 	StaticMeshComponent->SetStaticMesh(CubeMesh.Object);
	// }
	// // Load default engine material
	// static ConstructorHelpers::FObjectFinder<UMaterial> CubeMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	// if (CubeMaterial.Succeeded())
	// {
	// 	StaticMeshComponent->SetMaterial(0, CubeMaterial.Object);
	// }
	//
	// SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	// SkeletalMeshComponent->SetupAttachment(Root);
	//
	// WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	// WidgetComponent->SetupAttachment(Root);
	//
	// WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction Component"));
	// WidgetInteractionComponent->SetupAttachment(WidgetComponent);
	//
	// //InventoryComponent = CreateDefaultSubobject<UDieg_InventoryComponent>(TEXT("Inventory Component"));
	InventoryComponent3D = CreateDefaultSubobject<UDieg_3DInventoryComponent>(TEXT("3D Inventory Component"));
	//
	// // Default: only static visible
	// SkeletalMeshComponent->SetHiddenInGame(true);
	// SkeletalMeshComponent->SetVisibility(false);
	// SkeletalMeshComponent->SetComponentTickEnabled(false);
}

void ADieg_WorldInventoryActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UDieg_UtilityLibrary::CacheComponent(this, this->InventoryComponent);
	UDieg_UtilityLibrary::CacheComponent(this, this->StaticMeshComponent);
	
	// Only cache widget components if they exist
	if (IsValid(WidgetComponent))
	{
		UDieg_UtilityLibrary::CacheComponentChecked(this, this->WidgetComponent);
	}
	if (IsValid(WidgetInteractionComponent))
	{
		UDieg_UtilityLibrary::CacheComponentChecked(this, this->WidgetInteractionComponent);
	}
	
	if (!InventoryComponent3D->OnInventoryExternalLinkRequest.IsAlreadyBound(this, &ThisClass::Handle3DInventoryBindRequest))
	{
		InventoryComponent3D->OnInventoryExternalLinkRequest.AddDynamic(this, &ThisClass::Handle3DInventoryBindRequest);
	}
}

void ADieg_WorldInventoryActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// if (bUseSkeletalMesh)
	// {
	// 	SkeletalMeshComponent->SetHiddenInGame(false);
	// 	SkeletalMeshComponent->SetVisibility(true);
	// 	SkeletalMeshComponent->SetComponentTickEnabled(true);
	//
	// 	StaticMeshComponent->SetHiddenInGame(true);
	// 	StaticMeshComponent->SetVisibility(false);
	// 	StaticMeshComponent->SetComponentTickEnabled(false);
	//
	// 	if (bAttachWidgetToMesh)
	// 	{
	// 		// Attach widget to skeletal mesh if needed
	// 		WidgetComponent->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// 	}
	// }
	// else
	// {
	// 	StaticMeshComponent->SetHiddenInGame(false);
	// 	StaticMeshComponent->SetVisibility(true);
	// 	StaticMeshComponent->SetComponentTickEnabled(true);
	//
	// 	SkeletalMeshComponent->SetHiddenInGame(true);
	// 	SkeletalMeshComponent->SetVisibility(false);
	// 	SkeletalMeshComponent->SetComponentTickEnabled(false);
	//
	// 	if (bAttachWidgetToMesh)
	// 	{
	// 		// Attach widget to static mesh if needed
	// 		WidgetComponent->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// 	}
	// }
	//
	// if (!bAttachWidgetToMesh)
	// {
	// 	// Ensure widget is attached to root if not attached to a mesh
	// 	if (WidgetComponent->GetAttachParent() != Root)
	// 	{
	// 		WidgetComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	// 	}
	// }
}


// Called when the game starts or when spawned
void ADieg_WorldInventoryActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADieg_WorldInventoryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADieg_WorldInventoryActor::Handle3DInventoryBindRequest(UDieg_InventoryComponent*& OutInventoryRef)
{
	OutInventoryRef = InventoryComponent;
}

