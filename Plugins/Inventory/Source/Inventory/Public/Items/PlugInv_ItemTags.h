// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 *	You can add (or remove) tags with one of the following methods:
 *	Directly adding or removing them in Project Settings
 *	Importing them from Data Table assets
 *	Defining them with C++
 *	https://dev.epicgames.com/documentation/en-us/unreal-engine/using-gameplay-tags-in-unreal-engine
 **/

namespace GameItems
{
	namespace Equipment
	{
		namespace Weapons
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sword);
		}

		namespace Cloaks
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RedCloak);

		}

		namespace Masks
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(SteelMask);
		}
	}
	namespace Consumables
	{
		namespace Potions
		{
			namespace Red
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large);
			}
			namespace Blue
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large);
			}
		}
	}

	namespace Craftables
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireFernFruit);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(LuminDaisy);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ScorchPetalBlossom);
	}
}
