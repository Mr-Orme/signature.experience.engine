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

//TODO:: Make this into a conversion constructor!
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
//TODO:: Make this into a conversion constructor!!
BodyComponent* createBodyComponent(Object* newObject, ResourceManager* devices, tinyxml2::XMLElement * componentElement, SpriteComponent* sprite = nullptr)
{
	BodyPresets initializers;
	initializers.sprite = sprite;
	componentElement->QueryFloatAttribute("x", (float*)(&initializers.position.x));
	componentElement->QueryFloatAttribute("y", (float*)(&initializers.position.y));
	componentElement->QueryFloatAttribute("density", (float*)(&initializers.physics.density));
	componentElement->QueryFloatAttribute("restitution", (float*)(&initializers.physics.restitution));
	componentElement->QueryFloatAttribute("angularDamping", (float*)(&initializers.physics.angularDamping));
	componentElement->QueryFloatAttribute("linearDamping", (float*)(&initializers.physics.linearDamping));

	if ((string)componentElement->Attribute("bodyType") == "DYNAMIC")
	{
		initializers.physics.bodyType = BodyType::Dynamic;
	}
	else if ((string)componentElement->Attribute("bodyType") == "STATIC")
	{
		initializers.physics.bodyType = BodyType::Static;
	}

	if ((string)componentElement->Attribute("bodyShape") == "RECTANGLE")
	{
		initializers.physics.bodyShape = BodyShape::Rectangle;
	}
	else if ((string)componentElement->Attribute("bodyShape") == "CIRCLE")
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
	//ToDo:: Need to finish initializing components!
	for (
		tinyxml2::XMLElement* componentElement = objectElement->FirstChildElement();
		componentElement;
		componentElement = componentElement->NextSiblingElement()
		)
	{
		string componentName = componentElement->Attribute("name");
		if (componentName == "Sprite")
		{
			newObject->AddComponent(createSpriteComponent(newObject, devices, componentElement));			
		}
		else if (componentName == "Body")
		{
			newObject->AddComponent(createBodyComponent(newObject,devices, componentElement, newObject->getComponent<SpriteComponent>()));
			if (SpriteComponent* sprite = newObject->getComponent<SpriteComponent>(); sprite)
			{
				sprite->spriteBody = newObject->getComponent<BodyComponent>();
			}
		}
		else if (componentName == "Stat")
		{

			newObject->AddComponent(new StatComponent(newObject, devices, stoi(componentElement->Attribute("health"))));

		}
		else if (componentName == "Input")
		{
			newObject->AddComponent(new UserInputComponent(newObject,devices));

		}
		else if (componentName == "Joint")
		{
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
				BodyComponent* body{ createBodyComponent(newObject, devices, jointComponent, sprite) };
				
				sprite->spriteBody = body;

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
					//TODO::this doesn't work with the head case!
					if (joinTo == 0) presets.BodyB = currBody;
					while (currSprite->sprite)
					{						
						currSprite = currSprite->sprite.get();
						currBody = currBody->joinedWith.get();
						if (joinTo--; joinTo == 0) presets.BodyB = currBody;
					}

					if (!presets.BodyB)
					{
						return nullptr;
					}

					currSprite->sprite = unique_ptr<SpriteComponent>(sprite);
					currBody->joinedWith = unique_ptr<BodyComponent>(body);
										
					presets.BodyA = body;
					presets.type = setJointType((string)componentElement->Attribute("type"));
					componentElement->QueryBoolAttribute("collide", &presets.CollideConnected);
					JointParams->QueryIntAttribute("anchorXFromCenter", (int*)(&presets.AnchorA.x));
					JointParams->QueryIntAttribute("anchorYFromCenter", (int*)(&presets.AnchorA.y));
					JointParams->QueryIntAttribute("joinToAnchorXFromCenter", (int*)(&presets.AnchorB.x));
					JointParams->QueryIntAttribute("joinToAnchorYFromCenter", (int*)(&presets.AnchorB.y));
					JointParams->QueryIntAttribute("referenceAngle", (int*)(&presets.referenceAngle));
					
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

