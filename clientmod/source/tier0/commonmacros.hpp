#pragma once

#include "../../include/auto.hpp"
#include "../../include/base.hpp"
#include "../../include/win32.hpp"

#include "../../memory/base.hpp"

#define SETBITS( iBitVector, bits )															( ( iBitVector ) |= ( bits ) )
#define CLEARBITS( iBitVector, bits )														( ( iBitVector ) &= ~( bits ) )
#define FBitSet( iBitVector, bits )															( ( iBitVector ) & ( bits ) )

FORCEINLINE bool IsPowerOfTwo( int value )
{
	return ( ( value & ( value - 1 ) ) == 0 );
}