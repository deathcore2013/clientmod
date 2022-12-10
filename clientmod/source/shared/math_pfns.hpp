#pragma once

#include "../../include/auto.hpp"
#include "../../include/base.hpp"
#include "../../include/win32.hpp"

#include "../tier0/basetypes.hpp"
#include "../tier0/commonmacros.hpp"
#include "../tier0/dbg.hpp"
#include "../tier0/platform.hpp"

FORCEINLINE float API_VECTORCALL FastSqrt( float x )
{
	const auto root = _mm_sqrt_ss( _mm_load_ss( &x ) );
	return root.m128_f32[ 0 ];
}

FORCEINLINE float API_VECTORCALL FastRSqrtFast( float x )
{
	const auto rroot = _mm_rsqrt_ss( _mm_load_ss( &x ) );
	return rroot.m128_f32[ 0 ];
}

FORCEINLINE float API_VECTORCALL FastRSqrt( float x )
{
	const auto rroot = FastRSqrtFast( x );
	return ( ( 0.5f * rroot ) * ( 3.f - ( x * rroot ) * rroot ) );
}