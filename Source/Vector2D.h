#ifndef S2DVECTOR_H
#define S2DVECTOR_H
//------------------------------------------------------------------------
//
//  Name:   Vector2D.h
//
//  Desc:   2D vector struct
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include <iosfwd>
#include <limits>
#include "Utils.h"
#include "Definitions.h"


struct Vector2D
{
	eFloat x;
	eFloat y;

	Vector2D() :x(0.0), y(0.0) {}
	Vector2D(eFloat a, eFloat b) :x(a), y(b) {}

	//sets x and y to zero
	void Zero() { x = 0.0; y = 0.0; }

	//returns true if both x and y are zero
	bool isZero()const { return (x*x + y * y) < MinDouble; }

	//returns the length of the vector
	inline eFloat    Length()const;

	//returns the squared length of the vector (thereby avoiding the sqrt)
	inline eFloat    LengthSq()const;

	inline void      Normalize();

	inline eFloat    Dot(const Vector2D& v2)const;

	//returns positive if v2 is clockwise of this vector,
	//negative if anticlockwise (assuming the Y axis is pointing down,
	//X axis to right like a Window app)
	inline int       Sign(const Vector2D& v2)const;

	//returns the vector that is perpendicular to this one.
	inline Vector2D  Perp()const;

	//adjusts x and y so that the length of the vector does not exceed max
	inline void      Truncate(eFloat max);

	//returns the distance between this vector and th one passed as a parameter
	inline eFloat    Distance(const Vector2D &v2)const;

	//squared version of above.
	inline eFloat    DistanceSq(const Vector2D &v2)const;

	inline void      Reflect(const Vector2D& norm);

	//returns the vector that is the reverse of this vector
	inline Vector2D  GetReverse()const;


	eFloat getAngleRadians(const Vector2D & v2)
	{
		return acos(Dot(v2));
	}
	eFloat getAngleDegrees(const Vector2D & v2)
	{
		return (eFloat)(getAngleRadians(v2) * 180 / Pi);
	}
	//we need some overloaded operators
	const Vector2D& operator+=(const Vector2D &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2D& operator-=(const Vector2D &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2D& operator*=(const eFloat& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2D& operator/=(const eFloat& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const Vector2D& rhs)const
	{
		return (isEqual(x, rhs.x) && isEqual(y, rhs.y));
	}

	bool operator!=(const Vector2D& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y);
	}

};

//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D &lhs, eFloat rhs);
inline Vector2D operator*(eFloat lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, eFloat val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline eFloat Vector2D::Length()const
{
	return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline eFloat Vector2D::LengthSq()const
{
	return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline eFloat Vector2D::Dot(const Vector2D &v2)const
{
	return x * v2.x + y * v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum { clockwise = 1, anticlockwise = -1 };

inline int Vector2D::Sign(const Vector2D& v2)const
{
	if (y*v2.x > x*v2.y)
	{
		return anticlockwise;
	}
	else
	{
		return clockwise;
	}
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::Perp()const
{
	return Vector2D(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
inline eFloat Vector2D::Distance(const Vector2D &v2)const
{
	eFloat ySeparation = v2.y - y;
	eFloat xSeparation = v2.x - x;

	return sqrt(ySeparation*ySeparation + xSeparation * xSeparation);
}


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline eFloat Vector2D::DistanceSq(const Vector2D &v2)const
{
	eFloat ySeparation = v2.y - y;
	eFloat xSeparation = v2.x - x;

	return ySeparation * ySeparation + xSeparation * xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector2D::Truncate(eFloat max)
{
	if (this->Length() > max)
	{
		this->Normalize();

		*this *= max;
	}
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
inline void Vector2D::Reflect(const Vector2D& norm)
{
	*this += 2.0f * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::GetReverse()const
{
	return Vector2D(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void Vector2D::Normalize()
{
	eFloat vector_length = this->Length();

	if (vector_length > std::numeric_limits<eFloat>::epsilon())
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}


//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalize(const Vector2D &v)
{
	Vector2D vec = v;

	eFloat vector_length = vec.Length();

	if (vector_length > std::numeric_limits<eFloat>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}


inline eFloat Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

	eFloat ySeparation = v2.y - v1.y;
	eFloat xSeparation = v2.x - v1.x;

	return sqrt(ySeparation*ySeparation + xSeparation * xSeparation);
}

inline eFloat Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

	eFloat ySeparation = v2.y - v1.y;
	eFloat xSeparation = v2.x - v1.x;

	return ySeparation * ySeparation + xSeparation * xSeparation;
}

inline eFloat Vec2DLength(const Vector2D& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline eFloat Vec2DLengthSq(const Vector2D& v)
{
	return (v.x*v.x + v.y*v.y);
}


//inline Vector2D POINTStoVector(const POINTS& p)
//{
	//return Vector2D(p.x, p.y);
//}

//inline Vector2D POINTtoVector(const POINT& p)
//{
	//return Vector2D((eFloat)p.x, (eFloat)p.y);
//}

//inline POINTS VectorToPOINTS(const Vector2D& v)
//{
	//POINTS p;
	//p.x = (short)v.x;
	//p.y = (short)v.y;

	//return p;
//}

//inline POINT VectorToPOINT(const Vector2D& v)
//{
	//POINT p;
	//p.x = (long)v.x;
	//p.y = (long)v.y;

	//return p;
//}



//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D &lhs, eFloat rhs)
{
	Vector2D result(lhs);
	result *= rhs;
	return result;
}

inline Vector2D operator*(eFloat lhs, const Vector2D &rhs)
{
	Vector2D result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &lhs, eFloat val)
{
	Vector2D result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}

///////////////////////////////////////////////////////////////////////////////


//treats a window as a toroid
inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) { pos.x = 0.0; }

	if (pos.x < 0) { pos.x = (eFloat)MaxX; }

	if (pos.y < 0) { pos.y = (eFloat)MaxY; }

	if (pos.y > MaxY) { pos.y = 0.0; }
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return (p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return !((p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
	return !((p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom));
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Vector2D posFirst,
	Vector2D facingFirst,
	Vector2D posSecond,
	eFloat    fov)
{
	Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

	return facingFirst.Dot(toTarget) >= cos(fov / 2.0);
}





#endif
