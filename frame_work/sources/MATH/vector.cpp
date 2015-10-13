
#include "matrix.h"
#include "vector.h"
#include <math.h>


//À•W•ÏŠ·//////////////////////////////////////////////////////////////////////////
void Vector2Transform( VECTOR2 *Out, VECTOR2 *Vector, MATRIX3 *Matrix )
{
	Out->x = Matrix->m11 * Vector->x + Matrix->m21 * Vector->y + Matrix->m31;
	Out->y = Matrix->m12 * Vector->x + Matrix->m22 * Vector->y + Matrix->m32;
}


// ‘ã“ü ////////////////////////////////////////////////////////////////
VECTOR2& VECTOR2::operator=( const VECTOR2 &Vector )
{

	x = Vector.x;
	y = Vector.y;

	return *this;
}


// ‰ÁŽZ ////////////////////////////////////////////////////////////////
VECTOR2 VECTOR2::operator+( const VECTOR2 &Vector )
{
	VECTOR2 vector;

	vector.x = x + Vector.x;
	vector.y = y + Vector.y;

	return vector;
}

// ‰ÁŽZ ////////////////////////////////////////////////////////////////
VECTOR2& VECTOR2::operator+=( const VECTOR2 &Vector )
{

	x += Vector.x;
	y += Vector.y;

	return *this;
}



// Œ¸ŽZ ////////////////////////////////////////////////////////////////
VECTOR2 VECTOR2::operator-( const VECTOR2 &Vector )
{
	VECTOR2 vector;

	vector.x = x - Vector.x;
	vector.y = y - Vector.y;

	return vector;
}

// Œ¸ŽZ ////////////////////////////////////////////////////////////////
VECTOR2& VECTOR2::operator-=( const VECTOR2 &Vector )
{

	x -= Vector.x;
	y -= Vector.y;

	return *this;
}



// æŽZ ////////////////////////////////////////////////////////////////
VECTOR2 VECTOR2::operator*( float f )
{
	VECTOR2 vector;

	vector.x = x * f;
	vector.y = y * f;

	return vector;
}


// æŽZ ////////////////////////////////////////////////////////////////
VECTOR2& VECTOR2::operator*=( float f )
{

	x *= f;
	y *= f;

	return *this;
}


// œŽZ ////////////////////////////////////////////////////////////////
VECTOR2 VECTOR2::operator/( float f )
{
	VECTOR2 vector;

	vector.x = x / f;
	vector.y = y / f;

	return vector;
}

// œŽZ ////////////////////////////////////////////////////////////////
VECTOR2& VECTOR2::operator/=( float f )
{

	x /= f;
	y /= f;

	return *this;
}


// ŠOÏ ////////////////////////////////////////////////////////////////
float Vector2Cross( VECTOR2 *Vector1, VECTOR2 *Vector2 )
{

	return Vector1->x * Vector2->y - Vector1->y * Vector2->x;

}


// “àÏ ////////////////////////////////////////////////////////////////
float Vector2Dot( VECTOR2 *Vector1, VECTOR2 *Vector2 )
{

	return Vector1->x * Vector2->x + Vector1->y * Vector2->y;

}

// ³‹K‰» ////////////////////////////////////////////////////////////////
void Vector2Normalize(VECTOR2 *OutVector, VECTOR2 *InVector)
{
	float length = fabs(sqrtf(InVector->x * InVector->x + InVector->y * InVector->y));

	VECTOR2 culcVec = (*InVector);
	culcVec = culcVec / length;

	*OutVector = culcVec;
}