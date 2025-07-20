// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/AC_PlugInv_HighlightableStaticMesh.h"

void UPlugInv_HighlightableStaticMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UPlugInv_HighlightableStaticMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
