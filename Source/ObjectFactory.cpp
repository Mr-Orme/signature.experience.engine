#include "ObjectFactory.h"
#include "ComponentsList.h"
#include "ViewCallBack.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Object.h"
#include "AssetLibrary.h"
#include "Initializers.h"
#include "Listner.h"
#include "PhysicsDevice.h"
#include "ObjectCreationCallBack.h"


using namespace std;

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

Object * ObjectFactory::create(tinyxml2::XMLElement * objectElement) 
{
	
	std::unique_ptr<ObjectFactoryPresets> presets = std::move(createPresetsFromXML(objectElement));
	Object* newObject = create(presets.get());
	return newObject;
}
Object * ObjectFactory::create(ObjectFactoryPresets* presets) 
{
	Object* newObject = new Object;
	if (presets->spriteInitializers.createSprite)
	{
		newObject->AddComponent(new SpriteComponent(newObject, presets->spriteInitializers));
		presets->bodyInitializers.sprite = newObject->getComponent<SpriteComponent>();
	}
	if (presets->bodyInitializers.createBody)
	{
		newObject->AddComponent(new BodyComponent(newObject, devices, presets->bodyInitializers));
		newObject->getComponent<SpriteComponent>()->spriteBody = newObject->getComponent<BodyComponent>();
	}
	if (presets->userInputInitializers.createUserInput)
	{
		newObject->AddComponent(new UserInputTriggerComponent(newObject, devices, presets->userInputInitializers.TriggeredInput, (EventHandler::Event)presets->userInputInitializers.triggeringEvent));
		
	}
	if (presets->steeringInitializers.createSteering)
	{
		newObject->AddComponent(new SteeringBehaviorComponent(newObject, presets->steeringInitializers));
	}
	if (presets->jointInitializers.createJoint)
	{
		//Create primary joint components & add to presets
		newObject->AddComponent(new SpriteComponent(newObject, presets->jointInitializers.spriteInitializers));
		presets->jointInitializers.bodyInitializers.sprite = newObject->getComponent<SpriteComponent>();
		newObject->AddComponent(new BodyComponent(newObject, devices, presets->jointInitializers.bodyInitializers));
		newObject->getComponent<SpriteComponent>()->spriteBody = newObject->getComponent<BodyComponent>();
		presets->jointInitializers.BodyA = newObject->getComponent<BodyComponent>();		

		//create secondary joint components & add to presets.
		presets->jointInitializers.bodyInitializers.sprite->sprite = std::make_unique<SpriteComponent>(newObject, presets->jointInitializers.joinedTo.spriteInitializers);
		presets->jointInitializers.joinedTo.bodyInitializers.sprite = presets->jointInitializers.bodyInitializers.sprite->sprite.get();
		presets->jointInitializers.BodyA->joinedWith = std::make_unique<BodyComponent>(newObject, devices, presets->jointInitializers.joinedTo.bodyInitializers);
		presets->jointInitializers.bodyInitializers.sprite->sprite->spriteBody = presets->jointInitializers.BodyA->joinedWith.get();
		presets->jointInitializers.joinedTo.BodyB = presets->jointInitializers.BodyA->joinedWith.get();
		
		devices->pDevice->createJoint(presets->jointInitializers);

	}

	return newObject;
}
std::unique_ptr<ObjectFactoryPresets> ObjectFactory::createPresetsFromXML(tinyxml2::XMLElement * objectElement)
{
	std::unique_ptr<ObjectFactoryPresets> presets = std::make_unique<ObjectFactoryPresets>();
	presets->objectType = objectElement->Attribute("type");
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
			addSpritePresets(&presets->spriteInitializers, componentElement);
		}
		else if (componentName == "Body")
		{
			addBodyPresets(&presets->bodyInitializers, componentElement);
		}
		/*	else if (componentName == "Stat")
			{

				newObject->AddComponent(new StatComponent(newObject, devices, stoi(componentElement->Attribute("health"))));

			}*/
		else if (componentName == "UserInput")
		{

			addUserInputPresets(&presets->userInputInitializers, componentElement);

		}
		//else if (componentName == "Notification")
		//{
		//	newObject->AddComponent(new NotificationCallBackComponent(newObject, devices, componentElement));
		//	/*Todo:: This needs a body component, just like a sprite.
		//	It's body component determines where the notification is displayed
		//	need a triggerComponent. When collision detection detects collision with a trigger component, the event is sent
		//	to the eventHandler.
		//	class TriggerComponent
		//	{
		//	public:
		//		EventHandler::Event
		//	};
		//	BUT HOW DO I KNOW WHICH NOTIFICATION??? Do I need to store the trigger in the event?
		//	Pass trigger with call to event handler? So, yes, must store trigger in notification! If it is multi-inherited
		//	with a component, then more information can be sent... perhaps also send the object collided with?
		//	Do I need to create a struct of all possible event information to pass around???
		//	This should take care of all collision events, maybe even input events??? death events???
		//	struct EventInfo
		//	{
		//		Object* primaryObject;
		//		Object* secondaryObject;
		//	};
		//	Therefore, notifications need a trigger object created at the same time!. Can trigger multiple notifications this way
		//	Even if most notifications are one off!
		//	Trigger can handle all events where there is a collision
		//		Trigger, can handle a sound event, collision detector passes in two objects colliding.. Primary is player (has userInputComponent).
		//			each callBack can decide to play or not.
		//		Trigger can handle a item pick up
		//		Trigger can handle battle damage
		//	can trigger be used with keypresses???? Combined with UserInputTriggerComponent?????
		//	*/
		//	devices->eventHandler->getListner(EventHandler::Event::Notification)->
		//		addCallBack(newObject->getComponent<NotificationCallBackComponent>());
		//}
		else if (componentName == "Joint")
		{
			addJointPresets(&presets->jointInitializers, componentElement);
		}
		else if (componentName == "Steering")
		{
			addSteeringPresets(&presets->steeringInitializers, componentElement);
		}
	}
	return presets;
}
void ObjectFactory::addSpritePresets(SpritePresets* presets, tinyxml2::XMLElement * componentElement)
{
	presets->createSprite = true;
	presets->Devices = devices;
	bool isSprite{ false };
	componentElement->QueryBoolAttribute("sprite", &isSprite);
	if (isSprite)
	{
		presets->spriteTexture= devices->assetLibrary->getArtAsset(componentElement->Attribute("asset"));
	}
	else
	{
		presets->spriteTexture = std::make_shared<Texture>(devices->gDevice.get(), componentElement->Attribute("text"), false);
	}

}
void ObjectFactory::addBodyPresets(BodyPresets* presets, tinyxml2::XMLElement * componentElement)
{
	presets->createBody = true;
	componentElement->QueryFloatAttribute("x", (float*)(&presets->position.x));
	componentElement->QueryFloatAttribute("y", (float*)(&presets->position.y));
	componentElement->QueryFloatAttribute("density", (float*)(&presets->physics.density));
	componentElement->QueryFloatAttribute("restitution", (float*)(&presets->physics.restitution));
	componentElement->QueryFloatAttribute("angularDamping", (float*)(&presets->physics.angularDamping));
	componentElement->QueryFloatAttribute("linearDamping", (float*)(&presets->physics.linearDamping));

	if ((string)componentElement->Attribute("bodyType") == "DYNAMIC")
	{
		presets->physics.bodyType = BodyType::Dynamic;
	}
	else if ((string)componentElement->Attribute("bodyType") == "STATIC")
	{
		presets->physics.bodyType = BodyType::Static;
	}

	if ((string)componentElement->Attribute("bodyShape") == "RECTANGLE")
	{
		presets->physics.bodyShape = BodyShape::Rectangle;
	}
	else if ((string)componentElement->Attribute("bodyShape") == "CIRCLE")
	{
		presets->physics.bodyShape = BodyShape::Circle;
	}

	componentElement->QueryBoolAttribute("physicsOn", &presets->physics.physicsOn);

}
void ObjectFactory::addUserInputPresets(UserInputPresets* presets, tinyxml2::XMLElement * componentElement)
{
	presets->createUserInput = true;
	int temp;
	//1. Get trigger from XML
	componentElement->QueryIntAttribute("trigger", &temp);
	presets->TriggeredInput = (InputDevice::UserInputs)temp;
		
	//2. Create callback based on event
	componentElement->QueryIntAttribute("event", &presets->triggeringEvent);
	switch (EventHandler::Event(presets->triggeringEvent))
	{
	case EventHandler::Event::CreateObject:
	{	
		/*
		call backs:
		user input for keyboard, adjusts view
		mouse clicks need to vary and be able to set....
		*/
		devices->eventHandler->getListner(EventHandler::Event::CreateObject)->
			addCallBack(new ObjectCreationCallBack(this, createPresetsFromXML(componentElement->FirstChildElement())));
		break;
	}
	case EventHandler::Event::UserInput:

	default:
		break;
	}

	/* 4. Add to event handler */
}
void ObjectFactory::addJointPresets(PrimaryJoint* presets, tinyxml2::XMLElement * componentElement)
{
	//int jointNumber = 0;
	//*************Primary Joint******************
	presets->createJoint = true;

	tinyxml2::XMLElement* JointParams = componentElement->FirstChildElement();
	presets->type = setJointType((string)JointParams->Attribute("type"));
	JointParams->QueryBoolAttribute("collide", &presets->CollideConnected);
	
	tinyxml2::XMLElement* jointComponent = JointParams->FirstChildElement();
	addSpritePresets(&presets->spriteInitializers, jointComponent);

	jointComponent = jointComponent->NextSiblingElement();
	addBodyPresets(&presets->bodyInitializers, jointComponent);
	
	//*************Secondary Joint******************		
	JointParams = JointParams->NextSiblingElement();
	JointParams->QueryIntAttribute("anchorXFromCenter", (int*)(&presets->joinedTo.AnchorA.x));
	JointParams->QueryIntAttribute("anchorYFromCenter", (int*)(&presets->joinedTo.AnchorA.y));
	JointParams->QueryIntAttribute("joinToAnchorXFromCenter", (int*)(&presets->joinedTo.AnchorB.x));
	JointParams->QueryIntAttribute("joinToAnchorYFromCenter", (int*)(&presets->joinedTo.AnchorB.y));
	JointParams->QueryIntAttribute("referenceAngle", (int*)(&presets->joinedTo.referenceAngle));

	jointComponent = JointParams->FirstChildElement();
	addSpritePresets(&presets->joinedTo.spriteInitializers, jointComponent);
	jointComponent = jointComponent->NextSiblingElement();
	addBodyPresets(&presets->joinedTo.bodyInitializers, jointComponent);		
		
		//First one just gets added. Future ones must follow the chain to the end!
		//if (jointNumber == 0)
		//{
		//	addJointPresets
		///*	newObject->AddComponent(sprite);
		//	newObject->AddComponent(body);*/
		//}
		//else
		//{
			//********Create the joint***************
			//Joints presets;
			//SpriteComponent* currSprite = newObject->getComponent<SpriteComponent>();
			//BodyComponent* currBody = newObject->getComponent<BodyComponent>();
			//int joinTo;
			//JointParams->QueryIntAttribute("joinTo", &joinTo);
			////TODO::this doesn't work with the head case!
			//if (joinTo == 0) presets.BodyB = currBody;
			//while (currSprite->sprite)
			//{
			//	currSprite = currSprite->sprite.get();
			//	currBody = currBody->joinedWith.get();
			//	if (joinTo--; joinTo == 0) presets.BodyB = currBody;
			//}

			//if (!presets.BodyB)
			//{
			//	return nullptr;
			//}

			//currSprite->sprite = unique_ptr<SpriteComponent>(sprite);
			//currBody->joinedWith = unique_ptr<BodyComponent>(body);

			//presets.BodyA = body;
			//presets.type = setJointType((string)componentElement->Attribute("type"));
			//componentElement->QueryBoolAttribute("collide", &presets.CollideConnected);
			//JointParams->QueryIntAttribute("anchorXFromCenter", (int*)(&presets.AnchorA.x));
			//JointParams->QueryIntAttribute("anchorYFromCenter", (int*)(&presets.AnchorA.y));
			//JointParams->QueryIntAttribute("joinToAnchorXFromCenter", (int*)(&presets.AnchorB.x));
			//JointParams->QueryIntAttribute("joinToAnchorYFromCenter", (int*)(&presets.AnchorB.y));
			//JointParams->QueryIntAttribute("referenceAngle", (int*)(&presets.referenceAngle));

			////TODO::flush out for other types of joints that need other information!
			//switch (presets.type)
			//{
			//case JointType::Weld:
			//default:
			//	break;
			//}

			//devices->pDevice->createJoint(presets);
			//***************************************
		//}
	

}
void ObjectFactory::addSteeringPresets(SteeringPresets* presets, tinyxml2::XMLElement * componentElement)
{
	presets->devices = devices;
	
	for (
		tinyxml2::XMLElement* behaviorElement = componentElement->FirstChildElement();
		behaviorElement;
		behaviorElement = behaviorElement->NextSiblingElement()
		)
	{
		presets->createSteering = true;
		std::string behavior = behaviorElement->Attribute("name");
		if (behavior == "Seek")
		{
			presets->seek = true;
			int temp;
			behaviorElement->QueryIntAttribute("target", &temp);
			presets->type = SteeringPresets::TargetType(temp);
			if (presets->type == SteeringPresets::TargetType::setVector)
			{
				behaviorElement->QueryFloatAttribute("targetX", &presets->staticTargetVector.x);
				behaviorElement->QueryFloatAttribute("targetY", &presets->staticTargetVector.y);
			}
		}
		else if (behavior == "Arrive")
		{
			presets->arrive = true;
		}
	}

	
}




