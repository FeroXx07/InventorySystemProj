// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dieg_WorldInventoryActor.generated.h"

class UWidgetInteractionComponent;
class UWidgetComponent;
class UDieg_3DInventoryComponent;
class UDieg_InventoryComponent;

/**
 * @brief Base world inventory actor for the diegetic inventory system.
 * 
 * ADieg_WorldInventoryActor is the base class for all inventory actors that exist
 * in the 3D world. It provides the fundamental functionality for displaying
 * inventories in the world, including widget components, mesh components, and
 * integration with the 3D inventory system.
 * 
 * This actor serves as the foundation for specialized inventory actors like
 * briefcases, chests, and other container objects that can hold items.
 * 
 * The actor provides:
 * - Static mesh component for visual representation
 * - Widget component for inventory UI display
 * - Integration with 3D inventory components
 * - Base functionality for world inventory actors
 * 
 * @note This is an abstract base class that should be inherited by specific
 * inventory actor types.
 * @note This actor is designed to work with UDieg_3DInventoryComponent.
 * 
 * @see ADieg_Briefcase
 * @see UDieg_3DInventoryComponent
 * @see UDieg_InventoryComponent
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API ADieg_WorldInventoryActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Root component for the world inventory actor.
	 * 
	 * The root scene component that serves as the base for all other components
	 * attached to this actor.
	 * 
	 * @see USceneComponent
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Inventory Actor|Components",  meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	/**
	 * @brief Static mesh component for visual representation.
	 * 
	 * Used to display the 3D model of the inventory actor in the world.
	 * This could be a chest, briefcase, or other container model.
	 * 
	 * @see UStaticMeshComponent
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Inventory Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	// // It will change OnConstruction().
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	// bool bUseSkeletalMesh = false;
	//
	// // It will change attachment once in runtime, not in editor.
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	// bool bAttachWidgetToMesh = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Inventory Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Inventory Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Inventory Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_InventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Inventory Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_3DInventoryComponent> InventoryComponent3D;

	// Sets default values for this actor's properties
	ADieg_WorldInventoryActor();

	virtual void PostInitializeComponents() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	// UMeshComponent* GetActiveMesh() const
	// {
	// 	return bUseSkeletalMesh 
	// 		? Cast<UMeshComponent>(SkeletalMeshComponent)
	// 		: Cast<UMeshComponent>(StaticMeshComponent);
	// }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Game|Dieg|World Inventory Actor")
	virtual void Handle3DInventoryBindRequest(UDieg_InventoryComponent*& OutInventoryRef);	
};
