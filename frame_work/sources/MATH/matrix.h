#pragma once


struct VECTOR2;


struct MATRIX3
{
	union
	{
		struct
		{
			float m11; float m12; float m13;
			float m21; float m22; float m23;
			float m31; float m32; float m33;
		};

		float m[3][3];
	};
};


void Matrix3Identity( MATRIX3 *Out );
void Matrix3Translation( MATRIX3 *Out, VECTOR2 *Position );
void Matrix3Rotation( MATRIX3 *Out, float Angle );
void Matrix3Scaling( MATRIX3 *Out, VECTOR2 *Scale );
void Matrix3Multiply( MATRIX3 *Out, MATRIX3 *Matrix1, MATRIX3 *Matrix2 );
void Matrix3Inverse( MATRIX3 *Out, MATRIX3 *Matrix );

