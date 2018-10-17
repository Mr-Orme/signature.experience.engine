#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H

#include "Definitions.h"
#include "Box2D.h"

#include "ObjectFactory.h"

class Texture;
class Object;

class PhysicsDevice{
public:

	PhysicsDevice(Position gravity);
	bool initialize();
	bool update(float dt);

	struct RotateBody
	{
		ENGINE_FLT torque;
		ENGINE_FLT maxAngularVelocity;
		EngineInt radius;
		Position center;
	};

	enum class BodyShape { Rectangle, Circle };
	enum class BodyType { Static, Kinematic, Dynamic };

	struct PhysicsStats
	{
		ENGINE_FLT spinSpeed;
		BodyType bodyType;
		BodyShape bodyShape;
		ENGINE_FLT density;
		ENGINE_FLT friction;
		ENGINE_FLT restitution;
		ENGINE_FLT angularDamping;
		ENGINE_FLT linearDamping;
		ENGINE_FLT force;
		bool physicsOn;
	};


	bool createFixture
		(
			Object* object,
			PhysicsStats physics,
			ObjectFactory::ObjectFactoryPresets presets
		);

	bool setTransform( Object* object, Position position, ENGINE_FLT angle);
	bool setLinearVelocity( Object* object, Position linearVelociy);
	bool setAngularVelocity( Object* object, ENGINE_FLT angularVelocity);
	bool setTorque( Object* object, ENGINE_FLT torque);
	bool setLinearImpulse( Object* object, Position forceVec, Position forceCenter);
	bool setStatic( Object* object);
	bool setStopPhysics( Object* object);
	bool setAngle( Object* object, ENGINE_FLT angle);

	ENGINE_FLT getAngularVelocity( Object* object);
	Position getPosition( Object* object);
	ENGINE_FLT getAngle( Object* object);
	Position getVelocity( Object* object);
	Position getLinearVelocity( Object* object);
	b2World* getWorld(){ return world; }
	
	bool createRevolvingJoint( Object* object1,  Object* object2, Position anchor1, Position anchor2);

	
	b2Body* FindBody( Object* Object);
	b2Vec2 GV2PV(Position gameVec);
	Position PV2GV(b2Vec2 physicsVec);
	bool removeObject( Object* object);
	bool destroyJoint( Object* object);
		
	b2World* world;
	
	inline float PW2RW(ENGINE_FLT x){return x*fPRV;}
	inline float RW2PW(ENGINE_FLT x){return x/fPRV;}
	inline float RW2PW(EngineInt x){return (float)x/fPRV;}
	inline float RW2PWAngle(ENGINE_FLT x){return((float)x*(2.0f*3.14159f)/360.0f);} //degrees to radians
	inline float PW2RWAngle(ENGINE_FLT x){return((float)x*360.0f/(2.0f*3.14159f));} //radians to degrees
private:
	bool destroyJoint(b2Body* body);
	const b2Vec2 gravity;
	Position alignCenters( Object* object);
	static const float fPRV;

	

	

};
#endif