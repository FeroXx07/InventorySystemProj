# Spatial Inventory System

This document describes the spatial inventory system implemented in the InventorySys plugin.

## Overview

The spatial inventory system allows items to have diverse spatial shapes and be placed in a grid-based inventory with collision detection, rotation support, and realistic inventory management. Items can have various shapes like L, O, U, rectangles, squares, and custom shapes.

## Core Components

### 1. Inv_InventoryContainer
Represents a single inventory container with spatial grid-based item placement.

**Key Features:**
- Grid-based occupancy tracking
- Spatial item placement with collision detection
- Item rotation support
- Container locking/unlocking
- Fill percentage tracking

**Usage:**
```cpp
// Create a container
UInv_InventoryContainer* Container = NewObject<UInv_InventoryContainer>();
Container->Initialize(FName("MainContainer"), FText::FromString("Main Inventory"), 10, 10);

// Place an item
bool bPlaced = Container->TryPlaceItem(ItemInstance, FIntPoint(0, 0), 0.0f);
```

### 2. Inv_InventoryComponent
Enhanced inventory component that manages multiple containers.

**Key Features:**
- Multiple container management
- Container creation and removal
- Item transfer between containers
- Spatial placement validation

**Usage:**
```cpp
// Initialize spatial inventory system
InventoryComponent->InitializeSpatialInventory();

// Create a container
UInv_InventoryContainer* Container = InventoryComponent->CreateContainer(
    FName("Backpack"), 
    FText::FromString("Backpack"), 
    6, 8
);

// Add item to specific container
bool bAdded = InventoryComponent->TryAddItemToContainer(
    ItemInstance, 
    FName("Backpack"), 
    FIntPoint(0, 0), 
    90.0f
);
```

### 3. Inv_ItemDefinition (Enhanced)
Enhanced item definition with spatial shape support.

**New Properties:**
- `DefaultShape`: Array of FIntPoint defining the item's shape
- `bCanRotate`: Whether the item can be rotated
- `ShapeWidth/ShapeHeight`: Dimensions of the shape
- `ShapeColor`: Color for UI visualization

**Usage:**
```cpp
// Create item definition with L-shape
UInv_ItemDefinition* ItemDef = NewObject<UInv_ItemDefinition>();
ItemDef->DefaultShape = {
    FIntPoint(0, 0),
    FIntPoint(0, 1),
    FIntPoint(0, 2),
    FIntPoint(1, 2)
};
ItemDef->bCanRotate = true;
ItemDef->AutoCalculateShapeDimensions();
```

### 4. Inv_SpatialPlacementUtility
Utility class for spatial placement operations.

**Key Functions:**
- `FindBestPlacement()`: Find optimal placement for an item
- `CanPlaceItemAt()`: Check if item can be placed at position
- `GetValidPlacementPositions()`: Get all valid placement positions
- `GetContainerFillPercentage()`: Get container utilization
- `GetEmptyRegions()`: Find empty areas in container

**Usage:**
```cpp
// Find best placement
UInv_SpatialPlacementUtility::FPlacementOptions Options;
UInv_SpatialPlacementUtility::FPlacementResult Result = 
    UInv_SpatialPlacementUtility::FindBestPlacement(Container, Item, Options);

if (Result.bCanPlace)
{
    Container->TryPlaceItem(Item, Result.BestPosition, Result.BestRotation);
}
```

## Example Usage

### Basic Setup
```cpp
// In your actor's BeginPlay()
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize spatial inventory system
    InventoryComponent->InitializeSpatialInventory();
    
    // Create containers
    InventoryComponent->CreateContainer(FName("Main"), FText::FromString("Main Inventory"), 10, 10);
    InventoryComponent->CreateContainer(FName("Backpack"), FText::FromString("Backpack"), 6, 8);
}
```

### Creating Realistic Items
```cpp
// Create sword item
TArray<FIntPoint> SwordShape = {
    FIntPoint(1, 0), // Crossguard
    FIntPoint(0, 1), // Blade
    FIntPoint(1, 1), // Blade
    FIntPoint(2, 1), // Crossguard
    FIntPoint(1, 2), // Blade
    FIntPoint(1, 3), // Blade
    FIntPoint(1, 4)  // Blade
};

UInv_ItemInstance* SwordItem = CreateItemWithShape("Sword", SwordShape);
```

### Placing Items
```cpp
// Find best placement
UInv_SpatialPlacementUtility::FPlacementOptions Options;
Options.bAllowRotation = true;
Options.bFindBestFit = true;

UInv_SpatialPlacementUtility::FPlacementResult Result = 
    UInv_SpatialPlacementUtility::FindBestPlacement(Container, Item, Options);

if (Result.bCanPlace)
{
    Container->TryPlaceItem(Item, Result.BestPosition, Result.BestRotation);
}
```

### Container Analysis
```cpp
// Get container fill percentage
int32 FillPercentage = UInv_SpatialPlacementUtility::GetContainerFillPercentage(Container);

// Find empty regions
TArray<FIntPoint> EmptyRegions = UInv_SpatialPlacementUtility::GetEmptyRegions(Container, 4);
```

## Realistic Item Shapes

The system supports various realistic item shapes:

### Sword
```
 X
X X
 X
 X
 X
```

### Shield
```
 X
XXX
XXX
```

### Potion (2x2)
```
XX
XX
```

### Armor (L-shape)
```
XXX
XX
 X
```

### Tool (Hammer)
```
XX
XX
 X
```

### Book (Rectangle)
```
XX
XX
```

### Key (Small L)
```
XX
X
```

### Gem (Diamond)
```
 X
X X
 X
```

## Configuration

### Container Settings
- **Grid Width/Height**: Size of the container grid
- **Container Tags**: Gameplay tags for filtering
- **Lock State**: Whether container can be modified

### Item Settings
- **Shape Definition**: Array of FIntPoint defining the item's spatial footprint
- **Rotation Support**: Whether item can be rotated
- **Shape Dimensions**: Width and height of the shape
- **Visual Properties**: Color and icon for UI

### Placement Options
- **Allow Rotation**: Whether to try different rotations
- **Find Best Fit**: Whether to find optimal placement
- **Prefer Top-Left**: Whether to prefer top-left positions
- **Rotation Step**: Degrees between rotation attempts

## Best Practices

1. **Shape Design**: Keep shapes connected and reasonable in size
2. **Container Sizing**: Balance container size with gameplay needs
3. **Rotation**: Use rotation sparingly for complex shapes
4. **Performance**: Limit the number of placement attempts for large containers
5. **UI Integration**: Use the spatial information for visual feedback

## Debugging

### Debug Functions
```cpp
// Print container grid
UInv_SpatialPlacementUtility::DebugPrintGrid(Container);

// Print item shape
UInv_SpatialPlacementUtility::DebugPrintShape(Shape, "Sword");

// Print container status
Container->DebugPrintGrid();
```

### Common Issues
1. **Invalid Shapes**: Ensure all shape points are non-negative
2. **Placement Failures**: Check container bounds and occupancy
3. **Rotation Issues**: Verify rotation step and maximum steps
4. **Performance**: Limit container size and shape complexity

## Future Enhancements

- **Shape Templates**: Predefined shape libraries for common items
- **Advanced Rotation**: Non-90-degree rotations for more realistic placement
- **Shape Validation**: Automatic shape connectivity checking
- **Performance Optimization**: Spatial indexing for large containers
- **UI Integration**: Visual shape preview and placement feedback
- **Auto-Organization**: Automatic item arrangement and optimization
