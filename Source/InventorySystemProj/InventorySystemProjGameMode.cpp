// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemProjGameMode.h"
#include "InventorySystemProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventorySystemProjGameMode::AInventorySystemProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
