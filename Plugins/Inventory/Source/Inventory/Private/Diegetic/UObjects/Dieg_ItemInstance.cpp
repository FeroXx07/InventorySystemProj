// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/UObjects/Dieg_ItemInstance.h"
#include "Diegetic/UObjects/Dieg_ItemFragment.h"

void UDieg_ItemInstance::Initialize(UDieg_ItemDefinitionDataAsset* Def, int32 InQuantity)
{
	// checkf(bIsInitialized == false, TEXT("UDieg_ItemInstance::Initialize, Initializing Item Instance Twice"));
	checkf(Def != nullptr,  TEXT("UDieg_ItemInstance::Initialize, Initializing Item Instance from NULL data asset"));
	
	bIsInitialized = true;
	ItemDefinitionDataAsset = Def;
	const FDieg_ItemDefinition& ItemDefinition = ItemDefinitionDataAsset->ItemDefinition;

	ItemTags.AddTag(ItemDefinition.ItemType);
	Quantity = FMath::Clamp(InQuantity, 1, ItemDefinition.StackSizeMax);
	
	// Instance fragments from Item Definition
	ItemFragments.Empty();
	for (UDieg_ItemFragment* Fragment : ItemDefinition.Fragments) 
	{
		if (Fragment) 
		{
			UDieg_ItemFragment* InstancedFragment = DuplicateObject(Fragment, this);
			InstancedFragment->OnInstanced(this);
			ItemFragments.Add(InstancedFragment);
		}
	}
}

bool UDieg_ItemInstance::IsEqual(const UDieg_ItemInstance* ToCheck) const
{
	if (!ToCheck)
		return false;

	// Compare Item Definition
	if (ItemDefinitionDataAsset != ToCheck->ItemDefinitionDataAsset)
		return false;

	// Compare Item Tags
	if (ItemTags != ToCheck->ItemTags)
		return false;

	// Compare Fragment states
	if (ItemFragments.Num() != ToCheck->ItemFragments.Num())
		return false;

	for (int32 i = 0; i < ItemFragments.Num(); ++i)
	{
		const UDieg_ItemFragment* MyFrag = ItemFragments[i];
		const UDieg_ItemFragment* OtherFrag = ToCheck->ItemFragments[i];

		if (!MyFrag || !OtherFrag)
			return false;

		if (!MyFrag->IsEqual(OtherFrag)) 
			return false;
	}

	return true;
}


bool UDieg_ItemInstance::CanStackWith(const UDieg_ItemInstance* ToCheck) const
{
	return IsEqual(ToCheck);
}

UDieg_ItemDefinitionDataAsset* UDieg_ItemInstance::GetItemDefinitionDataAsset() const
{
	return ItemDefinitionDataAsset;
}
