// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AC_Interactor.h"

#include "Kismet/GameplayStatics.h"
#include "ActorComponent/AC_Interactable.h"
#include "BlueprintFunctionLibrary/BPF_DoubleLogger.h"
// Sets default values for this component's properties
UInteractor::UInteractor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UDoubleLogger::LogWarning("AC_Interactor C++ Begin Play");
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}


// Called every frame
void UInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UClass* ClassToSeek = UInteractable::StaticClass();
	DoSphereTraceMultiForObjects(ClassToSeek);
}
void UInteractor::DoSphereTraceMultiForObjects(UClass* ClassToSeek)
{
	TraceStartVector = GetOwner()->GetActorLocation() + TraceStartVectorOffset;
	TraceEndVector = TraceStartVector + CameraManager->GetActorForwardVector() * TraceDistance;
	constexpr bool TraceComplexCollisions = false;
	constexpr bool IgnoreSelf = true;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(nullptr);
	
	FHitResult HitResult;

	if (const bool Hit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStartVector, TraceEndVector, TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility)
		,TraceComplexCollisions, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, IgnoreSelf,
		FLinearColor::Red, FLinearColor::Green, 5.0f))
	{
		TObjectPtr<UInteractable> Interactable = HitResult.GetActor()->GetComponentByClass<UInteractable>();

		// Call not found event only once, avoid spam, useful for single new un-focus event.
		if (!IsValid(Interactable) && CurrentFocus == EInteractorFocus::FocusValid)
		{
			InteractableNotFoundEvent.Broadcast();
			CurrentFocus = EInteractorFocus::FocusInValid;
			return;
		}

		// Call found event only once, avoid spam, useful for single new focus event.
		if (IsValid(Interactable) && CurrentFocus == EInteractorFocus::FocusInValid)
		{
			InteractableFoundEvent.Broadcast(Interactable);	
			CurrentFocus = EInteractorFocus::FocusValid;
		}

		LastValidHit = HitResult;
		
		FString Name = HitResult.GetActor()->GetFName().ToString();

		TArray< FStringFormatArg > args; 
		args.Add( FStringFormatArg( Name )); 
		FString logString = FString::Format( TEXT( "Trace hit. Out actor name: {0}" ), args );
		
		//UDoubleLogger::Log(logString);
	}
}

