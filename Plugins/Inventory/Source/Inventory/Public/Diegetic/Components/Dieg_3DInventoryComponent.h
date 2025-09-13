// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dieg_3DInventoryComponent.generated.h"


class ADieg_WorldItemActor;
class UDieg_Slot;
class UWidgetComponent;
class UDieg_Grid;
class UDieg_InventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLinkInventoryRefExternally, UDieg_InventoryComponent*&, NewInventoryRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FWidgetSlotHover, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWidgetSlotUnHover, const FPointerEvent&, InMouseEvent, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandlerHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, HoveredItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandlerUnHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, HoveredItem);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UDieg_3DInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDieg_3DInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FLinkInventoryRefExternally OnInventoryExternalLinkRequest;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetSlotHover On3DWidgetSlotHovered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetSlotUnHover On3DWidgetSlotUnHovered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHandlerHoverItem OnHoverItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHandlerUnHoverItem OnUnHoverItem;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_InventoryComponent> InventoryComponentRef;

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<ADieg_WorldItemActor*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADieg_WorldItemActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_Grid> GridWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UWidgetComponent> WidgetComponentRef;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	bool bIsInitialized{false};

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void PreInitialize();

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void Initialize();

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void Populate3D();

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void DeInitializeInventory();
	
	UFUNCTION(Category = "Event Handler", BlueprintCallable)
	void WidgetSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Event Handler", BlueprintCallable)
	void WidgetSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

public:
	UFUNCTION(Category = "Direct", BlueprintCallable)
	void AddItemToInventory(ADieg_WorldItemActor* ItemActor);

	UFUNCTION(Category = "Direct", BlueprintCallable)
	void AddItemToInventorySlot(ADieg_WorldItemActor* ItemActor, const FIntPoint& SlotCoordinates, int32 RotationUsed);

	UFUNCTION(Category = "Direct", BlueprintCallable)
	void RemoveItemFromInventory(ADieg_WorldItemActor* ItemActor);

	
	UFUNCTION(BlueprintCallable)
	UDieg_Grid* GetGridWidget() {return GridWidget;}

	TWeakObjectPtr<UWidgetComponent> GetWidgetComponent() {return WidgetComponentRef;}
	TObjectPtr<UDieg_InventoryComponent> GetInventoryComponent() {return InventoryComponentRef;}

	TArray<ADieg_WorldItemActor*>& GetItems() {return Items;}
};
