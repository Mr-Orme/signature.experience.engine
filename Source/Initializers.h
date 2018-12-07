#ifndef INITIALIZERS_H
#define INITIALIZERS_H

#include "Definitions.h"
class ResourceManager;
class BodyComponent;
class SpriteComponent;
class Texture;

struct SpritePresets
{
	std::string textString{ "" };
	std::string vAlign{ "" };
	std::string hAlign{ "" };
	Texture* spriteTexture{ nullptr };
	ResourceManager* Devices{ nullptr };

};
struct RotateBody
{
	EngineFloat torque;
	EngineFloat maxAngularVelocity;
	EngineInt radius;
	Position center;
};

enum class BodyShape { Rectangle, Circle };
enum class BodyType { Static, Kinematic, Dynamic };
enum class JointType{Revolute, Distance, Prismatic, Wheel, Weld, Pulley, Friction, Gear, Mouse, Rope, None};
struct Joints
{
	JointType type;
	BodyComponent* BodyA;
	BodyComponent* BodyB;
	bool CollideConnected;
	Position AnchorA;
	Position AnchorB;
	EngineFloat referenceAngle;
	EngineFloat JointLimit;

};

struct PhysicsStats
{
	EngineFloat spinSpeed{ 0.0f };
	BodyType bodyType{ BodyType::Static };
	BodyShape bodyShape{BodyShape::Rectangle};
	EngineFloat density{ 0.0f };
	EngineFloat friction{ 0.0f };
	EngineFloat restitution{ 0.0f };
	EngineFloat angularDamping{ 0.0f };
	EngineFloat linearDamping{ 0.0f };
	EngineFloat force{ 0.0f };
	Joints joint;
	bool physicsOn{ false };
};
struct BodyPresets
{
	Position position{ 0,0 };
	EngineFloat angle{ 0.0f };
	PhysicsStats physics;
	SpriteComponent* sprite{ nullptr };
};
struct ObjectFactoryPresets
{
	std::string objectType{ "" };
	SpritePresets spriteInitializers;
	BodyPresets bodyInitializers;
	
	//TODO: need to update userInputComponent with this!
	EngineInt health{ 0 };//move to new AssetLibraryGame
	ResourceManager* devices{ nullptr };
};
#endif // !INITIALIZERS_H

