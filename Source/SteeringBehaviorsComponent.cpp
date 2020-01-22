#include "SteeringBehaviorsComponent.h"
#include "ResourceManager.h"
#include "ComponentsList.h"
#include "Component.h"
#include "Utils.h"
#include "Object.h"
#include "Timer.h"


#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;
SteeringBehaviorComponent::SteeringBehaviorComponent(Object* owner, SteeringPresets& presets) :Component(owner)
{
	devices = presets.devices;
	if (presets.seek) SeekOn();
	if (presets.arrive) ArriveOn();
	if (presets.flee) FleeOn();
	targetType = presets.type;
	if (targetType == SteeringPresets::TargetType::setVector)
	{
		m_vTarget = presets.staticTargetVector;
	}
	else
	{
		m_vTarget = devices->iDevice->getMousePosition();
	}
}

Vector2D SteeringBehaviorComponent::Calculate()
{
	Vector2D force{ 0,0 };
	//std::cout << force.x << " " << force.y << " --> ";
	if (isSeekOn())  force += Seek(m_vTarget);
	//std::cout << force.x << " " << force.y << " --> ";
	if (isArriveOn()) force += Arrive(m_vTarget, slow);
	//std::cout << force.x << " " << force.y << std::endl;
	if (isFleeOn()) force += Flee(m_vTarget);
	return force;

	//Arrive(m_vTarget, SteeringBehavior::Deceleration::normal);
}

std::vector<std::unique_ptr<Object>> SteeringBehaviorComponent::update()
{
	//cout << owner->getComponent<BodyComponent>()->getPosition()<< endl;
	if (targetType == SteeringPresets::TargetType::mouse)
	{
		m_vTarget = devices->iDevice->getMousePosition();
	}
	owner->getComponent<BodyComponent>()->setPosition(owner->getComponent<BodyComponent>()->getPosition()+ Calculate());
	//owner->getComponent<BodyComponent>()->applyForce(owner->getComponent<BodyComponent>()->getPosition() + 100*Calculate());
	//cout << owner->getComponent<BodyComponent>()->getPosition()  << endl;
	return std::vector<std::unique_ptr<Object>>();
}

/////////////////////////////////////////////////////////////////////////////// START OF BEHAVIORS

//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vector2D SteeringBehaviorComponent::Seek(Vector2D TargetPos)
{  //get sprite position
	Vector2D DesiredVelocity = Vec2DNormalize(
		(TargetPos - owner->getComponent<BodyComponent>()->getPosition())
		* owner->getComponent<BodyComponent>()->maxSpeed);

	return (DesiredVelocity - owner->getComponent<BodyComponent>()->getVelocity());
}

//----------------------------- Flee -------------------------------------
//
//  Does the opposite of Seek
//------------------------------------------------------------------------
Vector2D SteeringBehaviorComponent::Flee(Vector2D TargetPos)
{
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
	/*const eFloat PanicDistanceSq = 100.0f * 100.0;

	if (Vec2DDistanceSq(owner->getComponent<BodyComponent>()->getPosition(), TargetPos) > PanicDistanceSq)
	{
	  return Vector2D(0,0);
	}


	Vector2D DesiredVelocity = Vec2DNormalize(TargetPos - owner->getComponent<BodyComponent>()->getPosition()
		* owner->getComponent<BodyComponent>()->maxSpeed);

	return (DesiredVelocity - owner->getComponent<BodyComponent>()->getVelocity());*/
	return (Seek(TargetPos) * (-1));
}

//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
Vector2D SteeringBehaviorComponent::Arrive(Vector2D     TargetPos,
	Deceleration deceleration)
{
	Vector2D ToTarget = TargetPos - owner->getComponent<BodyComponent>()->getPosition();

	//calculate the distance to the target
	eFloat dist = ToTarget.Length();

	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const eFloat DecelerationTweaker = 0.5;

		//calculate the speed required to reach the target given the desired
		//deceleration
		eFloat speed = dist / ((eFloat)deceleration * DecelerationTweaker);

		//make sure the velocity does not exceed the max
		speed = min((eFloat)speed, owner->getComponent<BodyComponent>()->maxSpeed);

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vector2D DesiredVelocity = ToTarget * speed / dist;

		return (DesiredVelocity - owner->getComponent<BodyComponent>()->getVelocity());
	}

	return Vector2D(0, 0);
}

/*//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the
//  evader
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Pursuit(const GameObject* evader)
{
	//if the evader is ahead and facing the agent then we can just seek
	//for the evader's current position.
	Vector2D ToEvader = evader->getSpritePosition() - m_pVehicle->getSpritePosition();

	eFloat RelativeHeading = m_pVehicle->getHeading().Dot(evader->getHeading());

	if ((ToEvader.Dot(m_pVehicle->getHeading()) > 0) &&
		(RelativeHeading < -0.95))  //acos(0.95)=18 degs
	{
		return Seek(evader->getSpritePosition());
	}

	//Not considered ahead so we predict where the evader will be.

	//the lookahead time is propotional to the distance between the evader
	//and the pursuer; and is inversely proportional to the sum of the
	//agent's velocities
	eFloat LookAheadTime = ToEvader.Length() /
		(m_pVehicle->getMaxSpeed + evader->getSpeed());

	//now seek to the predicted future position of the evader
	return Seek(evader->getSpritePosition() + evader->getVelocity() * LookAheadTime);
}


//----------------------------- Evade ------------------------------------
//
//  similar to pursuit except the agent Flees from the estimated future
//  position of the pursuer
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Evade(const GameObject* pursuer)
{
	/* Not necessary to include the check for facing direction this time */

	/*	Vector2D ToPursuer = pursuer->getSpritePosition() - m_pVehicle->getSpritePosition();

		//uncomment the following two lines to have Evade only consider pursuers
		//within a 'threat range'
		const eFloat ThreatRange = 100.0;
		if (ToPursuer.LengthSq() > ThreatRange * ThreatRange) return Vector2D();

		//the lookahead time is propotional to the distance between the pursuer
		//and the pursuer; and is inversely proportional to the sum of the
		//agents' velocities
		eFloat LookAheadTime = ToPursuer.Length() /
			(m_pVehicle->getMaxSpeed() + pursuer->getSpeed());

		//now flee away from predicted future position of the pursuer
		return Flee(pursuer->getSpritePosition() + pursuer->getVelocity() * LookAheadTime);
	}


	//--------------------------- Wander -------------------------------------
	//
	//  This behavior makes the agent wander about randomly
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::Wander()
	{
		//this behavior is dependent on the update rate, so this line must
		//be included when using time independent framerate.
		eFloat JitterThisTimeSlice = m_dWanderJitter * 1/100;

		//first, add a small random vector to the target's position
		m_vWanderTarget += Vector2D(RandomClamped() * JitterThisTimeSlice,
			RandomClamped() * JitterThisTimeSlice);

		//reproject this new vector back on to a unit circle
		m_vWanderTarget.Normalize();

		//increase the length of the vector to the same as the radius
		//of the wander circle
		m_vWanderTarget *= m_dWanderRadius;

		//move the target into a position WanderDist in front of the agent
		Vector2D target = m_vWanderTarget + Vector2D(m_dWanderDistance, 0);

		//project the target into world space
		Vector2D Target = PointToWorldSpace(target,
			m_pVehicle->getHeading(),
			m_pVehicle->Side(),
			m_pVehicle->getSpritePosition());

		//and steer towards it
		return Target - m_pVehicle->getSpritePosition();
	}


	//---------------------- ObstacleAvoidance -------------------------------
	//
	//  Given a vector of CObstacles, this method returns a steering force
	//  that will prevent the agent colliding with the closest obstacle
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles)
	{
		//the detection box length is proportional to the agent's velocity
		m_dDBoxLength = Prm.MinDetectionBoxLength +
			(m_pVehicle->getSpeed() / m_pVehicle->getMaxSpeed()) *
			Prm.MinDetectionBoxLength;

		//tag all obstacles within range of the box for processing
		m_pVehicle->World()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);

		//this will keep track of the closest intersecting obstacle (CIB)
		BaseGameEntity* ClosestIntersectingObstacle = NULL;

		//this will be used to track the distance to the CIB
		eFloat DistToClosestIP = MaxDouble;

		//this will record the transformed local coordinates of the CIB
		Vector2D LocalPosOfClosestObstacle;

		std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();

		while (curOb != obstacles.end())
		{
			//if the obstacle has been tagged within range proceed
			if ((*curOb)->IsTagged())
			{
				//calculate this obstacle's position in local space
				Vector2D LocalPos = PointToLocalSpace((*curOb)->getSpritePosition(),
					m_pVehicle->getHeading(),
					m_pVehicle->Side(),
					m_pVehicle->getSpritePosition());

				//if the local position has a negative x value then it must lay
				//behind the agent. (in which case it can be ignored)
				if (LocalPos.x >= 0)
				{
					//if the distance from the x axis to the object's position is less
					//than its radius + half the width of the detection box then there
					//is a potential intersection.
					eFloat ExpandedRadius = (*curOb)->BRadius() + m_pVehicle->BRadius();

					if (fabs(LocalPos.y) < ExpandedRadius)
					{
						//now to do a line/circle intersection test. The center of the
						//circle is represented by (cX, cY). The intersection points are
						//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0.
						//We only need to look at the smallest positive value of x because
						//that will be the closest point of intersection.
						eFloat cX = LocalPos.x;
						eFloat cY = LocalPos.y;

						//we only need to calculate the sqrt part of the above equation once
						eFloat SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY * cY);

						eFloat ip = cX - SqrtPart;

						if (ip <= 0.0)
						{
							ip = cX + SqrtPart;
						}

						//test to see if this is the closest so far. If it is keep a
						//record of the obstacle and its local coordinates
						if (ip < DistToClosestIP)
						{
							DistToClosestIP = ip;

							ClosestIntersectingObstacle = *curOb;

							LocalPosOfClosestObstacle = LocalPos;
						}
					}
				}
			}

			++curOb;
		}

		//if we have found an intersecting obstacle, calculate a steering
		//force away from it
		Vector2D SteeringForce;

		if (ClosestIntersectingObstacle)
		{
			//the closer the agent is to an object, the stronger the
			//steering force should be
			eFloat multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.x) /
				m_dDBoxLength;

			//calculate the lateral force
			SteeringForce.y = (ClosestIntersectingObstacle->BRadius() -
				LocalPosOfClosestObstacle.y)  * multiplier;

			//apply a braking force proportional to the obstacles distance from
			//the vehicle.
			const eFloat BrakingWeight = 0.2;

			SteeringForce.x = (ClosestIntersectingObstacle->BRadius() -
				LocalPosOfClosestObstacle.x) *
				BrakingWeight;
		}

		//finally, convert the steering vector from local to world space
		return VectorToWorldSpace(SteeringForce,
			m_pVehicle->getHeading(),
			m_pVehicle->Side());
	}


	//--------------------------- WallAvoidance --------------------------------
	//
	//  This returns a steering force that will keep the agent away from any
	//  walls it may encounter
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::WallAvoidance(const std::vector<Wall2D>& walls)
	{
		//the feelers are contained in a std::vector, m_Feelers
		CreateFeelers();

		eFloat DistToThisIP = 0.0;
		eFloat DistToClosestIP = MaxDouble;

		//this will hold an index into the vector of walls
		int ClosestWall = -1;

		Vector2D SteeringForce,
			point,         //used for storing temporary info
			ClosestPoint;  //holds the closest intersection point

	//examine each feeler in turn
		for (unsigned int flr = 0; flr < m_Feelers.size(); ++flr)
		{
			//run through each wall checking for any intersection points
			for (unsigned int w = 0; w < walls.size(); ++w)
			{
				if (LineIntersection2D(m_pVehicle->getSpritePosition(),
					m_Feelers[flr],
					walls[w].From(),
					walls[w].To(),
					DistToThisIP,
					point))
				{
					//is this the closest found so far? If so keep a record
					if (DistToThisIP < DistToClosestIP)
					{
						DistToClosestIP = DistToThisIP;

						ClosestWall = w;

						ClosestPoint = point;
					}
				}
			}//next wall


			//if an intersection point has been detected, calculate a force
			//that will direct the agent away
			if (ClosestWall >= 0)
			{
				//calculate by what distance the projected position of the agent
				//will overshoot the wall
				Vector2D OverShoot = m_Feelers[flr] - ClosestPoint;

				//create a force in the direction of the wall normal, with a
				//magnitude of the overshoot
				SteeringForce = walls[ClosestWall].Normal() * OverShoot.Length();
			}

		}//next feeler

		return SteeringForce;
	}

	//------------------------------- CreateFeelers --------------------------
	//
	//  Creates the antenna utilized by WallAvoidance
	//------------------------------------------------------------------------
	void SteeringBehavior::CreateFeelers()
	{
		//feeler pointing straight in front
		m_Feelers[0] = m_pVehicle->getSpritePosition() + m_dWallDetectionFeelerLength * m_pVehicle->getHeading();

		//feeler to left
		Vector2D temp = m_pVehicle->getHeading();
		Vec2DRotateAroundOrigin(temp, HalfPi * 3.5f);
		m_Feelers[1] = m_pVehicle->getSpritePosition() + m_dWallDetectionFeelerLength / 2.0f * temp;

		//feeler to right
		temp = m_pVehicle->getHeading();
		Vec2DRotateAroundOrigin(temp, HalfPi * 0.5f);
		m_Feelers[2] = m_pVehicle->getSpritePosition() + m_dWallDetectionFeelerLength / 2.0f * temp;
	}


	//---------------------------- Separation --------------------------------
	//
	// this calculates a force repelling from the other neighbors
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::Separation(const vector<GameObject*> &neighbors)
	{
		Vector2D SteeringForce;

		for (unsigned int a = 0; a < neighbors.size(); ++a)
		{
			//make sure this agent isn't included in the calculations and that
			//the agent being examined is close enough. ***also make sure it doesn't
			//include the evade target ***
			if ((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged() &&
				(neighbors[a] != m_pTargetAgent1))
			{
				Vector2D ToAgent = m_pVehicle->getSpritePosition() - neighbors[a]->getSpritePosition();

				//scale the force inversely proportional to the agents distance
				//from its neighbor.
				SteeringForce += Vec2DNormalize(ToAgent) / ToAgent.Length();
			}
		}

		return SteeringForce;
	}


	//---------------------------- Alignment ---------------------------------
	//
	//  returns a force that attempts to align this agents heading with that
	//  of its neighbors
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::Alignment(const vector<GameObject*>& neighbors)
	{
		//used to record the average heading of the neighbors
		Vector2D AverageHeading;

		//used to count the number of vehicles in the neighborhood
		int    NeighborCount = 0;

		//iterate through all the tagged vehicles and sum their heading vectors
		for (unsigned int a = 0; a < neighbors.size(); ++a)
		{
			//make sure *this* agent isn't included in the calculations and that
			//the agent being examined  is close enough ***also make sure it doesn't
			//include any evade target ***
			if ((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged() &&
				(neighbors[a] != m_pTargetAgent1))
			{
				AverageHeading += neighbors[a]->getHeading();

				++NeighborCount;
			}
		}

		//if the neighborhood contained one or more vehicles, average their
		//heading vectors.
		if (NeighborCount > 0)
		{
			AverageHeading /= (eFloat)NeighborCount;

			AverageHeading -= m_pVehicle->getHeading();
		}

		return AverageHeading;
	}

	//-------------------------------- Cohesion ------------------------------
	//
	//  returns a steering force that attempts to move the agent towards the
	//  center of mass of the agents in its immediate area
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::Cohesion(const vector<GameObject*> &neighbors)
	{
		//first find the center of mass of all the agents
		Vector2D CenterOfMass, SteeringForce;

		int NeighborCount = 0;

		//iterate through the neighbors and sum up all the position vectors
		for (unsigned int a = 0; a < neighbors.size(); ++a)
		{
			//make sure *this* agent isn't included in the calculations and that
			//the agent being examined is close enough ***also make sure it doesn't
			//include the evade target ***
			if ((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged() &&
				(neighbors[a] != m_pTargetAgent1))
			{
				CenterOfMass += neighbors[a]->getSpritePosition();

				++NeighborCount;
			}
		}

		if (NeighborCount > 0)
		{
			//the center of mass is the average of the sum of positions
			CenterOfMass /= (eFloat)NeighborCount;

			//now seek towards that position
			SteeringForce = Seek(CenterOfMass);
		}

		//the magnitude of cohesion is usually much larger than separation or
		//allignment so it usually helps to normalize it.
		return Vec2DNormalize(SteeringForce);
	}


	/* NOTE: the next three behaviors are the same as the above three, except
			  that they use a cell-space partition to find the neighbors
	*/


	//---------------------------- Separation --------------------------------
	//
	// this calculates a force repelling from the other neighbors
	//
	//  USES SPACIAL PARTITIONING
	//------------------------------------------------------------------------
	/*Vector2D SteeringBehavior::SeparationPlus(const vector<GameObject*> &neighbors)
	{
		Vector2D SteeringForce;

		//iterate through the neighbors and sum up all the position vectors
		for (BaseGameEntity* pV = m_pVehicle->World()->CellSpace()->begin();
			!m_pVehicle->World()->CellSpace()->end();
			pV = m_pVehicle->World()->CellSpace()->next())
		{
			//make sure this agent isn't included in the calculations and that
			//the agent being examined is close enough
			if (pV != m_pVehicle)
			{
				Vector2D ToAgent = m_pVehicle->getSpritePosition() - pV->getSpritePosition();

				//scale the force inversely proportional to the agents distance
				//from its neighbor.
				SteeringForce += Vec2DNormalize(ToAgent) / ToAgent.Length();
			}

		}

		return SteeringForce;
	}
	//---------------------------- Alignment ---------------------------------
	//
	//  returns a force that attempts to align this agents heading with that
	//  of its neighbors
	//
	//  USES SPACIAL PARTITIONING
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::AlignmentPlus(const vector<GameObject*> &neighbors)
	{
		//This will record the average heading of the neighbors
		Vector2D AverageHeading;

		//This count the number of vehicles in the neighborhood
		eFloat    NeighborCount = 0.0;

		//iterate through the neighbors and sum up all the position vectors
		for (MovingEntity* pV = m_pVehicle->World()->CellSpace()->begin();
			!m_pVehicle->World()->CellSpace()->end();
			pV = m_pVehicle->World()->CellSpace()->next())
		{
			//make sure *this* agent isn't included in the calculations and that
			//the agent being examined  is close enough
			if (pV != m_pVehicle)
			{
				AverageHeading += pV->getHeading();

				++NeighborCount;
			}

		}

		//if the neighborhood contained one or more vehicles, average their
		//heading vectors.
		if (NeighborCount > 0.0)
		{
			AverageHeading /= NeighborCount;

			AverageHeading -= m_pVehicle->getHeading();
		}

		return AverageHeading;
	}


	//-------------------------------- Cohesion ------------------------------
	//
	//  returns a steering force that attempts to move the agent towards the
	//  center of mass of the agents in its immediate area
	//
	//  USES SPACIAL PARTITIONING
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::CohesionPlus(const vector<GameObject*> &neighbors)
	{
		//first find the center of mass of all the agents
		Vector2D CenterOfMass, SteeringForce;

		int NeighborCount = 0;

		//iterate through the neighbors and sum up all the position vectors
		for (BaseGameEntity* pV = m_pVehicle->World()->CellSpace()->begin();
			!m_pVehicle->World()->CellSpace()->end();
			pV = m_pVehicle->World()->CellSpace()->next())
		{
			//make sure *this* agent isn't included in the calculations and that
			//the agent being examined is close enough
			if (pV != m_pVehicle)
			{
				CenterOfMass += pV->getSpritePosition();

				++NeighborCount;
			}
		}

		if (NeighborCount > 0)
		{
			//the center of mass is the average of the sum of positions
			CenterOfMass /= (eFloat)NeighborCount;

			//now seek towards that position
			SteeringForce = Seek(CenterOfMass);
		}

		//the magnitude of cohesion is usually much larger than separation or
		//allignment so it usually helps to normalize it.
		return Vec2DNormalize(SteeringForce);
	}


	//--------------------------- Interpose ----------------------------------
	//
	//  Given two agents, this method returns a force that attempts to
	//  position the vehicle between them
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::Interpose(const GameObject* AgentA,
		const GameObject* AgentB)
	{
		//first we need to figure out where the two agents are going to be at
		//time T in the future. This is approximated by determining the time
		//taken to reach the mid way point at the current time at at max speed.
		Vector2D MidPoint = (AgentA->getSpritePosition() + AgentB->getSpritePosition()) / 2.0;

		eFloat TimeToReachMidPoint = Vec2DDistance(m_pVehicle->getSpritePosition(), MidPoint) /
			m_pVehicle->getMaxSpeed();

		//now we have T, we assume that agent A and agent B will continue on a
		//straight trajectory and extrapolate to get their future positions
		Vector2D APos = AgentA->getSpritePosition() + AgentA->Velocity() * TimeToReachMidPoint;
		Vector2D BPos = AgentB->getSpritePosition() + AgentB->Velocity() * TimeToReachMidPoint;

		//calculate the mid point of these predicted positions
		MidPoint = (APos + BPos) / 2.0;

		//then steer to Arrive at it
		return Arrive(MidPoint, fast);
	}

	//--------------------------- Hide ---------------------------------------
	//
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::Hide(const GameObject*           hunter,
		const vector<BaseGameEntity*>& obstacles)
	{
		eFloat    DistToClosest = MaxDouble;
		Vector2D BestHidingSpot;

		std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();
		std::vector<BaseGameEntity*>::const_iterator closest;

		while (curOb != obstacles.end())
		{
			//calculate the position of the hiding spot for this obstacle
			Vector2D HidingSpot = GetHidingPosition((*curOb)->getSpritePosition(),
				(*curOb)->BRadius(),
				hunter->getSpritePosition());

			//work in distance-squared space to find the closest hiding
			//spot to the agent
			eFloat dist = Vec2DDistanceSq(HidingSpot, m_pVehicle->getSpritePosition());

			if (dist < DistToClosest)
			{
				DistToClosest = dist;

				BestHidingSpot = HidingSpot;

				closest = curOb;
			}

			++curOb;

		}//end while

		//if no suitable obstacles found then Evade the hunter
		if (DistToClosest == MaxFloat)
		{
			return Evade(hunter);
		}

		//else use Arrive on the hiding spot
		return Arrive(BestHidingSpot, fast);
	}

	//------------------------- GetHidingPosition ----------------------------
	//
	//  Given the position of a hunter, and the position and radius of
	//  an obstacle, this method calculates a position DistanceFromBoundary
	//  away from its bounding radius and directly opposite the hunter
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::GetHidingPosition(const Vector2D& posOb,
		const eFloat     radiusOb,
		const Vector2D& posHunter)
	{
		//calculate how far away the agent is to be from the chosen obstacle's
		//bounding radius
		const eFloat DistanceFromBoundary = 30.0;
		eFloat       DistAway = radiusOb + DistanceFromBoundary;

		//calculate the heading toward the object from the hunter
		Vector2D ToOb = Vec2DNormalize(posOb - posHunter);

		//scale it to size and add to the obstacles position to get
		//the hiding spot.
		return (ToOb * DistAway) + posOb;
	}


	//------------------------------- FollowPath -----------------------------
	//
	//  Given a series of Vector2Ds, this method produces a force that will
	//  move the agent along the waypoints in order. The agent uses the
	// 'Seek' behavior to move to the next waypoint - unless it is the last
	//  waypoint, in which case it 'Arrives'
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::FollowPath()
	{
		//move to next target if close enough to current target (working in
		//distance squared space)
		if (Vec2DDistanceSq(m_pPath->CurrentWaypoint(), m_pVehicle->getSpritePosition()) <
			m_dWaypointSeekDistSq)
		{
			m_pPath->SetNextWaypoint();
		}

		if (!m_pPath->Finished())
		{
			return Seek(m_pPath->CurrentWaypoint());
		}

		else
		{
			return Arrive(m_pPath->CurrentWaypoint(), normal);
		}
	}

	//------------------------- Offset Pursuit -------------------------------
	//
	//  Produces a steering force that keeps a vehicle at a specified offset
	//  from a leader vehicle
	//------------------------------------------------------------------------
	Vector2D SteeringBehavior::OffsetPursuit(const GameObject*  leader,
		const Vector2D offset)
	{
		//calculate the offset's position in world space
		Vector2D WorldOffsetPos = PointToWorldSpace(offset,
			leader->getHeading(),
			leader->Side(),
			leader->getSpritePosition());

		Vector2D ToOffset = WorldOffsetPos - m_pVehicle->getSpritePosition();

		//the lookahead time is propotional to the distance between the leader
		//and the pursuer; and is inversely proportional to the sum of both
		//agent's velocities
		eFloat LookAheadTime = ToOffset.Length() /
			(m_pVehicle->getMaxSpeed() + leader->getSpeed());

		//now Arrive at the predicted future position of the offset
		return Arrive(WorldOffsetPos + leader->getVelocity() * LookAheadTime, fast);
	}



	//for receiving keyboard input from user
	#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
	//----------------------------- RenderAids -------------------------------
	//
	//------------------------------------------------------------------------
	void SteeringBehavior::RenderAids()
	{

		gdi->TransparentText();
		gdi->TextColor(Cgdi::grey);

		int NextSlot = 0; int SlotSize = 20;

		if (KEYDOWN(VK_INSERT)) { m_pVehicle->SetMaxForce(m_pVehicle->MaxForce() + 1000.0f*m_pVehicle->TimeElapsed()); }
		if (KEYDOWN(VK_DELETE)) { if (m_pVehicle->MaxForce() > 0.2f) m_pVehicle->SetMaxForce(m_pVehicle->MaxForce() - 1000.0f*m_pVehicle->TimeElapsed()); }
		if (KEYDOWN(VK_HOME)) { m_pVehicle->SetMaxSpeed(m_pVehicle->getMaxSpeed() + 50.0f*m_pVehicle->TimeElapsed()); }
		if (KEYDOWN(VK_END)) { if (m_pVehicle->getMaxSpeed() > 0.2f) m_pVehicle->SetMaxSpeed(m_pVehicle->getMaxSpeed() - 50.0f*m_pVehicle->TimeElapsed()); }

		if (m_pVehicle->MaxForce() < 0) m_pVehicle->SetMaxForce(0.0f);
		if (m_pVehicle->getMaxSpeed() < 0) m_pVehicle->SetMaxSpeed(0.0f);

		if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "MaxForce(Ins/Del):"); gdi->TextAtPos(160, NextSlot, ttos(m_pVehicle->MaxForce() / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }
		if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "MaxSpeed(Home/End):"); gdi->TextAtPos(160, NextSlot, ttos(m_pVehicle->getMaxSpeed())); NextSlot += SlotSize; }

		//render the steering force
		if (m_pVehicle->World()->RenderSteeringForce())
		{
			gdi->RedPen();
			Vector2D F = (m_vSteeringForce / Prm.SteeringForceTweaker) * Prm.VehicleScale;
			gdi->Line(m_pVehicle->getSpritePosition(), m_pVehicle->getSpritePosition() + F);
		}

		//render wander stuff if relevant
		if (On(wander) && m_pVehicle->World()->RenderWanderCircle())
		{
			if (KEYDOWN('F')) { m_dWanderJitter += 1.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderJitter, 0.0f, 100.0f); }
			if (KEYDOWN('V')) { m_dWanderJitter -= 1.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderJitter, 0.0f, 100.0f); }
			if (KEYDOWN('G')) { m_dWanderDistance += 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderDistance, 0.0f, 50.0f); }
			if (KEYDOWN('B')) { m_dWanderDistance -= 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderDistance, 0.0f, 50.0f); }
			if (KEYDOWN('H')) { m_dWanderRadius += 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderRadius, 0.0f, 100.0f); }
			if (KEYDOWN('N')) { m_dWanderRadius -= 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderRadius, 0.0f, 100.0f); }


			if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Jitter(F/V): "); gdi->TextAtPos(160, NextSlot, ttos(m_dWanderJitter)); NextSlot += SlotSize; }
			if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Distance(G/B): "); gdi->TextAtPos(160, NextSlot, ttos(m_dWanderDistance)); NextSlot += SlotSize; }
			if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Radius(H/N): "); gdi->TextAtPos(160, NextSlot, ttos(m_dWanderRadius)); NextSlot += SlotSize; }


			//calculate the center of the wander circle
			Vector2D m_vTCC = PointToWorldSpace(Vector2D(m_dWanderDistance*m_pVehicle->BRadius(), 0),
				m_pVehicle->getHeading(),
				m_pVehicle->Side(),
				m_pVehicle->getSpritePosition());
			//draw the wander circle
			gdi->GreenPen();
			gdi->HollowBrush();
			gdi->Circle(m_vTCC, m_dWanderRadius*m_pVehicle->BRadius());

			//draw the wander target
			gdi->RedPen();
			gdi->Circle(PointToWorldSpace((m_vWanderTarget + Vector2D(m_dWanderDistance, 0))*m_pVehicle->BRadius(),
				m_pVehicle->getHeading(),
				m_pVehicle->Side(),
				m_pVehicle->getSpritePosition()), 3);
		}

		//render the detection box if relevant
		if (m_pVehicle->World()->RenderDetectionBox())
		{
			gdi->GreyPen();

			//a vertex buffer rqd for drawing the detection box
			static std::vector<Vector2D> box(4);

			eFloat length = Prm.MinDetectionBoxLength +
				(m_pVehicle->getSpeed() / m_pVehicle->getMaxSpeed()) *
				Prm.MinDetectionBoxLength;

			//verts for the detection box buffer
			box[0] = Vector2D(0, m_pVehicle->BRadius());
			box[1] = Vector2D(length, m_pVehicle->BRadius());
			box[2] = Vector2D(length, -m_pVehicle->BRadius());
			box[3] = Vector2D(0, -m_pVehicle->BRadius());


			if (!m_pVehicle->isSmoothingOn())
			{
				box = WorldTransform(box, m_pVehicle->getSpritePosition(), m_pVehicle->getHeading(), m_pVehicle->Side());
				gdi->ClosedShape(box);
			}
			else
			{
				box = WorldTransform(box, m_pVehicle->getSpritePosition(), m_pVehicle->SmoothedHeading(), m_pVehicle->SmoothedHeading().Perp());
				gdi->ClosedShape(box);
			}


			//////////////////////////////////////////////////////////////////////////
			//the detection box length is proportional to the agent's velocity
			m_dDBoxLength = Prm.MinDetectionBoxLength +
				(m_pVehicle->getSpeed() / m_pVehicle->getMaxSpeed()) *
				Prm.MinDetectionBoxLength;

			//tag all obstacles within range of the box for processing
			m_pVehicle->World()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);

			//this will keep track of the closest intersecting obstacle (CIB)
			BaseGameEntity* ClosestIntersectingObstacle = NULL;

			//this will be used to track the distance to the CIB
			eFloat DistToClosestIP = MaxDouble;

			//this will record the transformed local coordinates of the CIB
			Vector2D LocalPosOfClosestObstacle;

			std::vector<BaseGameEntity*>::const_iterator curOb = m_pVehicle->World()->Obstacles().begin();

			while (curOb != m_pVehicle->World()->Obstacles().end())
			{
				//if the obstacle has been tagged within range proceed
				if ((*curOb)->IsTagged())
				{
					//calculate this obstacle's position in local space
					Vector2D LocalPos = PointToLocalSpace((*curOb)->getSpritePosition(),
						m_pVehicle->Heading(),
						m_pVehicle->Side(),
						m_pVehicle->getSpritePosition());

					//if the local position has a negative x value then it must lay
					//behind the agent. (in which case it can be ignored)
					if (LocalPos.x >= 0)
					{
						//if the distance from the x axis to the object's position is less
						//than its radius + half the width of the detection box then there
						//is a potential intersection.
						if (fabs(LocalPos.y) < ((*curOb)->BRadius() + m_pVehicle->BRadius()))
						{
							gdi->ThickRedPen();
							gdi->ClosedShape(box);
						}
					}
				}

				++curOb;
			}


			/////////////////////////////////////////////////////
		}

		//render the wall avoidnace feelers
		if (On(wall_avoidance) && m_pVehicle->World()->RenderFeelers())
		{
			gdi->OrangePen();

			for (unsigned int flr = 0; flr < m_Feelers.size(); ++flr)
			{

				gdi->Line(m_pVehicle->getSpritePosition(), m_Feelers[flr]);
			}
		}

		//render path info
		if (On(follow_path) && m_pVehicle->World()->RenderPath())
		{
			m_pPath->Render();
		}


		if (On(separation))
		{
			if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Separation(S/X):"); gdi->TextAtPos(160, NextSlot, ttos(m_dWeightSeparation / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }

			if (KEYDOWN('S')) { m_dWeightSeparation += 200 * m_pVehicle->TimeElapsed(); Clamp(m_dWeightSeparation, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
			if (KEYDOWN('X')) { m_dWeightSeparation -= 200 * m_pVehicle->TimeElapsed(); Clamp(m_dWeightSeparation, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
		}

		if (On(allignment))
		{
			if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Alignment(A/Z):"); gdi->TextAtPos(160, NextSlot, ttos(m_dWeightAlignment / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }

			if (KEYDOWN('A')) { m_dWeightAlignment += 200 * m_pVehicle->TimeElapsed(); Clamp(m_dWeightAlignment, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
			if (KEYDOWN('Z')) { m_dWeightAlignment -= 200 * m_pVehicle->TimeElapsed(); Clamp(m_dWeightAlignment, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
		}

		if (On(cohesion))
		{
			if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Cohesion(D/C):"); gdi->TextAtPos(160, NextSlot, ttos(m_dWeightCohesion / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }
			if (KEYDOWN('D')) { m_dWeightCohesion += 200 * m_pVehicle->TimeElapsed(); Clamp(m_dWeightCohesion, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
			if (KEYDOWN('C')) { m_dWeightCohesion -= 200 * m_pVehicle->TimeElapsed(); Clamp(m_dWeightCohesion, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
		}

		if (On(follow_path))
		{
			eFloat sd = sqrt(m_dWaypointSeekDistSq);
			if (m_pVehicle->ID() == 0) { gdi->TextAtPos(5, NextSlot, "SeekDistance(D/C):"); gdi->TextAtPos(160, NextSlot, ttos(sd)); NextSlot += SlotSize; }

			if (KEYDOWN('D')) { m_dWaypointSeekDistSq += 1.0; }
			if (KEYDOWN('C')) { m_dWaypointSeekDistSq -= 1.0; Clamp(m_dWaypointSeekDistSq, 0.0f, 400.0f); }
		}

	}*/