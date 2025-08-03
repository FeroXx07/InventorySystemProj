// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/F_PlugInv_ItemManifest.h"
#include "AC_PlugInv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UPlugInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlugInv_ItemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Manifest Getter.
	FPlugInv_ItemManifest GetItemManifest() const
	{
		return ItemManifest;
	}
	
	// Message Getter.
	FString GetPickupMessage() const
	{
		return PickupMessage;
	}

	void PickUp();
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickUp();
private:

	// The Manifest type to create Inventory Items.
	UPROPERTY(EditAnywhere, Category = "Inventory", Replicated)
	FPlugInv_ItemManifest ItemManifest;

	// The Pickup message when hovering onto the owning actor.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
