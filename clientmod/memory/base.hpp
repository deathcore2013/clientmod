#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#undef FIELD
#undef FIELD_PAD
#undef FIELD_OFFSET
#undef FIELD_BASE

#undef RTL_NUMBER_OF

#if !defined( FIELD )
#define FIELD( DataType, Name, Offset )								DataType& Name() { return *( DataType* )( memory::ToUIntPointer( this ) + Offset ); }
#endif // !FIELD

#if !defined( FIELD_PAD )
#define FIELD_PAD( NumberOfBytes )										std::uint8_t JOIN( __pad, __COUNTER__ )[ NumberOfBytes ] = { }
#endif // !FIELD_PAD

#if !defined( FIELD_OFFSET )
#define FIELD_OFFSET( DataType, Field )								( ( std::int32_t )( std::intptr_t )&( ( ( DataType* )0 )->Field ) )
#endif // !FIELD_OFFSET

#if !defined( FIELD_BASE )
#define FIELD_BASE( Pointer, DataType, Field )				( ( DataType* )( ( std::int8_t* )( Pointer ) - FIELD_OFFSET( DataType, Field ) ) )
#endif // !FIELD_BASE

#if !defined( RTL_NUMBER_OF )
#define RTL_NUMBER_OF( Array )												( sizeof( Array ) / sizeof( ( Array )[ 0 ] ) )
#endif // !RTL_NUMBER_OF

namespace atom::memory
{

#if defined( _M_IX86 )
constexpr std::uintptr_t MinimumUserModeAddress = 0x00010000;
constexpr std::uintptr_t MaximumUserModeAddress = 0x7FFEFFFF;
#elif defined( _M_X64 )
constexpr std::uintptr_t MinimumUserModeAddress = 0x0000000000010000;
constexpr std::uintptr_t MaximumUserModeAddress = 0x00007FFFFFFEFFFF;
#endif // _M_IX86

} // namespace atom::memory