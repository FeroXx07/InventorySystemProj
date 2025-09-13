// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dieg_WorldInventoryActor.generated.h"

class UWidgetInteractionComponent;
class UWidgetComponent;
class UDieg_3DInventoryComponent;
class UDieg_InventoryComponent;

UCLASS()
class INVENTORY_API ADieg_WorldInventoryActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Inventory Actor|Components",  meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
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
