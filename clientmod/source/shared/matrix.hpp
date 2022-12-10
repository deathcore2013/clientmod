#pragma once

#include "mathlib.hpp"
#include "math_pfns.hpp"

struct matrix3x4_t
{
public:
	matrix3x4_t();
	matrix3x4_t( const matrix3x4_t& matrix );

public:
	float* Base();
	const float* Base() const;

public:
	float* operator[]( int index );
	const float* operator[]( int index ) const;

public:
	matrix3x4_t& operator=( const matrix3x4_t& matrix );

public:
	float m_flMatVal[ 3 ][ 4 ] = { };										// 0x0000
};
// sizeof( matrix3x4_t ) = 0x0030 ( 48 )

class VMatrix
{
public:
	VMatrix();
	VMatrix( const VMatrix& matrix );

public:
	float* Base();
	const float* Base() const;

public:
	float* operator[]( int index );
	const float* operator[]( int index ) const;

public:
	VMatrix& operator=( const VMatrix& matrix );

public:
	float m[ 4 ][ 4 ] = { };														// 0x0000
};
// sizeof( VMatrix ) = 0x0040 ( 64 )

void MatrixGetColumn( const matrix3x4_t& in, int column, Vector& out );
void MatrixSetColumn( const Vector& in, int column, matrix3x4_t& out );