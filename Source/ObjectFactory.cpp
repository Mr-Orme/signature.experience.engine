#include <iostream>
#include <string>
#include "ObjectFactory.h"
#include "Component.h"
#include "ComponentsList.h"
#include "View.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Object.h"
#include "AssetLibrary.h"
#include "Initializers.h"

using namespace std;

SpriteComponent* createSpriteComponent(Object* newObject, ResourceManager* devices, tinyxml2::XMLElement * componentElement)
{
	bool isSprite{ false };
	componentElement->QueryBoolAttribute("sprite", &isSprite);

	SpritePresets initializers;
	initializers.Devices = devices;
	if (isSprite)
	{
		initializers.spriteTexture = devices->assetLibrary->getArtAsset(componentElement->Attribute("asset"));
	}
	else
	{
		initializers.spriteTexture = new Texture(devices->gDevice.get(), componentElement->Attribute("text"), false);
	}
	return new SpriteComponent(newObject, initializers);
}
BodyComponent* createBodyComponent(Object* newObject, ResourceManager* devices, tinyxml2::XMLElement * componentElement)
{
	BodyPresets initializers;
	componentElement->QueryFloatAttribute("x", (float*)(&initializers.position.x));
	componentElement->QueryFloatAttribute("y", (float*)(&initializers.position.y));
	componentElement->QueryFloatAttribute("density", (float*)(&initializers.physics.density));
	componentElement->QueryFloatAttribute("restitution", (float*)(&initializers.physics.restitution));
	componentElement->QueryFloatAttribute("angularDamping", (float*)(&initializers.physics.angularDamping));
	componentElement->QueryFloatAttribute("linearDamping", (float*)(&initializers.physics.linearDamping));
		
	if (componentElement->Attribute("bodyType") == "DYNAMIC") 
	{ 
		initializers.physics.bodyType = BodyType::Dynamic; 
	}
	else if (componentElement->Attribute("bodyType") == "STATIC") 
	{ 
		initializers.physics.bodyType = BodyType::Static; 
	}

	if (componentElement->Attribute("bodyShape") == "RECTANGLE") 
	{ 
		initializers.physics.bodyShape = BodyShape::Rectangle; 
	}
	else if (componentElement->Attribute("bodyShape") == "CIRCLE") 
	{ 
		initializers.physics.bodyShape = BodyShape::Circle; 
	}
	
	componentElement->QueryBoolAttribute("physicsOn", &initializers.physics.physicsOn);
	
	return new BodyComponent(newObject, devices, initializers);
}
JointType setJointType(string type)
{
	if (type == "Revolute") {return JointType::Revolute;}
	else if (type == "Distance") { return JointType::Distance; }
	else if (type == "Prismatic") { return JointType::Prismatic; }
	else if (type == "Wheel") { return JointType::Wheel; }
	else if (type == "Weld") { return JointType::Weld; }
	else if (type == "Pulley") { return JointType::Pulley; }
	else if (type == "Friction") { return JointType::Friction; }
	else if (type == "Gear") { return JointType::Gear; }
	else if (type == "Mouse") { return JointType::Mouse; }
	else if (type == "Rope") { return JointType::Rope; }
	return JointType::None;
}
ObjectFactory::ObjectFactory(ResourceManager * devices):devices(devices)
{
}

Object * ObjectFactory::Create(tinyxml2::XMLElement * objectElement)
{
	Object* newObject = new Object();
	ObjectFactoryPresets presets;
	presets.objectType = objectElement->Attribute("type");
	//MrOrme:: Need to finish initializing components!
	for (
		tinyxml2::XMLElement* componentElement = objectElement->FirstChildElement("Component");
		componentElement;
		componentElement = componentElement->NextSiblingElement("Component")
		)
	{
		string componentName = componentElement->Attribute("name");
		if (componentName == "Sprite")
		{
			newObject->AddComponent(createSpriteComponent(newObject, devices, componentElement));
			
		}
		else if (componentName == "Body")
		{
			newObject->AddComponent(createBodyComponent(newObject,devices, componentElement));
		}
		else if (componentName == "Health")
		{

			newObject->AddComponent(new HealthComponent(newObject, devices, stoi(componentElement->Attribute("health"))));

		}
		else if (componentName == "Input")
		{
			newObject->AddComponent(new UserInputComponent(newObject,devices));

		}
		else if (componentName == "Joint")
		{
			//TODO:: joint code
			for (
				tinyxml2::XMLElement* JointParams = componentElement->FirstChildElement();
				JointParams;
				JointParams = JointParams->NextSiblingElement()
				)
			{
				int jointNumber;
				JointParams->QueryIntAttribute("jointNumber", &jointNumber);
				
				tinyxml2::XMLElement* jointComponent = JointParams->FirstChildElement();
				SpriteComponent* sprite{ createSpriteComponent(newObject, devices, jointComponent) };
				
				jointComponent = jointComponent->NextSiblingElement();
				BodyComponent* body{ createBodyComponent(newObject, devices, jointComponent) };
				
				//First one just gets added. Future ones must follow the chain to the end!
				if (jointNumber == 0)
				{
					newObject->AddComponent(sprite);
					newObject->AddComponent(body);
				}
				else
				{
					//********Create the joint***************
					Joints presets;
					SpriteComponent* currSprite = newObject->getComponent<SpriteComponent>();
					BodyComponent* currBody = newObject->getComponent<BodyComponent>();
					int joinTo;
					JointParams->QueryIntAttribute("joinTo", &joinTo);
					while (currSprite->sprite)
					{
						if (joinTo == 0) presets.BodyB = currBody;
						currSprite = currSprite->sprite.get();
						currBody = currBody->joinedWith.get();
						joinTo--;
					}

					if (!presets.BodyB)
					{
						return nullptr;
					}

					currSprite->sprite = unique_ptr<SpriteComponent>(sprite);
					currBody->joinedWith = unique_ptr<BodyComponent>(body);
										
					presets.BodyA = body;
					presets.type = setJointType(jointComponent->Attribute("type"));
					jointComponent->QueryBoolAttribute("collide", &presets.CollideConnected);
					jointComponent->QueryIntAttribute("anchorXFromCenter", (int*)&presets.AnchorA.x);
					jointComponent->QueryIntAttribute("anchorYFromCenter", (int*)&presets.AnchorA.y);
					jointComponent->QueryIntAttribute("joinToAnchorXFromCenter", (int*)&presets.AnchorB.x);
					jointComponent->QueryIntAttribute("joinToAnchorYFromCenter", (int*)&presets.AnchorB.y);
					jointComponent->QueryIntAttribute("referenceAngle", (int*)&presets.referenceAngle);
					
					//TODO::flush out for other types of joints that need other information!
					switch (presets.type)
					{
					case JointType::Weld:
					default:
						break;
					}

					devices->pDevice->createJoint(presets);
					//***************************************
				}

				
				
			}
		}
		
	}
	return newObject;
}

