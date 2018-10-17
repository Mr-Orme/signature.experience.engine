#ifndef VIEW_H
#define VIEW_H
#include <vector>
#include "Definitions.h"

class Object;

class View{
public:
	View();
	~View();

	bool initialize(ENGINE_FLT x, ENGINE_FLT y);
	bool update();
	inline void Print();
	
	inline Position getPosition(){return position;}

	void addScroller(Object* object);
	inline void setPosition(Position position){this -> position = position;}
	inline void setX(ENGINE_FLT x){this -> position.x = x;}
	inline void setY(ENGINE_FLT y){this -> position.y = y;}
private:
	Position position;
	//TODO: move scrolling to event manager?? Perhaps a scroll component? Has a position that is a vector for amount of scroll?
	std::vector<Object*> scrollers;
	static const EngineInt border;

};


#endif