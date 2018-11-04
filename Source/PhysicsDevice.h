#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H

#include "Definitions.h"
#include "Initializers.h"
#include "Box2D.h"

#include "ObjectFactory.h"

class Texture;
class BodyComponent;

class PhysicsDevice{
public:
	PhysicsDevice() {}
	~PhysicsDevice() {}
	PhysicsDevice(Position gravity);
	bool initialize();
	bool update(float dt);

	


	bool createFixture(BodyComponent* object, ObjectFactoryPresets presets);

	bool setTransform( const BodyComponent* object, Position position, EngineFloat angle);
	bool setLinearVelocity( const BodyComponent* object, Position linearVelociy);
	bool setAngularVelocity( const BodyComponent* object, EngineFloat angularVelocity);
	bool setTorque(const BodyComponent* object, EngineFloat torque);
	bool setLinearImpulse(const BodyComponent* object, Position forceVec, Position forceCenter);
	bool setStatic(const BodyComponent* object);
	bool setStopPhysics(const BodyComponent* object);
	bool setAngle(const BodyComponent* object, EngineFloat angle);

	EngineFloat getAngularVelocity( const BodyComponent* object) const;
	Position getPosition( const BodyComponent* object) const;
	EngineFloat getAngle( const BodyComponent* object) const;
	Position getVelocity( const BodyComponent* object) const;
	Position getLinearVelocity( const BodyComponent* object) const;
	b2World* getWorld() const { return world; }
	
	bool createJoint(Joints joint);

	
	b2Body* FindBody( const BodyComponent* bodyComponent) const;
	b2Vec2 GV2PV(Position gameVec)const;
	Position PV2GV(b2Vec2 physicsVec)const;
	bool removeObject( BodyComponent* object);
	bool destroyJoint( BodyComponent* object);
		
	b2World* world;
	
	inline float PW2RW(EngineFloat x)const {return x*fPRV;}
	inline float RW2PW(EngineFloat x)const {return x/fPRV;}
	inline float RW2PW(EngineInt x)const {return (float)x/fPRV;}
	inline float RW2PWAngle(EngineFloat x)const {return((float)x*(2.0f*3.14159f)/360.0f);} //degrees to radians
	inline float PW2RWAngle(EngineFloat x)const {return((float)x*360.0f/(2.0f*3.14159f));} //radians to degrees
private:
	bool destroyJoint(b2Body* body);
	const b2Vec2 gravity;
	Position alignCenters(const  BodyComponent* object)const;
	static const float fPRV;

	

	

};
#endif