#pragma once

#include "../../include/auto.hpp"
#include "../../include/base.hpp"
#include "../../include/win32.hpp"

#include "../../memory/base.hpp"

#include <malloc.h>
#include <new.h>
#include <string.h>

using uint8 = unsigned char;
// sizeof( uint8 ) = 0x0001 ( 1 )

using int16 = __int16;
using uint16 = unsigned __int16;
using int32 = __int32;
using uint32 = unsigned __int32;
using int64 = __int64;
using uint64 = unsigned __int64;

using intp = __int32;
using uintp = unsigned __int32;

using float32 = float;
using float64 = double;

using uint = unsigned int;

#define SINGLE_INHERITANCE																			__single_inheritance
#define MULTIPLE_INHERITANCE																		__multiple_inheritance

#define NO_VTABLE																								__declspec( novtable )

#define abstract_class																					class

#define ALIGN16																									__declspec( align( 16 ) )
#define ALIGN32																									__declspec( align( 32 ) )

#define SELECTANY																								__declspec( selectany )

template< typename DataType >
FORCEINLINE DataType WordSwapC( DataType w )
{
	uint16 temp = 0;

	temp = ( *( reinterpret_cast< uint16* >( &w ) & 0xFF00 ) >> 8 );
	temp |= ( *( reinterpret_cast< uint16* >( &w ) & 0x00FF ) << 8 );

	return *reinterpret_cast< DataType* >( &temp );
}

template< typename DataType >
FORCEINLINE DataType DWordSwapC( DataType dw )
{
	uint32 temp = 0;

	temp = ( *( reinterpret_cast< uint32* >( &dw ) ) >> 24 );
	temp |= ( *( reinterpret_cast< uint32* >( &dw ) & 0x00FF0000 ) >> 8 );
	temp |= ( *( reinterpret_cast< uint32* >( &dw ) & 0x0000FF00 ) << 8 );
	temp |= ( *( reinterpret_cast< uint32* >( &dw ) & 0x000000FF ) << 24 );

	return *reinterpret_cast< DataType* >( &temp );
}

#define WordSwap																								WordSwapC
#define DWordSwap																								DWordSwapC

#if !defined( LITTLE_ENDIAN )
#define LITTLE_ENDIAN																						( 1 )
#endif // !LITTLE_ENDIAN

#if defined( LITTLE_ENDIAN )
#define BigShort( val )																					( WordSwap( val ) )
#define BigWord( val )																					( WordSwap( val ) )
#define BigLong( val )																					( DWordSwap( val ) )
#define BigDWord( val )																					( DWordSwap( val ) )
#define BigFloat( val )																					( DWordSwap( val ) )
#define LittleShort( val )																			( val )
#define LittleWord( val )																				( val )
#define LittleLong( val )																				( val )
#define LittleDWord( val )																			( val )
#define LittleFloat( val )																			( val )
#endif // LITTLE_ENDIAN

template< typename DataType >
FORCEINLINE void Construct( DataType* pMemory )
{
	::new( pMemory ) DataType;
}

template< typename DataType >
FORCEINLINE void CopyConstruct( DataType* pMemory, const DataType& src )
{
	::new( pMemory ) DataType( src );
}

template< typename DataType >
FORCEINLINE void Destruct( DataType* pMemory )
{
	pMemory->~DataType();
}