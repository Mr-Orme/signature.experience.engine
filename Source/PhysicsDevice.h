#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H

#include "Definitions.h"
#include "Box2D.h"

#include "ObjectFactory.h"

class Texture;
class Object;

class PhysicsDevice{
public:
	PhysicsDevice() {}
	~PhysicsDevice() {}
	PhysicsDevice(Position gravity);
	bool initialize();
	bool update(float dt);

	struct RotateBody
	{
		EngineFloat torque;
		EngineFloat maxAngularVelocity;
		EngineInt radius;
		Position center;
	};

	enum class BodyShape { Rectangle, Circle };
	enum class BodyType { Static, Kinematic, Dynamic };

	struct PhysicsStats
	{
		EngineFloat spinSpeed;
		BodyType bodyType;
		BodyShape bodyShape;
		EngineFloat density;
		EngineFloat friction;
		EngineFloat restitution;
		EngineFloat angularDamping;
		EngineFloat linearDamping;
		EngineFloat force;
		bool physicsOn;
	};


	bool createFixture
		(
			Object* object,
			PhysicsStats physics,
			ObjectFactory::ObjectFactoryPresets presets
		);

	bool setTransform( Object* object, Position position, EngineFloat angle);
	bool setLinearVelocity( Object* object, Position linearVelociy);
	bool setAngularVelocity( Object* object, EngineFloat angularVelocity);
	bool setTorque( Object* object, EngineFloat torque);
	bool setLinearImpulse( Object* object, Position forceVec, Position forceCenter);
	bool setStatic( Object* object);
	bool setStopPhysics( Object* object);
	bool setAngle( Object* object, EngineFloat angle);

	EngineFloat getAngularVelocity( Object* object);
	Position getPosition( Object* object);
	EngineFloat getAngle( Object* object);
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
	
	inline float PW2RW(EngineFloat x){return x*fPRV;}
	inline float RW2PW(EngineFloat x){return x/fPRV;}
	inline float RW2PW(EngineInt x){return (float)x/fPRV;}
	inline float RW2PWAngle(EngineFloat x){return((float)x*(2.0f*3.14159f)/360.0f);} //degrees to radians
	inline float PW2RWAngle(EngineFloat x){return((float)x*360.0f/(2.0f*3.14159f));} //radians to degrees
private:
	bool destroyJoint(b2Body* body);
	const b2Vec2 gravity;
	Position alignCenters( Object* object);
	static const float fPRV;

	

	

};
#endif