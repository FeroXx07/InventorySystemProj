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

namespace FragmentTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GridFragment);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IconFragment);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StackableFragment);
}
