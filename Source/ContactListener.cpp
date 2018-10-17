#include <string>
#include "ContactListener.h"
#include "Object.h"
//#include "ComponentsList.h"

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    //Grab the two Physics Bodies involved in the Contact
	b2Body* bodyA = contact->getFixtureA()->getBody();
	b2Body* bodyB = contact->getFixtureB()->getBody();
	//Cast them to object pointers
	 Object* objectA = static_cast<Object*>(bodyA -> getUserData());
	 Object* objectB = static_cast<Object*>(bodyB -> getUserData());

	//find their types
	std::string objectAType = objectA -> getObjectType();
	std::string objectBType = objectB -> getObjectType();
			

}
