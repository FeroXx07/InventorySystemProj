# Diegetic Inventory System

A modular and extensible inventory system for Unreal Engine.  
It is designed to be **diegetic** (existing inside the game world) and **flexible** (any actor can have an inventory).  
This project was developed as part of a final degree project (TFG).  

---

##  Features

### Core System
- **InventorySlot (`FDieg_InventorySlot`)**  
  Represents one position in the grid and holds an `ItemInstance`.  

- **InventoryComponent (`UDieg_InventoryComponent`)**  
  Collection of slots with logic to add, remove, and search items.  
  Can be attached to any actor (player, container, NPC).  

- **WorldInventoryActor (`ADieg_WorldInventoryActor`)**  
  Actor in the world that has an `InventoryComponent`.  
  Used for testing with a simple cube mesh and widget.  

### Visualization
- **Grid Widget (`UDieg_Grid`)** shows the inventory grid.  
- **Slot Widget (`UDieg_Slot`)** represents a single slot.  
- **Interact Widget (`UDieg_InteractWidget`)** displays prompts when looking at interactables.  

### Player Inputs
- Drag-and-drop system for moving items inside the grid.  
- Correct slot alignment and padding for consistent visuals.  
- Visual feedback: slots highlight when they are valid targets.  

### Containers
- **Briefcase (`ADieg_Briefcase`)**: a prototype portable container with a unique mesh and widget.  
- **3D Widget Support**: inventory UI can be displayed in world space, attached to the container.  

---

##  Installation

1. Clone the repository:  
   ```bash
   git clone https://github.com/yourusername/diegetic-inventory-system.git
