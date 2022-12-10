#pragma once

#include "../../include/auto.hpp"
#include "../../include/base.hpp"
#include "../../include/win32.hpp"

#if !defined( M_PI )
#define M_PI																					( 3.14159265358979323846 )
#endif // !M_PI

#if !defined( M_PI_F )
#define M_PI_F																				( ( float )( M_PI ) )
#endif // !M_PI_F

#if !defined( RAD2DEG )
#define RAD2DEG( x )																	( ( float )( x ) * ( float )( 180.f / M_PI_F ) )
#endif // !RAD2DEG

#if !defined( DEG2RAD )
#define DEG2RAD( x )																	( ( float )( x ) * ( float )( M_PI_F / 180.f ) )
#endif // !DEG2RAD

struct matrix3x4_t;

class VMatrix;

class QAngle;
class Vector;
class VectorAligned;
class Vector2D;
class Vector4D;

class RadianEuler
{
public:
	float x = 0.f;																			// 0x0000
	float y = 0.f;																			// 0x0004
	float z = 0.f;																			// 0x0008
};
// sizeof( RadianEuler ) = 0x000C ( 12 )

class Quaternion
{
public:
	float x = 0.f;																			// 0x0000
	float y = 0.f;																			// 0x0004
	float z = 0.f;																			// 0x0008
	float w = 0.f;																			// 0x000C
};
// sizeof( Quaternion ) = 0x0010 ( 16 )

template< typename DataType >
FORCEINLINE DataType Square( const DataType& data )
{
	return ( data * data );
}

void SinCos( float radians, float* sine, float* cosine );