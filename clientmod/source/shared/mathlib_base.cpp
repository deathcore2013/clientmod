#include "mathlib.hpp"
#include "vector.hpp"
#include "qangle.hpp"

const Vector vec3_origin( 0.f, 0.f, 0.f );
const QAngle vec3_angle( 0.f, 0.f, 0.f );
const Vector vec3_invalid( FLT_MAX, FLT_MAX, FLT_MAX );

void SinCos( float radians, float* sine, float* cosine )
{
	( *sine ) = std::sin( radians );
	( *cosine ) = std::cos( radians );
}