#include <iostream>
#include "View.h"
#include "Object.h"
//#include "ComponentsList.h"
//#include "GraphicsDevice.h"

View::View()
{

}

View::~View()
{
	
}

//**************************************
//set initial values
bool View::initialize(ENGINE_FLT x, ENGINE_FLT y)
//**************************************
{
	position = { x,y };
	

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

//**************************************
//print view data
void View::Print()
//**************************************
{
	std::cout << "*****VIEW******" << std::endl <<
		"View X: " << position.x << std::endl <<
		"View Y: " << position.y << std:: endl <<
		std::endl;
}

void View::addScroller( Object * object)
{
	scrollers.push_back(object);
}
