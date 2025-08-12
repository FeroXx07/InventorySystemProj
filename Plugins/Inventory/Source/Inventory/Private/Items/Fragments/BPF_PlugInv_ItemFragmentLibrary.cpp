// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"
#include "Widgets/Composite/UW_PlugInv_CompositeBase.h"
#include "BPF_PlugInv_DoubleLogger.h"
#include "Widgets/Composite/UW_PlugInv_Leaf_Image.h"
#include "Widgets/Composite/UW_PlugInv_Leaf_LabeledValue.h"
#include "Widgets/Composite/UW_PlugInv_Leaf_Text.h"


void FPlugInv_ConsumableFragment::OnConsume(APlayerController* PC)
{
	for (TInstancedStruct<FPlugInv_ConsumeModifier>& Modifier : ConsumeModifiers)
	{
		FPlugInv_ConsumeModifier& ModRef = Modifier.GetMutable();
		ModRef.OnConsume(PC);
	}
}

void FPlugInv_ConsumableFragment::Assimilate(UPlugInv_CompositeBase* Composite) const
{
	FPlugInv_InventoryItemFragment::Assimilate(Composite);
	
	for (const TInstancedStruct<FPlugInv_ConsumeModifier>& Modifier : ConsumeModifiers)
	{
		const FPlugInv_ConsumeModifier& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FPlugInv_ConsumableFragment::Manifest()
{
	FPlugInv_InventoryItemFragment::Manifest();

	for (TInstancedStruct<FPlugInv_ConsumeModifier>& Modifier : ConsumeModifiers)
	{
		FPlugInv_ConsumeModifier& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

void FPlugInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	FPlugInv_ConsumeModifier::OnConsume(PC);
	LOG_DOUBLE_S(5, FColor::Green, "Health potion consumed! Healing by: {0} (FPlugInv_HealthPotionFragment)", this->GetValue());
}

void FPlugInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	FPlugInv_ConsumeModifier::OnConsume(PC);
	LOG_DOUBLE_S(5, FColor::Cyan, "Mana potion consumed! Restored by: {0} (FPlugInv_ManaPotionFragment)", this->GetValue());
}

void FPlugInv_InventoryItemFragment::Assimilate(UPlugInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FPlugInv_InventoryItemFragment::MatchesWidgetTag(const UPlugInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(this->GetFragmentTag());
}

void FPlugInv_TextFragment::Assimilate(UPlugInv_CompositeBase* Composite) const
{
	FPlugInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	const UPlugInv_Leaf_Text* LeafText = Cast<UPlugInv_Leaf_Text>(Composite);
	if (!IsValid(LeafText)) return;
	
	LeafText->SetText(FragmentText);
}

void FPlugInv_LabeledNumberFragment::Assimilate(UPlugInv_CompositeBase* Composite) const
{
	FPlugInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	const UPlugInv_Leaf_LabeledValue* LabeledValue = Cast<UPlugInv_Leaf_LabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;
	
	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;
	LabeledValue->SetText_Value(FText::AsNumber(Value, &Options), bCollapseValue);
}

void FPlugInv_LabeledNumberFragment::Manifest()
{
	FPlugInv_InventoryItemFragment::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(Min, Max);
	}
	bRandomizeOnManifest = false;
}

void FPlugInv_ImageFragment::Assimilate(UPlugInv_CompositeBase* Composite) const
{
	FPlugInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	const UPlugInv_Leaf_Image* LeafImage = Cast<UPlugInv_Leaf_Image>(Composite);
	if (!IsValid(LeafImage)) return;
	
	LeafImage->SetImage(IconTexture);
	LeafImage->SetBoxSize(IconDimensions);
	LeafImage->SetImageSize(IconDimensions);
}
