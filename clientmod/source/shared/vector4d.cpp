#include "vector4d.hpp"

Vector4D::Vector4D()
	: x( 0.f )
	, y( 0.f )
	, z( 0.f )
	, w( 0.f )
{ }

Vector4D::Vector4D( const Vector4D& vector )
	: x( vector.x )
	, y( vector.y )
	, z( vector.z )
	, w( vector.w )
{ }

Vector4D::Vector4D( float vector_x, float vector_y, float vector_z, float vector_w )
	: x( vector_x )
	, y( vector_y )
	, z( vector_z )
	, w( vector_w )
{ }

float& Vector4D::operator[]( int index )
{
	return ( &x )[ index ];
}

float Vector4D::operator[]( int index ) const
{
	return ( &x )[ index ];
}

Vector4D& Vector4D::operator=( const Vector4D& vector )
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;

	return ( *this );
}