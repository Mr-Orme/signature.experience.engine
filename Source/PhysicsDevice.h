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

	bool setTransform( const Object* object, Position position, EngineFloat angle);
	bool setLinearVelocity( const Object* object, Position linearVelociy);
	bool setAngularVelocity( const Object* object, EngineFloat angularVelocity);
	bool setTorque(const Object* object, EngineFloat torque);
	bool setLinearImpulse(const Object* object, Position forceVec, Position forceCenter);
	bool setStatic(const Object* object);
	bool setStopPhysics(const Object* object);
	bool setAngle(const Object* object, EngineFloat angle);

	EngineFloat getAngularVelocity( const Object* object) const;
	Position getPosition( const Object* object) const;
	EngineFloat getAngle( const Object* object) const;
	Position getVelocity( const Object* object) const;
	Position getLinearVelocity( const Object* object) const;
	b2World* getWorld() const { return world; }
	
	bool createRevolvingJoint( Object* object1,  Object* object2, Position anchor1, Position anchor2);

	
	b2Body* FindBody( const Object* Object) const;
	b2Vec2 GV2PV(Position gameVec)const;
	Position PV2GV(b2Vec2 physicsVec)const;
	bool removeObject( Object* object);
	bool destroyJoint( Object* object);
		
	b2World* world;
	
	inline float PW2RW(EngineFloat x)const {return x*fPRV;}
	inline float RW2PW(EngineFloat x)const {return x/fPRV;}
	inline float RW2PW(EngineInt x)const {return (float)x/fPRV;}
	inline float RW2PWAngle(EngineFloat x)const {return((float)x*(2.0f*3.14159f)/360.0f);} //degrees to radians
	inline float PW2RWAngle(EngineFloat x)const {return((float)x*360.0f/(2.0f*3.14159f));} //radians to degrees
private:
	bool destroyJoint(b2Body* body);
	const b2Vec2 gravity;
	Position alignCenters(const  Object* object)const;
	static const float fPRV;

	

	

};
#endif