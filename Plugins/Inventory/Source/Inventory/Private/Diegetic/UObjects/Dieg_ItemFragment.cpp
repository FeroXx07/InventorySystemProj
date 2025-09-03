// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/UObjects/Dieg_ItemFragment.h"

const FGameplayTag& UDieg_ItemFragment::GetFragmentTag() const
{
	return FragmentTag;
}

void UDieg_ItemFragment::SetFragmentTag(const FGameplayTag& Tag)
{
	this->FragmentTag = Tag;
}
