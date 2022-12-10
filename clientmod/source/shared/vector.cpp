#include "vector.hpp"
#include "matrix.hpp"
#include "qangle.hpp"

// 
// [ Vector ] implementation
// 
Vector::Vector()
	: x( 0.f )
	, y( 0.f )
	, z( 0.f )
{ }

Vector::Vector( const Vector& vector )
	: x( vector.x )
	, y( vector.y )
	, z( vector.z )
{ }

Vector::Vector( const float* vector )
	: x( vector[ 0 ] )
	, y( vector[ 1 ] )
	, z( vector[ 2 ] )
{ }

Vector::Vector( float vector_x, float vector_y, float vector_z )
	: x( vector_x )
	, y( vector_y )
	, z( vector_z )
{ }

void Vector::Init( float vector_x /*= 0.f*/, float vector_y /*= 0.f*/, float vector_z /*= 0.f*/ )
{
	x = vector_x;
	y = vector_y;
	z = vector_z;
}

float* Vector::Base()
{
	return &x;
}

const float* Vector::Base() const
{
	return &x;
}

void Vector::Zero()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

void Vector::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

bool Vector::IsValid() const
{
	return ( IsFinite( x ) &&
					 IsFinite( y ) &&
					 IsFinite( z ) );
}

bool Vector::IsZero( float tolerance /*= 0.01f*/ ) const
{
	return ( x > -tolerance && x < tolerance&&
					 y > -tolerance && y < tolerance&&
					 z > -tolerance && z < tolerance );
}

bool Vector::IsZeroFast() const
{
	return ( *reinterpret_cast< const int* >( &x ) == 0 &&
					 *reinterpret_cast< const int* >( &y ) == 0 &&
					 *reinterpret_cast< const int* >( &z ) == 0 );
}

bool Vector::IsLengthLessThan( float value ) const
{
	return ( LengthSqr() < Square( value ) );
}

bool Vector::IsLengthGreaterThan( float value ) const
{
	return ( LengthSqr() > Square( value ) );
}

float Vector::Dot( const Vector& vector ) const
{
	return ( x * vector[ 0 ] +
					 y * vector[ 1 ] +
					 z * vector[ 2 ] );
}

float Vector::Length() const
{
	return FastSqrt( LengthSqr() );
}

float Vector::LengthSqr() const
{
	return ( Square( x ) + Square( y ) + Square( z ) );
}

float Vector::Length2D() const
{
	return FastSqrt( Length2DSqr() );
}

float Vector::Length2DSqr() const
{
	return ( Square( x ) + Square( y ) );
}

float Vector::DistTo( const Vector& vector ) const
{
	return ( *this - vector ).Length();
}

float Vector::DistToSqr( const Vector& vector ) const
{
	return ( *this - vector ).LengthSqr();
}

float Vector::Normalize()
{
	constexpr auto epsilon = std::numeric_limits< float >::epsilon();

	const auto length = Length();
	const auto length_normal = ( 1.f / ( length + epsilon ) );

	x *= length_normal;
	y *= length_normal;
	z *= length_normal;

	return length;
}

void Vector::NormalizeFast()
{
	constexpr auto epsilon = std::numeric_limits< float >::epsilon();

	const auto length_normal = FastRSqrtFast( LengthSqr() + epsilon );

	x *= length_normal;
	y *= length_normal;
	z *= length_normal;
}

Vector Vector::Cross( const Vector& vector ) const
{
	return { ( y * vector[ 2 ] ) - ( z * vector[ 1 ] ),
					 ( z * vector[ 0 ] ) - ( x * vector[ 2 ] ),
					 ( x * vector[ 1 ] ) - ( y * vector[ 0 ] ) };
}

Vector Vector::Transform( const matrix3x4_t& transform ) const
{
	return { Dot( transform[ 0 ] ) + transform[ 0 ][ 3 ],
					 Dot( transform[ 1 ] ) + transform[ 1 ][ 3 ],
					 Dot( transform[ 2 ] ) + transform[ 2 ][ 3 ] };
}

QAngle Vector::ToEulerAngles() const
{
	QAngle angles = { };

	if( x == 0.f && y == 0.f )
	{
		if( z > 0.f )
		{
			angles[ 0 ] = 270.f;
		}
		else
		{
			angles[ 0 ] = 90.f;
		}

		angles[ 1 ] = 0.f;
		angles[ 2 ] = 0.f;
	}
	else
	{
		angles[ 0 ] = RAD2DEG( std::atan2( -z, Length2D() ) );

		if( angles[ 0 ] < 0.f )
		{
			angles[ 0 ] += 360.f;
		}

		angles[ 1 ] = RAD2DEG( std::atan2( y, x ) );

		if( angles[ 1 ] < 0.f )
		{
			angles[ 1 ] += 360.f;
		}

		angles[ 2 ] = 0.f;
	}

	return angles;
}

QAngle Vector::ToEulerAngles( const Vector& pseudoup ) const
{
	QAngle angles = { };

	auto left = pseudoup.Cross( *this );
	left.Normalize();

	const auto length = Length2D();

	if( length > 0.001f )
	{
		angles[ 0 ] = RAD2DEG( std::atan2( -z, length ) );
		angles[ 1 ] = RAD2DEG( std::atan2( y, x ) );

		const auto up_z = ( ( left[ 1 ] * x ) - ( left[ 0 ] * y ) );

		angles[ 2 ] = RAD2DEG( std::atan2( left[ 1 ], up_z ) );
	}
	else
	{
		angles[ 0 ] = RAD2DEG( std::atan2( -z, length ) );
		angles[ 1 ] = RAD2DEG( std::atan2( -left[ 0 ], left[ 1 ] ) );
		angles[ 2 ] = 0.f;
	}

	return angles;
}

float& Vector::operator[]( int index )
{
	return Base()[ index ];
}

float Vector::operator[]( int index ) const
{
	return Base()[ index ];
}

Vector& Vector::operator=( const Vector& vector )
{
	x = vector.x;
	y = vector.y;
	z = vector.z;

	return ( *this );
}

Vector& Vector::operator+=( const Vector& vector )
{
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return ( *this );
}

Vector& Vector::operator-=( const Vector& vector )
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return ( *this );
}

Vector& Vector::operator*=( const Vector& vector )
{
	x *= vector.x;
	y *= vector.y;
	z *= vector.z;

	return ( *this );
}

Vector& Vector::operator/=( const Vector& vector )
{
	x /= vector.x;
	y /= vector.y;
	z /= vector.z;

	return ( *this );
}

Vector& Vector::operator+=( float value )
{
	x += value;
	y += value;
	z += value;

	return ( *this );
}

Vector& Vector::operator-=( float value )
{
	x -= value;
	y -= value;
	z -= value;

	return ( *this );
}

Vector& Vector::operator*=( float value )
{
	x *= value;
	y *= value;
	z *= value;

	return ( *this );
}

Vector& Vector::operator/=( float value )
{
	x /= value;
	y /= value;
	z /= value;

	return ( *this );
}

Vector Vector::operator+( const Vector& vector ) const
{
	return { x + vector.x,
					 y + vector.y,
					 z + vector.z };
}

Vector Vector::operator-( const Vector& vector ) const
{
	return { x - vector.x,
					 y - vector.y,
					 z - vector.z };
}

Vector Vector::operator*( const Vector& vector ) const
{
	return { x * vector.x,
					 y * vector.y,
					 z * vector.z };
}

Vector Vector::operator/( const Vector& vector ) const
{
	return { x / vector.x,
					 y / vector.y,
					 z / vector.z };
}

Vector Vector::operator+( float value ) const
{
	return { x + value,
					 y + value,
					 z + value };
}

Vector Vector::operator-( float value ) const
{
	return { x - value,
					 y - value,
					 z - value };
}

Vector Vector::operator*( float value ) const
{
	return { x * value,
					 y * value,
					 z * value };
}

Vector Vector::operator/( float value ) const
{
	return { x / value,
					 y / value,
					 z / value };
}

// 
// [ VectorAligned ] implementation
// 
VectorAligned::VectorAligned()
	: Vector()
	, w( 0.f )
{ }

VectorAligned::VectorAligned( float vector_x, float vector_y, float vector_z )
	: Vector( vector_x, vector_y, vector_z )
	, w( 0.f )
{ }

VectorAligned::VectorAligned( const Vector& vector )
	: Vector( vector )
	, w( 0.f )
{ }

VectorAligned& VectorAligned::operator=( const Vector& vector )
{
	x = vector.x;
	y = vector.y;
	z = vector.z;

	return ( *this );
}

VectorAligned& VectorAligned::operator=( const VectorAligned& vector )
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;

	return ( *this );
}

float DotProduct( const Vector& source, const Vector& destination )
{
	return ( source[ 0 ] * destination[ 0 ] +
					 source[ 1 ] * destination[ 1 ] +
					 source[ 2 ] * destination[ 2 ] );
}

void CrossProduct( const Vector& source, const Vector& destination, Vector& output )
{
	output[ 0 ] = ( ( source[ 1 ] * destination[ 2 ] ) - ( source[ 2 ] * destination[ 1 ] ) );
	output[ 1 ] = ( ( source[ 2 ] * destination[ 0 ] ) - ( source[ 0 ] * destination[ 2 ] ) );
	output[ 2 ] = ( ( source[ 0 ] * destination[ 1 ] ) - ( source[ 1 ] * destination[ 0 ] ) );
}

float VectorLength( const Vector& vector )
{
	return std::sqrt( Square( vector[ 0 ] ) +
										Square( vector[ 1 ] ) +
										Square( vector[ 2 ] ) );
}

float API_FASTCALL VectorNormalize( Vector& vector )
{
	const auto length = VectorLength( vector );
	const auto length_normal = ( 1.f / ( length + FLT_EPSILON ) );

	vector[ 0 ] *= length_normal;
	vector[ 1 ] *= length_normal;
	vector[ 2 ] *= length_normal;

	return length;
}

void API_FASTCALL VectorNormalizeFast( Vector& vector )
{
	const auto length_normal = ( 1.f / ( VectorLength( vector ) + FLT_EPSILON ) );

	vector[ 0 ] *= length_normal;
	vector[ 1 ] *= length_normal;
	vector[ 2 ] *= length_normal;
}

void VectorTransform( const Vector& vector, const matrix3x4_t& transform, Vector& output )
{
	output[ 0 ] = DotProduct( vector, transform[ 0 ] ) + transform[ 0 ][ 3 ];
	output[ 1 ] = DotProduct( vector, transform[ 1 ] ) + transform[ 1 ][ 3 ];
	output[ 2 ] = DotProduct( vector, transform[ 2 ] ) + transform[ 2 ][ 3 ];
}

void VectorAngles( const Vector& forward, QAngle& angles )
{
	if( forward[ 0 ] == 0.f && forward[ 1 ] == 0.f )
	{
		if( forward[ 2 ] > 0.f )
		{
			angles[ 0 ] = 270.f;
		}
		else
		{
			angles[ 0 ] = 90.f;
		}

		angles[ 1 ] = 0.f;
		angles[ 2 ] = 0.f;
	}
	else
	{
		angles[ 0 ] = RAD2DEG( std::atan2( -forward[ 2 ], forward.Length2D() ) );

		if( angles[ 0 ] < 0.f )
		{
			angles[ 0 ] += 360.f;
		}

		angles[ 1 ] = RAD2DEG( std::atan2( forward[ 1 ], forward[ 0 ] ) );

		if( angles[ 1 ] < 0.f )
		{
			angles[ 1 ] += 360.f;
		}

		angles[ 2 ] = 0.f;
	}
}

void VectorAngles( const Vector& forward, const Vector& pseudoup, QAngle& angles )
{
	Vector left = { };
	CrossProduct( pseudoup, forward, left );
	VectorNormalizeFast( left );

	const auto length = forward.Length2D();

	if( length > 0.001f )
	{
		angles[ 0 ] = RAD2DEG( std::atan2( -forward[ 2 ], length ) );
		angles[ 1 ] = RAD2DEG( std::atan2( forward[ 1 ], forward[ 0 ] ) );

		const auto up_z = ( ( left[ 1 ] * forward[ 0 ] ) - ( left[ 0 ] * forward[ 1 ] ) );

		angles[ 2 ] = RAD2DEG( std::atan2( left[ 1 ], up_z ) );
	}
	else
	{
		angles[ 0 ] = RAD2DEG( std::atan2( -forward[ 2 ], length ) );
		angles[ 1 ] = RAD2DEG( std::atan2( -left[ 0 ], left[ 1 ] ) );
		angles[ 2 ] = 0.f;
	}
}