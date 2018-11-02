#include <cmath>

#include "PhysicsDevice.h"
//#include "SpriteComponent.h"
#include "ContactListener.h"
#include "Object.h"
#include "Texture.h"
#include "ObjectFactory.h"

#include "SpriteComponent.h"


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
bool PhysicsDevice::setTransform( Object* object, Position position, EngineFloat angle)
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
bool PhysicsDevice::setAngle( Object* object, EngineFloat angle)
{
	b2Body* body = FindBody(object);
	body -> SetTransform(body -> GetPosition(), RW2PWAngle(angle));
	return true;
}

//**************************************
//sets angular velocity
bool PhysicsDevice::setAngularVelocity( Object* object, EngineFloat angularVelocity)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> SetAngularVelocity(RW2PW(angularVelocity));
	return true;
	
}

//**************************************
//sets angular velocity
bool PhysicsDevice::setLinearVelocity( Object* object, Position linearVelociy)
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
bool PhysicsDevice::setLinearImpulse( Object* object, Position forceVec, Position forceCenter)
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
bool PhysicsDevice::setTorque( Object* object, EngineFloat torque)
//**************************************
{
	b2Body* body = FindBody(object);
	body -> ApplyTorque(RW2PW(torque), true);
	return true;
}

bool PhysicsDevice::setStatic( Object* object)
{
	b2Body* body = FindBody(object);
	body -> SetType(b2_staticBody);
	return true;
}

bool PhysicsDevice::setStopPhysics( Object* object)
{
	b2Body* body = FindBody(object);
	body -> SetActive(false);
	return true;	
}

//**************************************
//gets Angular velocity of body
EngineFloat PhysicsDevice::getAngularVelocity( Object* object)
//**************************************
{
	b2Body* body = FindBody(object);
	return (PW2RWAngle(body -> GetAngularVelocity()));
}
Position PhysicsDevice::getLinearVelocity( Object* object)
{
	b2Body* body = FindBody(object);
	return(PV2GV(body -> GetLinearVelocity()));
}

//**************************************
//gets bodies' position
Position PhysicsDevice::getPosition( Object* object)
//**************************************
{
	//b2Body* body = FindBody(object);
	////PV2GV = Physics vector to Game vector (b2vec2 to Position)
	//return (PV2GV(body -> getPosition()));
	return (alignCenters(object));
}

//**************************************
//gets bodies' angle
EngineFloat PhysicsDevice::getAngle( Object* object)
//**************************************
{
	b2Body* body = FindBody(object);
	return (PW2RWAngle(body -> GetAngle()));
}

Position PhysicsDevice::getVelocity( Object* object)
{
	b2Body* body = FindBody(object);
	return (PV2GV(body -> GetLinearVelocity()));
}

//**************************************
//Creates a body and fixture for a passed in object
//based on passed in values
bool PhysicsDevice::createFixture
	(
		 Object* object,
		PhysicsStats physics,
		ObjectFactory::ObjectFactoryPresets presets
	)
//**************************************
{
	SpriteComponent* compRenderer = object -> getComponent<SpriteComponent>();
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
	switch (physics.bodyType)
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
	Texture* texture = object -> getComponent<SpriteComponent>() -> texture;
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
	body -> SetAngularDamping(physics.angularDamping);
	body -> SetLinearDamping(physics.linearDamping);

	//set fixture's shape
	switch (physics.bodyShape)
	{
	case BodyShape::Rectangle:
		//rectangle's dimensions
		pShape.SetAsBox(RW2PW(compRenderer -> texture -> getWidth()/2.0f), RW2PW(compRenderer -> texture -> getHeight()/2.0f));
		shapefd.shape = &pShape;
		break;
	case BodyShape::Circle:
		//circle radius based on object's width.
		EngineFloat width = compRenderer -> texture -> getWidth()/2.0f;
		EngineFloat height = compRenderer -> texture -> getHeight()/2.0f;

		if (width > height)	cShape.m_radius = (RW2PW(width));
		else cShape.m_radius = (RW2PW(height));
		shapefd.shape = &cShape;
		break;
	}

	//set fixture values based on passed in values.
	shapefd.density = physics.density;
	shapefd.friction = physics.friction;
	shapefd.restitution = physics.restitution;

	//create the fixture on the body.
	body -> CreateFixture(&shapefd);
	body -> SetActive(physics.physicsOn);
	
	return true;
	
}

//**************************************
//Removes an object from the World
bool PhysicsDevice::removeObject( Object* object)
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
bool PhysicsDevice::destroyJoint( Object* object)
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
b2Body* PhysicsDevice::FindBody( Object* object)
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
b2Vec2 PhysicsDevice::GV2PV(Position gameVec)
//**************************************
{
	return b2Vec2(RW2PW(gameVec.x), RW2PW(gameVec.y));
}

//**************************************
//converts passed b2Vec2 to Position
Position PhysicsDevice::PV2GV(b2Vec2 physicsVec)
//**************************************
{
	Position temp;
	temp.x = PW2RW(physicsVec.x);
	temp.y = PW2RW(physicsVec.y);
	return temp;
}

//**************************************
//Creates a revolute joint using the passed objects and anchor points
bool PhysicsDevice::createRevolvingJoint( Object* object1,  Object* object2, Position anchor1, Position anchor2)
//**************************************
{
	//find corresponding bodies for objects
	b2Body* bodyA = FindBody(object1);
	b2Body* bodyB = FindBody(object2);
	if(bodyA == NULL || bodyB == NULL)
	{
		return false;
	}

	//joint definition
	b2RevoluteJointDef revoluteJointDef;

	//assign bodies to definition
	revoluteJointDef.bodyA = bodyA;
	revoluteJointDef.bodyB = bodyB;

	//They cannot collide
	revoluteJointDef.collideConnected = false;

	//set anchor points
	revoluteJointDef.localAnchorA.Set(RW2PW(anchor1.x), RW2PW(anchor1.y));
	revoluteJointDef.localAnchorB.Set(RW2PW(anchor2.x), RW2PW(anchor2.y));

	//Joint starts at angle zero
	revoluteJointDef.referenceAngle = 0;

	//Add the joint to the world. If successful will return True, othewise false.
	return (b2RevoluteJoint*)world -> CreateJoint(&revoluteJointDef);
	
}

//**************************************
//adjusts postion based on the fact that SDL is top left and Box2d uses the center of an object for position.
Position PhysicsDevice::alignCenters( Object* object)
//**************************************
{
	b2Body* body = FindBody(object);
	b2Vec2 physPosition = body -> GetPosition();
	Position position;
	Texture* texture = object -> getComponent<SpriteComponent>() -> texture;

		//subtract off half the width.
		position.x = PW2RW(physPosition.x) - (texture -> getWidth()/2);
		//subtract off half the height
		position.y = PW2RW(physPosition.y) - (texture -> getHeight()/2);
	

	return (position);
}