#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#if !defined( ENUM_OPERATOR )
#define ENUM_OPERATOR( EnumType )																																																																											\
	FORCEINLINE EnumType& operator|=( EnumType& left, EnumType right ) { return left = EnumType( ( __underlying_type( EnumType ) )left | ( __underlying_type( EnumType ) )right ); }			\
	FORCEINLINE EnumType& operator&=( EnumType& left, EnumType right ) { return left = EnumType( ( __underlying_type( EnumType ) )left & ( __underlying_type( EnumType ) )right ); }			\
	FORCEINLINE EnumType& operator^=( EnumType& left, EnumType right ) { return left = EnumType( ( __underlying_type( EnumType ) )left ^ ( __underlying_type( EnumType ) )right ); }			\
	FORCEINLINE constexpr EnumType operator|( EnumType left, EnumType right ) { return EnumType( ( __underlying_type( EnumType ) )left | ( __underlying_type( EnumType ) )right ); }			\
	FORCEINLINE constexpr EnumType operator&( EnumType left, EnumType right ) { return EnumType( ( __underlying_type( EnumType ) )left & ( __underlying_type( EnumType ) )right ); }			\
	FORCEINLINE constexpr EnumType operator^( EnumType left, EnumType right ) { return EnumType( ( __underlying_type( EnumType ) )left ^ ( __underlying_type( EnumType ) )right ); }			\
	FORCEINLINE constexpr EnumType operator~( EnumType data ) { return EnumType( ~( __underlying_type( EnumType ) )data ); }																															\
	FORCEINLINE constexpr bool operator!( EnumType data ) { return !( __underlying_type( EnumType ) )data; }
#endif // !ENUM_OPERATOR

namespace atom::core
{

template< typename EnumType >
FORCEINLINE bool HasAllFlags( EnumType data, EnumType contains )
{
	return ( ( ( __underlying_type( EnumType ) )data ) & ( __underlying_type( EnumType ) )contains ) == ( ( __underlying_type( EnumType ) )contains );
}

template< typename EnumType >
FORCEINLINE bool HasAnyFlags( EnumType data, EnumType contains )
{
	return ( ( ( __underlying_type( EnumType ) )data ) & ( __underlying_type( EnumType ) )contains ) != 0;
}

} // namespace atom::core