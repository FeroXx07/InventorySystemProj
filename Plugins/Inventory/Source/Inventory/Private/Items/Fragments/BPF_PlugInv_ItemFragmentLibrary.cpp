// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"

#include "BPF_PlugInv_DoubleLogger.h"

void FPlugInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	FPlugInv_ConsumableFragment::OnConsume(PC);

	LOG_DOUBLE_S(FColor::Green, "Health potion consumed! Healing by: {0} (FPlugInv_HealthPotionFragment)", this->HealAmount);
}

void FPlugInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	FPlugInv_ConsumableFragment::OnConsume(PC);

	LOG_DOUBLE_S(FColor::Cyan, "Mana potion consumed! Restored by: {0} (FPlugInv_ManaPotionFragment)", this->ManaAmount);

}
