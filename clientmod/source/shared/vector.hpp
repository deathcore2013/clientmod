#pragma once

#include "mathlib.hpp"
#include "math_pfns.hpp"

class Vector
{
public:
	Vector();
	Vector( const Vector& vector );
	Vector( const float* vector );
	Vector( float vector_x, float vector_y, float vector_z );

public:
	void Init( float vector_x = 0.f, float vector_y = 0.f, float vector_z = 0.f );

	float* Base();
	const float* Base() const;

	void Zero();
	void Negate();

	bool IsValid() const;

	bool IsZero( float tolerance = 0.01f ) const;
	bool IsZeroFast() const;

	bool IsLengthLessThan( float value ) const;
	bool IsLengthGreaterThan( float value ) const;

	float Dot( const Vector& vector ) const;

	float Length() const;
	float LengthSqr() const;

	float Length2D() const;
	float Length2DSqr() const;

	float DistTo( const Vector& vector ) const;
	float DistToSqr( const Vector& vector ) const;

	float Normalize();
	void NormalizeFast();

	Vector Cross( const Vector& vector ) const;
	Vector Transform( const matrix3x4_t& transform ) const;

	QAngle ToEulerAngles() const;
	QAngle ToEulerAngles( const Vector& pseudoup ) const;


public:
	float& operator[]( int index );
	float operator[]( int index ) const;

public:
	Vector& operator=( const Vector& vector );

public:
	Vector& operator+=( const Vector& vector );
	Vector& operator-=( const Vector& vector );
	Vector& operator*=( const Vector& vector );
	Vector& operator/=( const Vector& vector );

	Vector& operator+=( float value );
	Vector& operator-=( float value );
	Vector& operator*=( float value );
	Vector& operator/=( float value );

public:
	Vector operator+( const Vector& vector ) const;
	Vector operator-( const Vector& vector ) const;
	Vector operator*( const Vector& vector ) const;
	Vector operator/( const Vector& vector ) const;

	Vector operator+( float value ) const;
	Vector operator-( float value ) const;
	Vector operator*( float value ) const;
	Vector operator/( float value ) const;

public:
	float x = 0.f;																			// 0x0000
	float y = 0.f;																			// 0x0004
	float z = 0.f;																			// 0x0008
};
// sizeof( Vector ) = 0x000C ( 12 )

FORCEINLINE Vector operator*( float value, const Vector& vector )
{
	return ( value * vector );
}

class VectorAligned : public Vector
{
public:
	VectorAligned();
	VectorAligned( float vector_x, float vector_y, float vector_z );

public:
	explicit VectorAligned( const Vector& vector );

public:
	VectorAligned& operator=( const Vector& vector );
	VectorAligned& operator=( const VectorAligned& vector );

public:
	float w = 0.f;																			// 0x000C
};
// sizeof( VectorAligned ) = 0x0010 ( 16 )

extern const Vector vec3_origin;
extern const Vector vec3_invalid;

float DotProduct( const Vector& source, const Vector& destination );

void CrossProduct( const Vector& source, const Vector& destination, Vector& output );

float VectorLength( const Vector& vector );

float API_FASTCALL VectorNormalize( Vector& vector );
void API_FASTCALL VectorNormalizeFast( Vector& vector );

void VectorTransform( const Vector& vector, const matrix3x4_t& transform, Vector& output );

void VectorAngles( const Vector& forward, QAngle& angles );
void VectorAngles( const Vector& forward, const Vector& pseudoup, QAngle& angles );