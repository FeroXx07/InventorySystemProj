// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/Inv_ItemInstance.h"

void UInv_ItemInstance::Initialize(UInv_ItemDataAsset* InItemDataAsset, int32 InQuantity)
{
}

const UInv_ItemDataAsset* UInv_ItemInstance::GetItemDefinitionDataAsset() const
{
	return nullptr;
}

const UInv_ItemDefinition* UInv_ItemInstance::GetItemDefinition() const
{
	return nullptr;
}

bool UInv_ItemInstance::IsEqual(const UInv_ItemInstance* OtherItemInstance) const
{
	return false;
}

bool UInv_ItemInstance::CanStackWith(const UInv_ItemInstance* OtherItemInstance) const
{
	return false;
}
