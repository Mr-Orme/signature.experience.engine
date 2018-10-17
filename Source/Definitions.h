#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//Basic Types
typedef unsigned int	Uint32;
typedef float			ENGINE_FLT;
typedef Uint32			EngineInt;

struct Position
{
	ENGINE_FLT x;
	ENGINE_FLT y;
	
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
		return { abs(src.x),abs(src.y) };
	}
};


enum class DIRECTION {N =0, E = 90, S = 180, W = 270};

//TODO:make levels an array of resource managers! or something that has nearby areas. . .
enum class LEVEL {ENGINE_LEVEL_MAIN, ENGINE_LEVEL_BASEMENT,};


struct RGBA
{
	EngineInt R;
	EngineInt G;
	EngineInt B;
	EngineInt A;
};

//Constants
const ENGINE_FLT PI = 3.14159f;

#endif