#ifndef INITIALIZERS_H
#define INITIALIZERS_H
#include <memory>
#include "Definitions.h"
#include "Vector2D.h"
#include "InputDevice.h"

class ICallBack;
class BodyCallBack;
class ResourceManager;
class BodyComponent;
class SpriteComponent;
class Texture;


struct SpritePresets
{
	bool createSprite{ false };
	//bool isSprite{ false };
	//std::string assetOrText;
	//std::string textString;
	//std::string vAlign;
	//std::string hAlign;
	std::shared_ptr<Texture> spriteTexture{ nullptr };
	ResourceManager* Devices{ nullptr };

};
struct RotateBody
{
	eFloat torque{ 0.0f };
	eFloat maxAngularVelocity{ 0.0f };
	eInt radius{ 0};
	Vector2D center{ 0.0f, 0.0f };
};

enum class BodyShape { Rectangle, Circle };
enum class BodyType { Static, Kinematic, Dynamic };
enum class JointType{Revolute, Distance, Prismatic, Wheel, Weld, Pulley, Friction, Gear, Mouse, Rope, None};
struct PhysicsStats
{
	eFloat spinSpeed{ 0.0f };
	BodyType bodyType{ BodyType::Static };
	BodyShape bodyShape{ BodyShape::Rectangle };
	eFloat density{ 0.0f };
	eFloat friction{ 0.0f };
	eFloat restitution{ 0.0f };
	eFloat angularDamping{ 0.0f };
	eFloat linearDamping{ 0.0f };
	eFloat force{ 0.0f };

	bool physicsOn{ false };
};
struct BodyPresets
{
	BodyPresets();
	~BodyPresets();
	bool createBody{ false };
	Vector2D position{ 0,0 };
	eFloat angle{ 0.0f };
	PhysicsStats physics;
	SpriteComponent* sprite{ nullptr };
	std::vector<std::unique_ptr<BodyCallBack>> callBacks;
};

struct SecondayJoint
{
	BodyPresets bodyInitializers;
	SpritePresets spriteInitializers;
	
	
	BodyComponent* BodyB{ nullptr };
	
	
	
	Vector2D AnchorA{ 0.0f, 0.0f };
	Vector2D AnchorB{ 0.0f, 0.0f };
	eFloat referenceAngle{ 0.0f };
	eFloat JointLimit{ 0.0f };
	std::unique_ptr<SecondayJoint> jointPresets{ nullptr };

};
struct PrimaryJoint
{
	bool createJoint{ false };
	JointType type{ JointType::None };
	bool CollideConnected{ false };
	BodyComponent* BodyA{ nullptr };
	BodyPresets bodyInitializers;
	SpritePresets spriteInitializers;
	SecondayJoint joinedTo;
};


struct UserInputPresets
{
	bool createUserInput{ false };
	InputDevice::UserInputs TriggeredInput;
	int triggeringEvent;
	//ICallBack* callback;

};
struct SteeringPresets
{
	enum TargetType{setVector, mouse};
	bool createSteering{ false };
	bool seek{ false };
	bool arrive{ false };
	TargetType type;
	Vector2D staticTargetVector;
	ResourceManager* devices;
};
struct ObjectFactoryPresets
{
	std::string objectType{ "" };
	SpritePresets spriteInitializers;
	BodyPresets bodyInitializers;
	UserInputPresets userInputInitializers;
	SteeringPresets steeringInitializers;
	PrimaryJoint jointInitializers;
	
	//eInt health{ 0 };//move to new AssetLibraryGame
	ResourceManager* devices{ nullptr };
};
#endif // !INITIALIZERS_H

