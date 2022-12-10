#include "color.hpp"

namespace atom::graphics
{

constexpr float Color::FromInteger( int code )
{
	return static_cast< float >( code ) / 255.f;
}

Color::Color()
	: m_red( 0.f )
	, m_green( 0.f )
	, m_blue( 0.f )
	, m_alpha( 0.f )
{ }

Color::Color( int red, int green, int blue, int alpha /*= 255*/ )
	: m_red( FromInteger( red ) )
	, m_green( FromInteger( green ) )
	, m_blue( FromInteger( blue ) )
	, m_alpha( FromInteger( alpha ) )
{ }

Color::Color( float red, float green, float blue, float alpha /*= 1.f*/ )
	: m_red( red )
	, m_green( green )
	, m_blue( blue )
	, m_alpha( alpha )
{ }

Color::Color( const Color& color )
	: m_red( color.m_red )
	, m_green( color.m_green )
	, m_blue( color.m_blue )
	, m_alpha( color.m_alpha )
{ }

Color& Color::operator=( const Color& color )
{
	if( std::addressof( color ) != this )
	{
		m_red = color.m_red;
		m_green = color.m_green;
		m_blue = color.m_blue;
		m_alpha = color.m_alpha;
	}

	return *this;
}

float Color::operator[]( std::size_t index ) const
{
	return ( &m_red )[ index ];
}

float& Color::operator[]( std::size_t index )
{
	return ( &m_red )[ index ];
}

Color::operator ImColor() const
{
	return { m_red, m_green, m_blue, m_alpha };
}

Color::operator ImU32() const
{
	return ImGui::ColorConvertFloat4ToU32( { m_red, m_green, m_blue, m_alpha } );
}

const Color Color::Transparent( 0.f, 0.f, 0.f, 0.f );
const Color Color::Black( 0.f, 0.f, 0.f );
const Color Color::White( 1.f, 1.f, 1.f );
const Color Color::Red( 1.f, 0.f, 0.f );
const Color Color::Green( 0.f, 1.f, 0.f );
const Color Color::Blue( 0.f, 0.f, 1.f );
const Color Color::Yellow( 1.f, 1.f, 0.f );
const Color Color::Orange( 1.f, 0.5f, 0.f );
const Color Color::Cyan( 0.f, 0.5f, 1.f );
const Color Color::Grey( 0.5f, 0.5f, 0.5f );

} // namespace atom::graphics