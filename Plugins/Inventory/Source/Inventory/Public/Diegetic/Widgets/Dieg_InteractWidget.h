// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dieg_InteractWidget.generated.h"

class UOverlay;
class UTextBlock;

/**
 * @brief Widget for displaying interaction prompts in the diegetic inventory system.
 * 
 * UDieg_InteractWidget is a user widget that displays interaction prompts to the player
 * when they can interact with objects in the world. It shows text like "Press E to open"
 * and can be positioned in 3D world space.
 * 
 * The widget provides:
 * - 3D world positioning
 * - Interaction text display
 * - Overlay-based UI structure
 * - Blueprint integration for customization
 * 
 * @note This widget is designed to be displayed in 3D world space.
 * @note This widget is Blueprint-compatible and can be customized in Blueprint.
 * 
 * @see UUserWidget
 * @see IDieg_Interactable
 * @see ADieg_PlayerController
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_InteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Root overlay widget for the interaction widget.
	 * 
	 * The main overlay container that holds all other UI elements
	 * in the interaction widget.
	 * 
	 * @see UOverlay
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;
	
	/**
	 * @brief Text block for displaying interaction text.
	 * 
	 * Displays the interaction prompt text to the player,
	 * such as "Press E to open" or "Press F to pick up".
	 * 
	 * @see UTextBlock
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InteractText;

	/**
	 * @brief World location where the widget should be positioned.
	 * 
	 * The 3D world position where this interaction widget
	 * should be displayed in world space.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Interact Widget|Properties", meta = (AllowPrivateAccess = "true"))
	FVector WorldLocation;

	/**
	 * @brief Sets the world position of the interaction widget.
	 * 
	 * Updates the world location where the widget should be displayed.
	 * This is typically called when the player hovers over an interactable object.
	 * 
	 * @param NewPosition The new world position for the widget
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Interact Widget")
	void SetWorldPosition(const FVector& NewPosition);

	/**
	 * @brief Sets the interaction text displayed to the player.
	 * 
	 * Updates the text that is shown to the player when they can
	 * interact with an object, such as "Press E to open".
	 * 
	 * @param Text The new interaction text to display
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Interact Widget")
	void SetInteractText(const FText& Text);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Interact Widget")
	bool SetWidgetPositionFromWorldPosition(const FVector& WorldPosition);
};
