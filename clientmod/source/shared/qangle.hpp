#pragma once

#include "mathlib.hpp"
#include "math_pfns.hpp"

class QAngle
{
public:
	QAngle();
	QAngle( const QAngle& angles );
	QAngle( const float* angles );
	QAngle( float angles_x, float angles_y, float angles_z );

public:
	void Init( float angles_x = 0.f, float angles_y = 0.f, float angles_z = 0.f );

	float* Base();
	const float* Base() const;

	void Zero();
	void Negate();

	bool IsValid() const;

	void Normalize();
	void Clamp();

	Vector ToVector() const;
	Vector ToVectors( Vector* right = nullptr, Vector* up = nullptr ) const;

public:
	float& operator[]( int index );
	float operator[]( int index ) const;

public:
	QAngle& operator=( const QAngle& angles );

public:
	QAngle& operator+=( const QAngle& angles );
	QAngle& operator-=( const QAngle& angles );
	QAngle& operator*=( const QAngle& angles );
	QAngle& operator/=( const QAngle& angles );

	QAngle& operator+=( float value );
	QAngle& operator-=( float value );
	QAngle& operator*=( float value );
	QAngle& operator/=( float value );

public:
	QAngle operator+( const QAngle& angles ) const;
	QAngle operator-( const QAngle& angles ) const;
	QAngle operator*( const QAngle& angles ) const;
	QAngle operator/( const QAngle& angles ) const;

	QAngle operator+( float value ) const;
	QAngle operator-( float value ) const;
	QAngle operator*( float value ) const;
	QAngle operator/( float value ) const;

public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};
// sizeof( QAngle ) = 0x000C ( 12 )

FORCEINLINE QAngle operator*( float value, const QAngle& angles )
{
	return ( value * angles );
}

extern const QAngle vec3_angle;

float AngleDiff( float destination, float source );
float AngleDistance( float next, float current );
float AngleNormalize( float angle );

void AngleVectors( const QAngle& angles, Vector* forward );
void AngleVectors( const QAngle& angles, Vector* forward, Vector* right, Vector* up );