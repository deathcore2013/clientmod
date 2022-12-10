#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#if !defined( IMGUI_DEFINE_MATH_OPERATORS )
#define IMGUI_DEFINE_MATH_OPERATORS
#endif // !IMGUI_DEFINE_MATH_OPERATORS

#include "../imgui/imgui.hpp"
#include "../imgui/imgui_internal.hpp"

namespace atom::graphics
{

class Color
{
protected:
	static constexpr float FromInteger( int code );

public:
	Color();
	Color( int red, int green, int blue, int alpha = 255 );
	Color( float red, float green, float blue, float alpha = 1.f );

public:
	Color( const Color& color );

public:
	Color& operator=( const Color& color );

public:
	float operator[]( std::size_t index ) const;
	float& operator[]( std::size_t index );

public:
	operator ImColor() const;
	operator ImU32() const;

public:
	static const Color Transparent;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Orange;
	static const Color Cyan;
	static const Color Grey;

protected:
	float m_red = 0.f;
	float m_green = 0.f;
	float m_blue = 0.f;
	float m_alpha = 0.f;
};

} // namespace atom::graphics