#pragma once

#include "../../include/auto.hpp"
#include "../../include/base.hpp"
#include "../../include/win32.hpp"

#include "../../memory/base.hpp"

template< typename DataType >
FORCEINLINE DataType AlignValue( DataType val, unsigned alignment )
{
	return DataType( ( unsigned( val ) + alignment - 1 ) & ~( alignment - 1 ) );
}

#define PAD_NUMBER( number, boundary )													( ( ( ( number ) + ( ( boundary ) - 1 ) ) / ( boundary ) ) * ( boundary ) )

template< typename DataType >
FORCEINLINE DataType clamp( const DataType& val, const DataType& minVal, const DataType& maxVal )
{
	if( val < minVal )
	{
		return minVal;
	}
	else if( val > maxVal )
	{
		return maxVal;
	}
	else
	{
		return val;
	}
}

using qboolean = int;
using byte = unsigned char;
using word = unsigned short;

enum ThreeState_t
{
	TRS_FALSE,
	TRS_TRUE,
	TRS_NONE,
};
// sizeof( ThreeState_t ) = 0x0004 ( 4 )

using vec_t = float;

FORCEINLINE unsigned long& FloatBits( vec_t& f )
{
	return *reinterpret_cast< unsigned long* >( &f );
}

FORCEINLINE const unsigned long& FloatBits( const vec_t& f )
{
	return *reinterpret_cast< const unsigned long* >( &f );
}

FORCEINLINE vec_t BitsToFloat( unsigned long i )
{
	void* bits = &i;
	return *reinterpret_cast< vec_t* >( bits );
}

FORCEINLINE bool IsFinite( vec_t f )
{
	return ( ( FloatBits( f ) & 0x7F800000 ) != 0x7F800000 );
}

FORCEINLINE unsigned long FloatAbsBits( vec_t f )
{
	return ( FloatBits( f ) & 0x7FFFFFFF );
}

FORCEINLINE float FloatMakeNegative( vec_t f )
{
	return BitsToFloat( FloatBits( f ) | 0x80000000 );
}

#pragma intrinsic( fabs )
FORCEINLINE float FloatMakePositive( vec_t f )
{
	return fabs( f );
}

FORCEINLINE float FloatNegate( vec_t f )
{
	return BitsToFloat( FloatBits( f ) ^ 0x80000000 );
}

#define FLOAT32_NAN_BITS																				( ( unsigned long )0x7FC00000 )
#define FLOAT32_NAN																							( BitsToFloat( FLOAT32_NAN_BITS ) )

#define VEC_T_NAN																								( FLOAT32_NAN )

struct color24
{
public:
	byte r = 0;																										// 0x0000
	byte g = 0;																										// 0x0001
	byte b = 0;																										// 0x0002
};
// sizeof( color24 ) = 0x0003 ( 3 )

struct color32
{
public:
	bool operator!=( const color32& other ) const;

public:
	byte r = 0;																										// 0x0000
	byte g = 0;																										// 0x0001
	byte b = 0;																										// 0x0002
	byte a = 0;																										// 0x0003
};
// sizeof( color32 ) = 0x0004 ( 4 )

FORCEINLINE bool color32::operator!=( const color32& other ) const
{
	return ( r != other.r ||
					 g != other.g ||
					 b != other.b ||
					 a != other.a );
}

struct colorVec
{
public:
	unsigned r = 0;																								// 0x0000
	unsigned g = 0;																								// 0x0004
	unsigned b = 0;																								// 0x0008
	unsigned a = 0;																								// 0x000C
};
// sizeof( colorVec ) = 0x0010 ( 16 )

struct vrect_t
{
public:
	int x = 0;																										// 0x0000
	int y = 0;																										// 0x0004
	int width = 0;																								// 0x0008
	int height = 0;																								// 0x000C
	vrect_t* pnext = nullptr;																			// 0x0010
};
// sizeof( vrect_t ) = 0x0014 ( 20 )

struct Rect_t
{
public:
	int x = 0;																										// 0x0000
	int y = 0;																										// 0x0004
	int width = 0;																								// 0x0008
	int height = 0;																								// 0x000C
};
// sizeof( Rect_t ) = 0x0010 ( 16 )

struct interval_t
{
	float start = 0.f;																						// 0x0000
	float range = 0.f;																						// 0x0004
};
// sizeof( interval_t ) = 0x0008 ( 8 )