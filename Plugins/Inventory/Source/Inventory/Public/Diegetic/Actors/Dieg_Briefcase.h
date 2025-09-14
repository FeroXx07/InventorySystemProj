// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dieg_WorldInventoryActor.h"
#include "Dieg_Briefcase.generated.h"

/**
 * @brief Briefcase actor for the diegetic inventory system.
 * 
 * ADieg_Briefcase is a specialized world inventory actor that represents a briefcase
 * in the 3D world. It extends the base world inventory actor functionality with
 * briefcase-specific features like opening/closing animations and skeletal mesh support.
 * 
 * The briefcase can contain items and be interacted with by players. It provides
 * a portable inventory solution that can be carried around the world.
 * 
 * @note This actor uses a skeletal mesh component for animated briefcase behavior.
 * @note This actor inherits from ADieg_WorldInventoryActor for inventory functionality.
 * 
 * @see ADieg_WorldInventoryActor
 * @see UDieg_InventoryComponent
 * @see UDieg_3DInventoryComponent
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API ADieg_Briefcase : public ADieg_WorldInventoryActor
{
	GENERATED_BODY()

protected:
	/**
	 * @brief The skeletal mesh component for the briefcase.
	 * 
	 * Used for displaying the briefcase model and handling animations
	 * like opening and closing the briefcase.
	 * 
	 * @see USkeletalMeshComponent
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Briefcase|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

public:
	// Sets default values for this actor's properties
	ADieg_Briefcase();

	/**
	 * @brief Opens the briefcase.
	 * 
	 * This function triggers the opening animation and makes the briefcase
	 * inventory accessible to the player. Should be implemented in Blueprint
	 * to handle the specific opening animation for the briefcase.
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|Briefcase")
	void OpenBriefcase();

	/**
	 * @brief Closes the briefcase.
	 * 
	 * This function triggers the closing animation and makes the briefcase
	 * inventory inaccessible to the player. Should be implemented in Blueprint
	 * to handle the specific closing animation for the briefcase.
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|Briefcase")
	void CloseBriefcase();

	/**
	 * @brief Initializes the briefcase with an external inventory component.
	 * 
	 * Sets up the briefcase to use the provided inventory component for
	 * managing items. This allows the briefcase to be connected to an
	 * existing inventory system.
	 * 
	 * @param ExternalInventoryComponent The inventory component to use
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|Briefcase")
	void Initialize(UDieg_InventoryComponent* ExternalInventoryComponent);

protected:
	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
