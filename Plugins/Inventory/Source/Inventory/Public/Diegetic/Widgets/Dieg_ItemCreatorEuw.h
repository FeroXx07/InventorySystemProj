// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dieg_Grid.h"
#include "Editor/ScriptableEditorWidgets/Public/Components/SinglePropertyView.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetComponents.h"
#include "Dieg_ItemCreatorEuw.generated.h"

class UDieg_ItemDefinitionDataAsset;
class USpinBox;
class UComboBox;
class UButton;
class UCanvasPanel;
class USizeBox;

/**
 * @brief Editor utility widget for creating item definitions in the diegetic inventory system.
 * 
 * UDieg_ItemCreatorEuw is an editor utility widget that provides a user interface
 * for creating and configuring item definitions in the Unreal Editor. It allows
 * designers to set up item properties, visual assets, and inventory shapes
 * without needing to modify code.
 * 
 * The widget provides:
 * - Item definition creation and editing
 * - Visual asset assignment
 * - Inventory shape configuration
 * - Property editing interface
 * - Grid-based shape preview
 * 
 * @note This is an editor-only widget and is not available in packaged builds.
 * @note This widget extends UEditorUtilityWidget for editor integration.
 * 
 * @see UEditorUtilityWidget
 * @see UDieg_ItemDefinitionDataAsset
 * @see FDieg_ItemDefinition
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_ItemCreatorEuw : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CanvasPanel_Modify;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditorUtilityButton> Button_Modify;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditorUtilityButton> Button_Confirm;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditorUtilityComboBoxString> ComboBox_DataAsset;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USpinBox> SpinBox_GridSize;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USinglePropertyView> SinglePropertyView_Name;
	
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USinglePropertyView> SinglePropertyView_StaticMesh;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USinglePropertyView> SinglePropertyView_MaxQuantity;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_Grid> GridWidget;

	UPROPERTY(EditAnywhere, Category = "Game|Dieg|Item Creator EUW|Containers", meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<UDieg_ItemDefinitionDataAsset>> DataAssetMap;

	UPROPERTY(EditAnywhere, Category = "Game|Dieg|Item Creator EUW|Containers", meta = (AllowPrivateAccess = "true"))
	TSet<FIntPoint> SelectedSlots;

	const FString EmptySelectionOption = "Create New Item";

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	void SetupGrid(int32 GridSize) const;

	UPROPERTY(EditAnywhere, Category = "Game|Dieg|Item Creator EUW|Properties")
	TSubclassOf<UPrimaryDataAsset> DataAssetClass;
	
	UPROPERTY(EditAnywhere, Category = "Game|Dieg|Item Creator EUW|Properties")
	FDirectoryPath ItemsDataAssetsStoragePath;

	UPROPERTY(EditAnywhere, Category = "Game|Dieg|Item Creator EUW|Properties")
	FString ItemsDataAssetsPrefix = "DA_Items_";

	// To bind to SinglePropertyView it caches its variable name. 
	UPROPERTY(EditInstanceOnly, Category = "Game|Dieg|Item Creator EUW|Properties")
	FText Property_Name;
	
	// To bind to SinglePropertyView it caches its variable name. 
	UPROPERTY(EditInstanceOnly, Category = "Game|Dieg|Item Creator EUW|Properties")
	TObjectPtr<UStaticMesh> Property_StaticMesh;

	// To bind to SinglePropertyView it caches its variable name. 
	UPROPERTY(EditInstanceOnly, Category = "Game|Dieg|Item Creator EUW|Properties")
	int32 Property_MaxQuantity;
	
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	void ModifyDataAsset();
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	void CreateDataAsset();
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	void SelectDataAsset(const FString& Option);
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	void AddDataAssetToMap(FString Name, UDieg_ItemDefinitionDataAsset* DataAsset);
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	void RefreshItemList();

	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|Item Creator EUW|Loading")
	void HandleSlotPressed(UDieg_Slot* GridSlot);
	
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	FString GetSelectedName() const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	UDieg_ItemDefinitionDataAsset* GetSelectedDataAsset();
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	int32 GetGridSize() const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Creator EUW")
	bool IsValidAsset() const;

private:
	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void GridSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void GridSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void GridSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void GridSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void HandleButtonModifyClicked();

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void HandleButtonConfirmClicked();

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void HandleComboBoxDaChange(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void HandleComboBoxDaOpening();

	UFUNCTION(Category = "Game|Dieg|Item Creator EUW")
	void HandleSpinBoxGridSizeChange(float InValue);
};
