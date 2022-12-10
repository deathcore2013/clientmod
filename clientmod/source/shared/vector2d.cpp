#include "vector2d.hpp"

Vector2D::Vector2D()
	: x( 0.f )
	, y( 0.f )
{ }

Vector2D::Vector2D( const Vector2D& vector )
	: x( vector.x )
	, y( vector.y )
{ }

Vector2D::Vector2D( float vector_x, float vector_y )
	: x( vector_x )
	, y( vector_y )
{ }

float& Vector2D::operator[]( int index )
{
	return ( &x )[ index ];
}

float Vector2D::operator[]( int index ) const
{
	return ( &x )[ index ];
}

Vector2D& Vector2D::operator=( const Vector2D& vector )
{
	x = vector.x;
	y = vector.y;

	return ( *this );
}