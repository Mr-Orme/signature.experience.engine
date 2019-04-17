#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H

#include <memory>
#include "Box2D.h"
#include "Definitions.h"
#include "Initializers.h"
#include "Vector2D.h"

class Texture;
class BodyComponent;

class PhysicsDevice{
public:
	//Tricky trying to get initialize into one of the constructors.
	PhysicsDevice() = delete;
	~PhysicsDevice();
	PhysicsDevice(Vector2D gravity);
	
	bool update(float dt);
	bool createFixture(BodyComponent* object, const BodyPresets& presets);
	bool createJoint(PrimaryJoint& joint);
	bool removeObject(BodyComponent* object);

	bool setTransform(BodyComponent* object, Vector2D position, eFloat angle);
	bool setLinearVelocity( const BodyComponent* object, Vector2D linearVelociy);
	bool setAngularVelocity( const BodyComponent* object, eFloat angularVelocity);
	bool setTorque(const BodyComponent* object, eFloat torque);
	bool setLinearImpulse(const BodyComponent* object, Vector2D forceVec, Vector2D forceCenter);
	bool setStatic(const BodyComponent* object);
	bool setStopPhysics(const BodyComponent* object);
	bool setAngle(const BodyComponent* object, eFloat angle);
	bool accelerate(const BodyComponent* object, Vector2D forceVec);

	eFloat getAngularVelocity( const BodyComponent* object) const;
	Vector2D getPosition( const BodyComponent* object) const;
	eFloat getAngle( const BodyComponent* object) const;
	Vector2D getVelocity( const BodyComponent* object) const;
	Vector2D getLinearVelocity( const BodyComponent* object) const;	

private:

	b2Body * FindBody(const BodyComponent* bodyComponent) const;
	
	inline float PW2RW(eFloat x)const { return x * fPRV; }
	inline float RW2PW(eFloat x)const { return x / fPRV; }
	inline float RW2PW(eInt x)const { return (float)x / fPRV; }
	inline b2Vec2 RW2PW(Vector2D vec)const { return { vec.x / fPRV, vec.y / fPRV }; }
	inline float RW2PWAngle(eFloat x)const { return((float)x*(2.0f*3.14159f) / 360.0f); } //degrees to radians
	inline float PW2RWAngle(eFloat x)const { return((float)x*360.0f / (2.0f*3.14159f)); } //radians to degrees
	b2Vec2 GV2PV(Vector2D gameVec)const;
	Vector2D PV2GV(b2Vec2 physicsVec)const;
	Vector2D alignCenters(const  BodyComponent* object)const;
	

	
	bool destroyJoint(BodyComponent* object);
	bool destroyJoint(b2Body* body);	

	std::unique_ptr<b2World> world{ nullptr };
	const b2Vec2 gravity{ 0,0 };
	static const float fPRV;

	

	

};
#endif