#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <cmath>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <iostream>
//TODO:: const correctness. Check to see if consts can be put on pointer parameters and class methods.
//TODO:: clear out #includes. Make sure every #include is needed in ever file! Cleared out all includes and have put them
//in definitions
//TODO:: Move initialization to the constructor and stopping/finishing to the destructor.
//TODO:: Move default initialization of member variables to .h file using {} for ALL classes!
//Basic Types

namespace EngineDefs
{
	typedef unsigned int	Uint32;
	typedef float			Float;
	typedef Uint32			Int;

	struct Vector
	{
		Float x;
		Float y;

		friend Vector operator+ (const Vector & src1, const Vector & src2)
		{
			return { src1.x + src2.x, src1.y + src2.y };
		}
		friend Vector& operator+=(Vector& src1, const Vector& src2)
		{
			src1.x += src2.x;
			src1.y += src2.y;
			return src1;
		}
		friend Vector operator- (const Vector & src1, const Vector & src2)
		{
			return { src1.x - src2.x, src1.y - src2.y };
		}
		friend bool operator> (const Vector & src1, int src2)
		{
			if (src1.x > src2 && src1.y > src2)
			{
				return true;
			}
			return false;
		}
		friend bool operator< (const Vector & src1, int src2)
		{
			if (src1.x < src2 && src1.y < src2)
			{
				return true;
			}
			return false;
		}
		friend bool operator== (const Vector & src1, const Vector & src2)
		{
			if (src1.x == src2.x && src1.y == src2.y)
			{
				return true;
			}
			return false;
		}
		friend Vector abs(Vector & src)
		{
			return { std::abs((float)src.x),std::abs((float)src.y) };
		}
	};

}

	struct RGBA
	{
		EngineDefs::Int R;
		EngineDefs::Int G;
		EngineDefs::Int B;
		EngineDefs::Int A;
	};

	//Constants
	const EngineDefs::Float PI = 3.14159f;


#endif