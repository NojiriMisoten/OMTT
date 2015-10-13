#include <math.h>
#include "matrix.h"
#include "vector.h"


//íPà çsóÒ/////////////////////////////////////////////////////////////////////////////////////////////////
void Matrix3Identity( MATRIX3 *Out )
{
	Out->m11 = 1.0f;
	Out->m12 = 0.0f;
	Out->m13 = 0.0f;

	Out->m21 = 0.0f;
	Out->m22 = 1.0f;
	Out->m23 = 0.0f;

	Out->m31 = 0.0f;
	Out->m32 = 0.0f;
	Out->m33 = 1.0f;
}


//à⁄ìÆ/////////////////////////////////////////////////////////////////////////////////////////////////
void Matrix3Translation( MATRIX3 *Out, VECTOR2 *Position )
{
	Matrix3Identity( Out );

	Out->m31 = Position->x;
	Out->m32 = Position->y;
}



//âÒì]/////////////////////////////////////////////////////////////////////////////////////////////////
void Matrix3Rotation( MATRIX3 *Out, float Angle )
{
	Matrix3Identity( Out );


	float st, ct;

	st = sinf( Angle );
	ct = cosf( Angle );

	Out->m11 = ct;
	Out->m12 = -st;
	Out->m21 = st;
	Out->m22 = ct;
}


//ägèk/////////////////////////////////////////////////////////////////////////////////////////////////
void Matrix3Scaling( MATRIX3 *Out, VECTOR2 *Scale )
{
	Matrix3Identity( Out );

	Out->m11 = Scale->x;
	Out->m22 = Scale->y;
}


//èÊéZ/////////////////////////////////////////////////////////////////////////////////////////////////
void Matrix3Multiply( MATRIX3 *Out, MATRIX3 *Matrix1, MATRIX3 *Matrix2 )
{
	MATRIX3 matrix;

	matrix.m11 = Matrix1->m11 * Matrix2->m11 + Matrix1->m12 * Matrix2->m21 + Matrix1->m13 * Matrix2->m31;
	matrix.m12 = Matrix1->m11 * Matrix2->m12 + Matrix1->m12 * Matrix2->m22 + Matrix1->m13 * Matrix2->m32;
	matrix.m13 = Matrix1->m11 * Matrix2->m13 + Matrix1->m12 * Matrix2->m23 + Matrix1->m13 * Matrix2->m33;

	matrix.m21 = Matrix1->m21 * Matrix2->m11 + Matrix1->m22 * Matrix2->m21 + Matrix1->m23 * Matrix2->m31;
	matrix.m22 = Matrix1->m21 * Matrix2->m12 + Matrix1->m22 * Matrix2->m22 + Matrix1->m23 * Matrix2->m32;
	matrix.m23 = Matrix1->m21 * Matrix2->m13 + Matrix1->m22 * Matrix2->m23 + Matrix1->m23 * Matrix2->m33;

	matrix.m31 = Matrix1->m31 * Matrix2->m11 + Matrix1->m32 * Matrix2->m21 + Matrix1->m33 * Matrix2->m31;
	matrix.m32 = Matrix1->m31 * Matrix2->m12 + Matrix1->m32 * Matrix2->m22 + Matrix1->m33 * Matrix2->m32;
	matrix.m33 = Matrix1->m31 * Matrix2->m13 + Matrix1->m32 * Matrix2->m23 + Matrix1->m33 * Matrix2->m33;

	*Out = matrix;
}




//ãtçsóÒ/////////////////////////////////////////////////////////////////////////////////////////////////
void Matrix3Inverse( MATRIX3 *Out, MATRIX3 *Matrix )
{
	MATRIX3 matrix, inverse;
	float buf;

	matrix = *Matrix;

	for( int i = 0; i < 3; i++ )
	{
		buf = 1.0f / matrix.m[i][i];

		for( int j = 0; j < 3; j++ )
		{
			matrix.m[i][j] *= buf;
			inverse.m[i][j] *= buf;
		}

		for( int j = 0; j < 3; j++ )
		{
			if( i != j )
			{
				buf = matrix.m[j][i];
				for( int k = 0; k < 3; k++ )
				{
					matrix.m[j][k] -= matrix.m[i][k] * buf;
					inverse.m[j][k] -= inverse.m[i][k] * buf;
				}
			}
		}
	}

	*Out = inverse;
}



