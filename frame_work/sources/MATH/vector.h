#pragma once


struct MATRIX3;


struct VECTOR2
{
	float x;
	float y;

	VECTOR2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	VECTOR2( float _x, float _y )
	{
		x = _x;
		y = _y;
	}

	VECTOR2 &operator=( const VECTOR2 &Vector );

	VECTOR2 operator+( const VECTOR2 &Vector );
	VECTOR2 &operator+=( const VECTOR2 &Vector );

	VECTOR2 operator-( const VECTOR2 &Vector );
	VECTOR2 &operator-=( const VECTOR2 &Vector );

	VECTOR2 operator*( float f );
	VECTOR2 &operator*=( float f );

	VECTOR2 operator/( float f );
	VECTOR2 &operator/=( float f );

};


void Vector2Transform( VECTOR2 *Out, VECTOR2 *Vector, MATRIX3 *Matrix );

float Vector2Cross( VECTOR2 *Vector1, VECTOR2 *Vector2 );
float Vector2Dot( VECTOR2 *Vector1, VECTOR2 *Vector2 );
void Vector2Normalize(VECTOR2 *OutVector, VECTOR2 *InVector);
