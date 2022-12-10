#pragma once

#include "mathlib.hpp"
#include "math_pfns.hpp"

class Vector2D
{
public:
	Vector2D();
	Vector2D( const Vector2D& vector );
	Vector2D( float vector_x, float vector_y );

public:
	float& operator[]( int index );
	float operator[]( int index ) const;

public:
	Vector2D& operator=( const Vector2D& vector );

public:
	float x = 0.f;																			// 0x0000
	float y = 0.f;																			// 0x0004
};
// sizeof( Vector2D ) = 0x0008 ( 8 )