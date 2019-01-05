#ifndef INITIALIZERS_H
#define INITIALIZERS_H

#include "Definitions.h"
#include "ICallBack.h"
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
	EngineDefs::Float torque;
	EngineDefs::Float maxAngularVelocity;
	EngineDefs::Int radius;
	EngineDefs::Vector center;
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
	EngineDefs::Vector AnchorA;
	EngineDefs::Vector AnchorB;
	EngineDefs::Float referenceAngle;
	EngineDefs::Float JointLimit;

};

struct PhysicsStats
{
	EngineDefs::Float spinSpeed{ 0.0f };
	BodyType bodyType{ BodyType::Static };
	BodyShape bodyShape{BodyShape::Rectangle};
	EngineDefs::Float density{ 0.0f };
	EngineDefs::Float friction{ 0.0f };
	EngineDefs::Float restitution{ 0.0f };
	EngineDefs::Float angularDamping{ 0.0f };
	EngineDefs::Float linearDamping{ 0.0f };
	EngineDefs::Float force{ 0.0f };
	Joints joint;
	bool physicsOn{ false };
};
struct BodyPresets
{
	EngineDefs::Vector position{ 0,0 };
	EngineDefs::Float angle{ 0.0f };
	PhysicsStats physics;
	SpriteComponent* sprite{ nullptr };
	std::vector<std::unique_ptr<ICallBack>> callBacks;
};
struct ObjectFactoryPresets
{
	std::string objectType{ "" };
	SpritePresets spriteInitializers;
	BodyPresets bodyInitializers;
	
	EngineDefs::Int health{ 0 };//move to new AssetLibraryGame
	ResourceManager* devices{ nullptr };
};
#endif // !INITIALIZERS_H

