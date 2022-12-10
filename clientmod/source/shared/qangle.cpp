#include "qangle.hpp"
#include "vector.hpp"

QAngle::QAngle()
	: x( 0.f )
	, y( 0.f )
	, z( 0.f )
{ }

QAngle::QAngle( const QAngle& angles )
	: x( angles.x )
	, y( angles.y )
	, z( angles.z )
{ }

QAngle::QAngle( const float* angles )
	: x( angles[ 0 ] )
	, y( angles[ 1 ] )
	, z( angles[ 2 ] )
{ }

QAngle::QAngle( float angles_x, float angles_y, float angles_z )
	: x( angles_x )
	, y( angles_y )
	, z( angles_z )
{ }

void QAngle::Init( float angles_x /*= 0.f*/, float angles_y /*= 0.f*/, float angles_z /*= 0.f*/ )
{
	x = angles_x;
	y = angles_z;
	z = angles_z;
}

float* QAngle::Base()
{
	return &x;
}

const float* QAngle::Base() const
{
	return &x;
}

void QAngle::Zero()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

void QAngle::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

bool QAngle::IsValid() const
{
	return ( IsFinite( x ) &&
					 IsFinite( y ) &&
					 IsFinite( z ) );
}

void QAngle::Normalize()
{
	x = AngleNormalize( x );
	y = AngleNormalize( y );
	z = AngleNormalize( z );
}

void QAngle::Clamp()
{
	x = std::clamp( x, -89.f, 89.f );
	y = std::clamp( y, -180.f, 180.f );
	z = std::clamp( x, -50.f, 50.f );
}

Vector QAngle::ToVector() const
{
	const auto radians_pitch = DEG2RAD( x );
	const auto radians_yaw = DEG2RAD( y );

	const auto sine_pitch = std::sin( radians_pitch );
	const auto sine_yaw = std::sin( radians_yaw );

	const auto cosine_pitch = std::cos( radians_pitch );
	const auto cosine_yaw = std::cos( radians_yaw );

	return { ( cosine_pitch * cosine_yaw ),
					 ( cosine_pitch * sine_yaw ),
					 ( -sine_pitch ) };
}

Vector QAngle::ToVectors( Vector* right /*= nullptr*/, Vector* up /*= nullptr*/ ) const
{
	const auto radians_pitch = DEG2RAD( x );
	const auto radians_yaw = DEG2RAD( y );
	const auto radians_roll = DEG2RAD( z );

	const auto sine_pitch = std::sin( radians_pitch );
	const auto sine_yaw = std::sin( radians_yaw );
	const auto sine_roll = std::sin( radians_roll );

	const auto cosine_pitch = std::cos( radians_pitch );
	const auto cosine_yaw = std::cos( radians_yaw );
	const auto cosine_roll = std::cos( radians_roll );

	if( right )
	{
		right->x = ( -1.f * sine_roll * sine_pitch * cosine_yaw + -1.f * cosine_roll * -sine_yaw );
		right->y = ( -1.f * sine_roll * sine_pitch * sine_yaw + -1.f * cosine_roll * cosine_yaw );
		right->z = ( -1.f * sine_roll * cosine_pitch );
	}

	if( up )
	{
		up->x = ( cosine_roll * sine_pitch * cosine_yaw + -sine_roll * -sine_yaw );
		up->y = ( cosine_roll * sine_pitch * sine_yaw + -sine_roll * cosine_yaw );
		up->z = ( cosine_roll * cosine_pitch );
	}

	return { ( cosine_pitch * cosine_yaw ),
					 ( cosine_pitch * sine_yaw ),
					 ( -sine_pitch ) };
}

float& QAngle::operator[]( int index )
{
	return Base()[ index ];
}

float QAngle::operator[]( int index ) const
{
	return Base()[ index ];
}

QAngle& QAngle::operator=( const QAngle& angles )
{
	x = angles.x;
	y = angles.y;
	z = angles.z;

	return ( *this );
}

QAngle& QAngle::operator+=( const QAngle& angles )
{
	x += angles.x;
	y += angles.y;
	z += angles.z;

	return ( *this );
}

QAngle& QAngle::operator-=( const QAngle& angles )
{
	x -= angles.x;
	y -= angles.y;
	z -= angles.z;

	return ( *this );
}

QAngle& QAngle::operator*=( const QAngle& angles )
{
	x *= angles.x;
	y *= angles.y;
	z *= angles.z;

	return ( *this );
}

QAngle& QAngle::operator/=( const QAngle& angles )
{
	x /= angles.x;
	y /= angles.y;
	z /= angles.z;

	return ( *this );
}

QAngle& QAngle::operator+=( float value )
{
	x += value;
	y += value;
	z += value;

	return ( *this );
}

QAngle& QAngle::operator-=( float value )
{
	x -= value;
	y -= value;
	z -= value;

	return ( *this );
}

QAngle& QAngle::operator*=( float value )
{
	x *= value;
	y *= value;
	z *= value;

	return ( *this );
}

QAngle& QAngle::operator/=( float value )
{
	x /= value;
	y /= value;
	z /= value;

	return ( *this );
}

QAngle QAngle::operator+( const QAngle& angles ) const
{
	return { x + angles.x,
					 y + angles.y,
					 z + angles.z };
}

QAngle QAngle::operator-( const QAngle& angles ) const
{
	return { x - angles.x,
					 y - angles.y,
					 z - angles.z };
}

QAngle QAngle::operator*( const QAngle& angles ) const
{
	return { x * angles.x,
					 y * angles.y,
					 z * angles.z };
}

QAngle QAngle::operator/( const QAngle& angles ) const
{
	return { x / angles.x,
					 y / angles.y,
					 z / angles.z };
}

QAngle QAngle::operator+( float value ) const
{
	return { x + value,
					 y + value,
					 z + value };
}

QAngle QAngle::operator-( float value ) const
{
	return { x - value,
					 y - value,
					 z - value };
}

QAngle QAngle::operator*( float value ) const
{
	return { x * value,
					 y * value,
					 z * value };
}

QAngle QAngle::operator/( float value ) const
{
	return { x * value,
					 y * value,
					 z * value };
}

float AngleDiff( float destination, float source )
{
	auto distance = std::fmod( destination - source, 360.f );

	if( destination > source )
	{
		if( distance >= 180.f )
		{
			distance -= 360.f;
		}
	}
	else
	{
		if( distance <= -180.f )
		{
			distance += 360.f;
		}
	}

	return distance;
}

float AngleDistance( float next, float current )
{
	auto distance = ( next - current );

	if( distance < -180.f )
	{
		distance += 360.f;
	}
	else if( distance > 180.f )
	{
		distance -= 360.f;
	}

	return distance;
}

float AngleNormalize( float angle )
{
	angle = std::fmod( angle, 360.f );

	if( angle > 180.f )
	{
		angle -= 360.f;
	}

	if( angle < -180.f )
	{
		angle += 360.f;
	}

	return angle;
}

void AngleVectors( const QAngle& angles, Vector* forward )
{
	const auto radians_pitch = DEG2RAD( angles[ 0 ] );
	const auto radians_yaw = DEG2RAD( angles[ 1 ] );

	const auto sine_pitch = std::sin( radians_pitch );
	const auto sine_yaw = std::sin( radians_yaw );

	const auto cosine_pitch = std::cos( radians_pitch );
	const auto cosine_yaw = std::cos( radians_yaw );

	if( forward )
	{
		forward->x = ( cosine_pitch * cosine_yaw );
		forward->y = ( cosine_pitch * sine_yaw );
		forward->z = ( -sine_pitch );
	}
}

void AngleVectors( const QAngle& angles, Vector* forward, Vector* right, Vector* up )
{
	const auto radians_pitch = DEG2RAD( angles[ 0 ] );
	const auto radians_yaw = DEG2RAD( angles[ 1 ] );
	const auto radians_roll = DEG2RAD( angles[ 2 ] );

	const auto sine_pitch = std::sin( radians_pitch );
	const auto sine_yaw = std::sin( radians_yaw );
	const auto sine_roll = std::sin( radians_roll );

	const auto cosine_pitch = std::cos( radians_pitch );
	const auto cosine_yaw = std::cos( radians_yaw );
	const auto cosine_roll = std::cos( radians_roll );

	if( forward )
	{
		forward->x = ( cosine_pitch * cosine_yaw );
		forward->y = ( cosine_pitch * sine_yaw );
		forward->z = ( -sine_pitch );
	}

	if( right )
	{
		right->x = ( -1.f * sine_roll * sine_pitch * cosine_yaw + -1.f * cosine_roll * -sine_yaw );
		right->y = ( -1.f * sine_roll * sine_pitch * sine_yaw + -1.f * cosine_roll * cosine_yaw );
		right->z = ( -1.f * sine_roll * cosine_pitch );
	}

	if( up )
	{
		up->x = ( cosine_roll * sine_pitch * cosine_yaw + -sine_roll * -sine_yaw );
		up->y = ( cosine_roll * sine_pitch * sine_yaw + -sine_roll * cosine_yaw );
		up->z = ( cosine_roll * cosine_pitch );
	}
}