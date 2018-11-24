#ifndef VIEW_H
#define VIEW_H
#include <vector>
#include "Definitions.h"

class View{
public:

	bool initialize(Position position);
	bool update();
	//Declared addScroller function within header file before beginning definition process.
	void addScroller(Object* object);
	Position position;

};


#endif