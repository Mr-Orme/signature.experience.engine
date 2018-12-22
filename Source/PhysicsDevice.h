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
	//Tricky trying to get initialize into one of the constructors.
	PhysicsDevice() {}
	~PhysicsDevice() {}
	PhysicsDevice(EngineDefs::Vector gravity);
	bool initialize();
	bool update(float dt);

	bool createFixture(BodyComponent* object, BodyPresets presets);

	bool setTransform( const BodyComponent* object, EngineDefs::Vector position, EngineDefs::Float angle);
	bool setLinearVelocity( const BodyComponent* object, EngineDefs::Vector linearVelociy);
	bool setAngularVelocity( const BodyComponent* object, EngineDefs::Float angularVelocity);
	bool setTorque(const BodyComponent* object, EngineDefs::Float torque);
	bool setLinearImpulse(const BodyComponent* object, EngineDefs::Vector forceVec, EngineDefs::Vector forceCenter);
	bool setStatic(const BodyComponent* object);
	bool setStopPhysics(const BodyComponent* object);
	bool setAngle(const BodyComponent* object, EngineDefs::Float angle);
	bool accelerate(const BodyComponent* object, EngineDefs::Vector forceVec);

	EngineDefs::Float getAngularVelocity( const BodyComponent* object) const;
	EngineDefs::Vector getPosition( const BodyComponent* object) const;
	EngineDefs::Float getAngle( const BodyComponent* object) const;
	EngineDefs::Vector getVelocity( const BodyComponent* object) const;
	EngineDefs::Vector getLinearVelocity( const BodyComponent* object) const;
	b2World* getWorld() const { return world; }
	
	bool createJoint(Joints joint);

	
	b2Body* FindBody( const BodyComponent* bodyComponent) const;
	b2Vec2 GV2PV(EngineDefs::Vector gameVec)const;
	EngineDefs::Vector PV2GV(b2Vec2 physicsVec)const;
	bool removeObject( BodyComponent* object);
	bool destroyJoint( BodyComponent* object);
		
	b2World* world;
	
	inline float PW2RW(EngineDefs::Float x)const {return x*fPRV;}
	inline float RW2PW(EngineDefs::Float x)const {return x/fPRV;}
	inline float RW2PW(EngineDefs::Int x)const {return (float)x/fPRV;}
	inline float RW2PWAngle(EngineDefs::Float x)const {return((float)x*(2.0f*3.14159f)/360.0f);} //degrees to radians
	inline float PW2RWAngle(EngineDefs::Float x)const {return((float)x*360.0f/(2.0f*3.14159f));} //radians to degrees
private:
	bool destroyJoint(b2Body* body);
	const b2Vec2 gravity;
	EngineDefs::Vector alignCenters(const  BodyComponent* object)const;
	static const float fPRV;

	

	

};
#endif