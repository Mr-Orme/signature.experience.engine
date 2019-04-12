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
	eFloat torque;
	eFloat maxAngularVelocity;
	eInt radius;
	Vector2D center;
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
	Vector2D AnchorA;
	Vector2D AnchorB;
	eFloat referenceAngle;
	eFloat JointLimit;

};

struct PhysicsStats
{
	eFloat spinSpeed{ 0.0f };
	BodyType bodyType{ BodyType::Static };
	BodyShape bodyShape{BodyShape::Rectangle};
	eFloat density{ 0.0f };
	eFloat friction{ 0.0f };
	eFloat restitution{ 0.0f };
	eFloat angularDamping{ 0.0f };
	eFloat linearDamping{ 0.0f };
	eFloat force{ 0.0f };
	Joints joint;
	bool physicsOn{ false };
};
struct BodyPresets
{
	Vector2D position{ 0,0 };
	eFloat angle{ 0.0f };
	PhysicsStats physics;
	SpriteComponent* sprite{ nullptr };
	std::vector<std::unique_ptr<ICallBack>> callBacks;
};
struct ObjectFactoryPresets
{
	std::string objectType{ "" };
	SpritePresets spriteInitializers;
	BodyPresets bodyInitializers;
	
	eInt health{ 0 };//move to new AssetLibraryGame
	ResourceManager* devices{ nullptr };
};
#endif // !INITIALIZERS_H

