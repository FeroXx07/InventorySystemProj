// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Manifest/F_PlugInv_ItemManifest.h"

#include "O_PlugInv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InventoryItem : public UObject
{
	GENERATED_BODY()

public:
	// Replication with UObjects needs to call GetLifetimeReplicatedProps()
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// So that it can be a registered sub object lists.
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
	void SetItemManifest(const FPlugInv_ItemManifest& Manifest);
	const FPlugInv_ItemManifest& GetItemManifest() const{ return ItemManifest.Get<FPlugInv_ItemManifest>(); }
	FPlugInv_ItemManifest& GetItemManifestMutable(){ return ItemManifest.GetMutable<FPlugInv_ItemManifest>(); }

	int32 GetTotalStackCount() const{ return TotalStackCount; }
	void SetTotalStackCount(const int32 Value){this->TotalStackCount = Value; }

private:

	// Constraint its inheritance capabilities to only FPlugInv_ItemManifest.
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.FPlugInv_ItemManifest"), Replicated)
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount{0};
};

template <typename FragmentType>
const FragmentType* GetFragment(const UPlugInv_InventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item))
	{
		return nullptr;
	};

	const FPlugInv_ItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentOfTypeByTag<FragmentType>(Tag);
}