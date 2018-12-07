#ifndef VIEW_H
#define VIEW_H
#include "Definitions.h"

class View{
public:

	bool initialize(Position position);
	bool update();
	
	Position position;

};


#endif