// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_Interactable.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "AC_Interactor.generated.h"

UENUM()
enum class EInteractorFocus : uint8
{
	FocusInValid UMETA(DisplayName = "FocusInValid"),
	FocusValid UMETA(DisplayName = "FocusValid"),
};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FValidInteractableFound, UInteractable*, InteractableComponent);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoValidInteractableFound);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEMPROJ_API UInteractor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Interactor")
	FValidInteractableFound InteractableFoundEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "Interactor")
	FNoValidInteractableFound InteractableNotFoundEvent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interactor")
	EInteractorFocus CurrentFocus = EInteractorFocus::FocusInValid;
	
	UFUNCTION(BlueprintCallable, Category = "Tracer")
	void DoSphereTraceMultiForObjects(UClass* ClassToSeek);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracer")
	FVector TraceStartVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracer")
	FVector TraceEndVector;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Tracer")
	FVector TraceStartVectorOffset = FVector(0.0f, 0.0f, 60.0f);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Tracer")
	float TraceDistance = 200.0f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Tracer")
	float TraceRadius = 30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracer")
	TObjectPtr<APlayerCameraManager> CameraManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracer")
	FHitResult LastValidHit;
};
