// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/AC_PlugInv_Item.h"


// Sets default values for this component's properties
UPlugInv_Item::UPlugInv_Item()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PickupMessage = TEXT("E - Pick Up");
	// ...
}


