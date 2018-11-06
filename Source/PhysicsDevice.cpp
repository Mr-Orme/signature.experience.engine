#include <cmath>

#include "PhysicsDevice.h"
#include "SpriteComponent.h"
#include "ContactListener.h"
#include "BodyComponent.h"
#include "Texture.h"

#include "Object.h"
#include "Initializers.h"


const float PhysicsDevice::fPRV{ 10.0f };

PhysicsDevice::PhysicsDevice(Position gravity):gravity(gravity.x, gravity.y)
{

}
//**************************************
//Creates a new world on initialization
bool PhysicsDevice::initialize()
//**************************************
{
	world = new b2World(gravity);
	if (world == NULL) return false;
	ContactListener* c1 = new ContactListener();
	world -> SetContactListener(c1);
	return true;
}

//**************************************
//Steps the physics world
bool PhysicsDevice::update(float dt)
//**************************************
{	
	world -> Step(dt, 8, 3);
	return true;
}

//**************************************
//Moves body to a set location & angle
bool PhysicsDevice::setTransform(const BodyComponent* object, Position position, EngineFloat angle)
//**************************************
{
	//finds which body this object is attached to.
	//based on value set in UserData field of body
	b2Body* body = FindBody(object);

	//Applies' Box2D transform.
	body -> SetTransform
		(
			GV2PV(position), 
			RW2PWAngle(angle)
		);
	return true;
	
}
bool PhysicsDevice::setAngle(const BodyComponent* object, EngineFloat angle)
{
	b2Body* body = FindBody(object);
	body -> SetTransform(body -> GetPosition(), RW2PWAngle(angle));
	return true;
}

//**************************************
//sets angular velocity
bool PhysicsDevice::setAngularVelocity(const BodyComponent* object, EngineFloat angularVelocity)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> SetAngularVelocity(RW2PW(angularVelocity));
	return true;
	
}

//**************************************
//sets angular velocity
bool PhysicsDevice::setLinearVelocity(const BodyComponent* object, Position linearVelociy)
//**************************************
{
	b2Body* body = FindBody(object);
	b2Vec2 b2LVelocity;
	b2LVelocity.x = RW2PW(linearVelociy.x);
	b2LVelocity.y = RW2PW(linearVelociy.y);
	body -> SetLinearVelocity(b2LVelocity);
	return true;
}

//**************************************
//sets a linear impulse based on passed force vector and center vector
bool PhysicsDevice::setLinearImpulse(const BodyComponent* object, Position forceVec, Position forceCenter)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> ApplyLinearImpulse
			(
			GV2PV(forceVec),
			body -> GetWorldPoint(GV2PV(forceCenter)),
			true
			);
	return true;
}

//**************************************
//sets torque on body based on passed values
bool PhysicsDevice::setTorque(const BodyComponent* object, EngineFloat torque)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> ApplyTorque(RW2PW(torque), true);
	return true;
}

bool PhysicsDevice::setStatic(const BodyComponent* object)
{
	b2Body* body = FindBody(object);
	body -> SetType(b2_staticBody);
	return true;
}

bool PhysicsDevice::setStopPhysics(const BodyComponent* object)
{
	b2Body* body = FindBody(object);
	body -> SetActive(false);
	return true;	
}

//**************************************
//gets Angular velocity of body
EngineFloat PhysicsDevice::getAngularVelocity(const BodyComponent* object) const
//**************************************
{
	b2Body* body = FindBody(object);
	return (PW2RWAngle(body -> GetAngularVelocity()));
}
Position PhysicsDevice::getLinearVelocity(const BodyComponent* object) const
{
	b2Body* body = FindBody(object);
	return(PV2GV(body -> GetLinearVelocity()));
}

//**************************************
//gets bodies' position
Position PhysicsDevice::getPosition(const  BodyComponent* object) const
//**************************************
{
	//b2Body* body = FindBody(object);
	////PV2GV = Physics vector to Game vector (b2vec2 to Position)
	//return (PV2GV(body -> getPosition()));
	return (alignCenters(object));
}

//**************************************
//gets bodies' angle
EngineFloat PhysicsDevice::getAngle( const BodyComponent* object) const
//**************************************
{
	b2Body* body = FindBody(object);
	return (PW2RWAngle(body -> GetAngle()));
}

Position PhysicsDevice::getVelocity(const BodyComponent* object) const
{
	b2Body* body = FindBody(object);
	return (PV2GV(body -> GetLinearVelocity()));
}

//**************************************
//Creates a body and fixture for a passed in object
//based on passed in values
bool PhysicsDevice::createFixture( BodyComponent* object, BodyPresets presets)
//**************************************
{
	Texture* texture{ presets.sprite->texture };
	
	if (!texture)
	{
		texture = object->getOwner()->getComponent<SpriteComponent>()->texture;
	}

	//new body definition
	b2BodyDef* bd = new b2BodyDef;
	//made need one or the other, depending on what was passed.
	b2PolygonShape pShape;
	b2CircleShape cShape;

	//Fixture definition
	b2FixtureDef shapefd;
	
	//set userData field with passed in object pointer.
	bd -> userData = object;
	

	//set body type
	switch (presets.physics.bodyType)
	{
	case BodyType::Static:
		bd -> type = b2_staticBody;
		break;
	case BodyType::Kinematic:
		bd -> type = b2_kinematicBody;
		break;
	case BodyType::Dynamic:
		bd -> type = b2_dynamicBody;
		break;
	}

	//********Adjust postion because SDL is top left, while box2d is center*************
	//subtract off half the width.
	presets.position.x += (texture -> getWidth()/2);
	//subtract off half the height
	presets.position.y += (texture -> getHeight()/2);
	//**********************************************************************************

	// set starting position & angle
	bd -> position.Set(RW2PW(presets.position.x), RW2PW(presets.position.y));
	bd -> angle = RW2PWAngle(presets.angle);

	//add the body to the world
	b2Body* body = world -> CreateBody(bd);

	//set damping values on the body
	body -> SetAngularDamping(presets.physics.angularDamping);
	body -> SetLinearDamping(presets.physics.linearDamping);

	//set fixture's shape
	switch (presets.physics.bodyShape)
	{
	case BodyShape::Rectangle:
		//rectangle's dimensions
		pShape.SetAsBox(RW2PW(texture-> getWidth()/2.0f), RW2PW(texture-> getHeight()/2.0f));
		shapefd.shape = &pShape;
		break;
	case BodyShape::Circle:
		//circle radius based on object's width.
		EngineFloat width = texture-> getWidth()/2.0f;
		EngineFloat height = texture-> getHeight()/2.0f;

		if (width > height)	cShape.m_radius = (RW2PW(width));
		else cShape.m_radius = (RW2PW(height));
		shapefd.shape = &cShape;
		break;
	}

	//set fixture values based on passed in values.
	shapefd.density = presets.physics.density;
	shapefd.friction = presets.physics.friction;
	shapefd.restitution = presets.physics.restitution;

	//create the fixture on the body.
	body -> CreateFixture(&shapefd);
	body -> SetActive(presets.physics.physicsOn);
	
	return true;
	
}

//**************************************
//Removes an object from the World
bool PhysicsDevice::removeObject( BodyComponent* object)
//**************************************
{
	//Find the body based on the passed object
	b2Body* body = FindBody(object);
	destroyJoint(body);
	
	//check to see if we have a body.
	if(body == NULL)
	{return false;}
	else
	{
		//delete the body from the world
		world -> DestroyBody(body);
		return true;
	}

}

//**************************************
//Destroys a  physics joint from outside the physics device
bool PhysicsDevice::destroyJoint( BodyComponent* object)
//**************************************
{
	b2Body* body = FindBody(object);
	
	//destroy joints associated with body.
	return destroyJoint(body);
}

//**************************************
//Destroys the joint from within the device
bool PhysicsDevice::destroyJoint(b2Body* body)
//**************************************
{
	bool jointFound = false;
	//destroy joints associated with body.
	for(b2JointEdge* j = body -> GetJointList(); j != NULL; j = j -> next)
	{
		world -> DestroyJoint(j -> joint);
		jointFound = true;
	}
	return jointFound;
}

//**************************************
//Uses the user data field in the body and compares the memory location
//to the memory location of the passed object pointer.
//Returns the body that has the same memory location in the user data field
b2Body* PhysicsDevice::FindBody( const BodyComponent* object) const
//**************************************
{
	//loop through the bodies
	for (b2Body* body = world -> GetBodyList(); body; body = body -> GetNext())
	{
		//when we have a match, return it.
		if(object == body -> GetUserData())
		{
			return body;
		}
	}
	//if there was no match, return NULL
	return NULL;
}

//**************************************
//Converts passed Position to b2Vec2
b2Vec2 PhysicsDevice::GV2PV(Position gameVec)const
//**************************************
{
	return b2Vec2(RW2PW(gameVec.x), RW2PW(gameVec.y));
}

//**************************************
//converts passed b2Vec2 to Position
Position PhysicsDevice::PV2GV(b2Vec2 physicsVec)const
//**************************************
{
	Position temp;
	temp.x = PW2RW(physicsVec.x);
	temp.y = PW2RW(physicsVec.y);
	return temp;
}

//**************************************
//Creates a revolute joint using the passed objects and anchor points
bool PhysicsDevice::createJoint( Joints joint)
//**************************************
{
	//find corresponding bodies for objects
	b2Body* bodyA = FindBody(joint.BodyA);
	b2Body* bodyB = FindBody(joint.BodyB);
	if(bodyA == NULL || bodyB == NULL)
	{
		return false;
	}
	//TODO::Flush out other joint types!
	switch (joint.type)
	{
	case JointType::Revolute:
		break;
	case JointType::Distance:
		break;
	case JointType::Prismatic:
		break;
	case JointType::Wheel:
		break;
	case JointType::Weld:
	{
		b2WeldJointDef weldJointDef;
		weldJointDef.bodyA = bodyA;
		weldJointDef.bodyB = bodyB;
		weldJointDef.collideConnected = joint.CollideConnected;
		weldJointDef.localAnchorA.Set(RW2PW(joint.AnchorA.x), RW2PW(joint.AnchorA.y));
		weldJointDef.localAnchorB.Set(RW2PW(joint.AnchorB.x), RW2PW(joint.AnchorB.y));
		weldJointDef.referenceAngle = RW2PWAngle(joint.referenceAngle);
		return world->CreateJoint(&weldJointDef);
		break;
		
	}
	case JointType::Pulley:
		break;
	case JointType::Friction:
		break;
	case JointType::Gear:
		break;
	case JointType::Mouse:
		break;
	case JointType::Rope:
		break;
	default:
		break;
	}
	return false;
	
}

//**************************************
//adjusts postion based on the fact that SDL is top left and Box2d uses the center of an object for position.
Position PhysicsDevice::alignCenters(const BodyComponent* object) const
//**************************************
{
	b2Body* body = FindBody(object);
	b2Vec2 physPosition = body -> GetPosition();
	Position position;
	Texture* texture = object->getOwner()->getComponent<SpriteComponent>()->texture;

		//subtract off half the width.
		position.x = PW2RW(physPosition.x) - (texture -> getWidth()/2);
		//subtract off half the height
		position.y = PW2RW(physPosition.y) - (texture -> getHeight()/2);
	

	return (position);
}