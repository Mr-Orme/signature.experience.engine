#ifndef COMPONENTSLIST_H
#define COMPONENTSLIST_H

/*If you add a component you need to make the following changes.
 1. Asset in xml 
 2. enum in AssetLibrary
 3. switch in AssetLibrary.cpp 
 4. if statements in ResourceManager.cpp
*/
#include "Component.h"
#include "BodyComponent.h"
//Changed from HealthComponent to StatComponent
#include "StatComponent.h"
#include "SpriteComponent.h"

#include "UserInputComponent.h"
//#include "BackpackComponent.h"
//#include "InventoryComponent.h"
//#include "GhostComponent.h"



#endif