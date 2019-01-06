#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H

#include <memory>
#include "Box2D.h"
#include "Definitions.h"
#include "Initializers.h"

class Texture;
class BodyComponent;

class PhysicsDevice{
public:
	//Tricky trying to get initialize into one of the constructors.
	PhysicsDevice() = delete;
	~PhysicsDevice();
	PhysicsDevice(EngineDefs::Vector gravity);
	
	bool update(float dt);
	bool createFixture(BodyComponent* object, BodyPresets presets);
	bool createJoint(Joints joint);
	bool removeObject(BodyComponent* object);

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

private:

	b2Body * FindBody(const BodyComponent* bodyComponent) const;
	
	inline float PW2RW(EngineDefs::Float x)const { return x * fPRV; }
	inline float RW2PW(EngineDefs::Float x)const { return x / fPRV; }
	inline float RW2PW(EngineDefs::Int x)const { return (float)x / fPRV; }
	inline float RW2PWAngle(EngineDefs::Float x)const { return((float)x*(2.0f*3.14159f) / 360.0f); } //degrees to radians
	inline float PW2RWAngle(EngineDefs::Float x)const { return((float)x*360.0f / (2.0f*3.14159f)); } //radians to degrees
	b2Vec2 GV2PV(EngineDefs::Vector gameVec)const;
	EngineDefs::Vector PV2GV(b2Vec2 physicsVec)const;
	EngineDefs::Vector alignCenters(const  BodyComponent* object)const;

	
	bool destroyJoint(BodyComponent* object);
	bool destroyJoint(b2Body* body);	

	std::unique_ptr<b2World> world{ nullptr };
	const b2Vec2 gravity{ 0,0 };
	static const float fPRV;

	

	

};
#endif