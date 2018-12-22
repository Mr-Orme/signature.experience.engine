#ifndef VIEW_H
#define VIEW_H
#include "Definitions.h"

class View{
public:

	bool initialize(EngineDefs::Vector position);
	bool update();
	
	EngineDefs::Vector position;

};


#endif