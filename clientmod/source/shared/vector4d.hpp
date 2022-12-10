#pragma once

#include "mathlib.hpp"
#include "math_pfns.hpp"

class Vector4D
{
public:
	Vector4D();
	Vector4D( const Vector4D& vector );
	Vector4D( float vector_x, float vector_y, float vector_z, float vector_w );

public:
	float& operator[]( int index );
	float operator[]( int index ) const;

public:
	Vector4D& operator=( const Vector4D& vector );

public:
	float x = 0.f;																			// 0x0000
	float y = 0.f;																			// 0x0004
	float z = 0.f;																			// 0x0008
	float w = 0.f;																			// 0x000C
};
// sizeof( Vector4D ) = 0x0010 ( 16 )