#include "View.h"
#include "Object.h"


//**************************************
//set initial values
bool View::initialize(EngineDefs::Vector position)
//**************************************
{
	this->position = position;
	return true;
}

//**************************************
//NOT USED
bool View::update()
//**************************************
{
//*********************************************************************************
	return true;
}


//void View::addScroller( Object * object)
//{
//	//TODO: scrollers component based on time, owner position, or input. Store the amount of scroll in a position variable.
//	//owner position could have a map of scroll vectors based ond different positions. . .
//	scrollers.push_back(object);
//}
