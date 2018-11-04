#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <cmath>
//TODO:: const correctness. Check to see if consts can be put on pointer parameters and class methods.
//TODO:: clear out #includes. Make sure every #include is needed in ever file!
//TODO:: Move initialization to the constructor and stopping/finishing to the destructor.
//TODO:: Move default initialization of member variables to .h file using {} for ALL classes!
//Basic Types
typedef unsigned int	Uint32;
typedef float			EngineFloat;
typedef Uint32			EngineInt;

struct Position
{
	EngineFloat x;
	EngineFloat y;
	
	friend Position operator+ (const Position & src1, const Position & src2)
	{
		return { src1.x + src2.x, src1.y + src2.y };
	}
	friend Position operator- (const Position & src1, const Position & src2)
	{
		return { src1.x - src2.x, src1.y - src2.y };
	}
	friend bool operator> (const Position & src1, int src2)
	{
		if (src1.x > src2 && src1.y > src2)
		{
			return true;
		}
		return false;
	}
	friend bool operator< (const Position & src1, int src2)
	{
		if (src1.x < src2 && src1.y < src2)
		{
			return true;
		}
		return false;
	}
	friend bool operator== (const Position & src1, const Position & src2)
	{
		if (src1.x == src2.x && src1.y == src2.y)
		{
			return true;
		}
		return false;
	}
	friend Position abs(Position & src)
	{
		return { abs((float)src.x),abs((float)src.y) };
	}
};


struct RGBA
{
	EngineInt R;
	EngineInt G;
	EngineInt B;
	EngineInt A;
};

//Constants
const EngineFloat PI = 3.14159f;

#endif