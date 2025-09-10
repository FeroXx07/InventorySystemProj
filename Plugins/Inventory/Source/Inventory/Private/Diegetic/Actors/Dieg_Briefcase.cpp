// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Actors/Dieg_Briefcase.h"

#include "Diegetic/Dieg_UtilityLibrary.h"
#include "Diegetic/Components/Dieg_3DInventoryComponent.h"


// Sets default values
ADieg_Briefcase::ADieg_Briefcase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADieg_Briefcase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UDieg_UtilityLibrary::CacheComponent(this, this->SkeletalMeshComponent);
}

// Called when the game starts or when spawned
void ADieg_Briefcase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADieg_Briefcase::OpenBriefcase_Implementation()
{
}

void ADieg_Briefcase::CloseBriefcase_Implementation()
{
}

// Called every frame
void ADieg_Briefcase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADieg_Briefcase::Initialize_Implementation(UDieg_InventoryComponent* ExternalInventoryComponent)
{
	this->InventoryComponent = ExternalInventoryComponent;
}

