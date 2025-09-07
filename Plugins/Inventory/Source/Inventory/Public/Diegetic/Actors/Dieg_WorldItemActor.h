// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Diegetic/Interfaces/Dieg_Interactable.h"
#include "Diegetic/UStructs/Dieg_PrePopulate.h"
#include "GameFramework/Actor.h"
#include "Dieg_WorldItemActor.generated.h"

class UDieg_ItemInstance;
class UDieg_ItemDefinitionDataAsset;

UCLASS()
class INVENTORY_API ADieg_WorldItemActor : public AActor, public IDieg_Interactable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADieg_WorldItemActor();
	
private:
	/** Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Items" , meta = (AllowPrivateAccess = "true")) 
	FDieg_PrePopulate PrePopulateData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_ItemInstance> ItemInstance;
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void SetFromDataAsset(UDieg_ItemDefinitionDataAsset* InItemDataAsset);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	float OffsetZ{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	float CurrentRotation{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	FVector LastValidWorldLocation{FVector{0.0f,0.0f,0.0f}};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	int32 Quantity {1};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract_Implementation(UObject* Interactor) override;

	const TObjectPtr<UDieg_ItemInstance>& GetItemInstance() const;
	TObjectPtr<UDieg_ItemInstance>& GetItemInstanceMutable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void SetItemInstance(UDieg_ItemInstance* Instance);
};
