#include "matrix.hpp"
#include "vector.hpp"

// 
// [ matrix3x4_t ] implementation
// 
matrix3x4_t::matrix3x4_t()
{
	std::memset( Base(), 0, sizeof( matrix3x4_t ) );
}

matrix3x4_t::matrix3x4_t( const matrix3x4_t& matrix )
{
	std::memcpy( Base(), matrix.Base(), sizeof( matrix3x4_t ) );
}

float* matrix3x4_t::Base()
{
	return &m_flMatVal[ 0 ][ 0 ];
}

const float* matrix3x4_t::Base() const
{
	return &m_flMatVal[ 0 ][ 0 ];
}

float* matrix3x4_t::operator[]( int index )
{
	return m_flMatVal[ index ];
}

const float* matrix3x4_t::operator[]( int index ) const
{
	return m_flMatVal[ index ];
}

matrix3x4_t& matrix3x4_t::operator=( const matrix3x4_t& matrix )
{
	std::memcpy( Base(), matrix.Base(), sizeof( matrix3x4_t ) );
	return ( *this );
}

// 
// [ VMatrix ] implementation
// 
VMatrix::VMatrix()
{
	std::memset( Base(), 0, sizeof( VMatrix ) );
}

VMatrix::VMatrix( const VMatrix& matrix )
{
	std::memcpy( Base(), matrix.Base(), sizeof( VMatrix ) );
}

float* VMatrix::Base()
{
	return &m[ 0 ][ 0 ];
}

const float* VMatrix::Base() const
{
	return &m[ 0 ][ 0 ];
}

float* VMatrix::operator[]( int index )
{
	return m[ index ];
}

const float* VMatrix::operator[]( int index ) const
{
	return m[ index ];
}

VMatrix& VMatrix::operator=( const VMatrix& matrix )
{
	std::memcpy( Base(), matrix.Base(), sizeof( VMatrix ) );
	return ( *this );
}

void MatrixGetColumn( const matrix3x4_t& in, int column, Vector& out )
{
	out[ 0 ] = in[ 0 ][ column ];
	out[ 1 ] = in[ 1 ][ column ];
	out[ 2 ] = in[ 2 ][ column ];
}

void MatrixSetColumn( const Vector& in, int column, matrix3x4_t& out )
{
	out[ 0 ][ column ] = in[ 0 ];
	out[ 1 ][ column ] = in[ 1 ];
	out[ 2 ][ column ] = in[ 2 ];
}