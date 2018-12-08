#include "ContactListener.h"
#include "Object.h"
//#include "ComponentsList.h"

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    //Grab the two Physics Bodies involved in the Contact
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	//Cast them to object pointers
	 Object* objectA = static_cast<Object*>(bodyA -> GetUserData());
	 Object* objectB = static_cast<Object*>(bodyB -> GetUserData());
			

}
