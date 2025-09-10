// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dieg_WorldInventoryActor.h"
#include "Dieg_Briefcase.generated.h"

UCLASS()
class INVENTORY_API ADieg_Briefcase : public ADieg_WorldInventoryActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

public:
	// Sets default values for this actor's properties
	ADieg_Briefcase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	void OpenBriefcase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	void CloseBriefcase();

protected:
	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	void Initialize(UDieg_InventoryComponent* ExternalInventoryComponent);
};
