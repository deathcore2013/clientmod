#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "../core/enum_operator.hpp"
#include "../core/no_copy.hpp"
#include "../core/no_move.hpp"
#include "../core/singleton.hpp"

#include "color.hpp"

#include "../imgui/backends/imgui_impl_dx9.hpp"
#include "../imgui/backends/imgui_impl_win32.hpp"

namespace atom::graphics
{

enum class TextLayout
{
	Left = 0x00000000,
	Right = 0x00000001,
	CenterV = 0x00000002,
	CenterH = 0x00000004,
	Center = ( CenterV | CenterH ),
};

ENUM_OPERATOR( TextLayout );

class Renderer : public core::Singleton< Renderer >
{
public:
	~Renderer();

public:
	bool Create( IDirect3DDevice9* device );
	void Destroy();

	bool Begin();
	void End();

	void Lost();
	bool Reset();

public:
	IDirect3DDevice9* GetDevice() const;
	HWND GetWindow() const;

	const D3DDEVICE_CREATION_PARAMETERS& GetCreationParameters() const;
	const D3DVIEWPORT9& GetViewport() const;

public:
	void DrawCircle( const ImVec2& center_screen, float radius, const Color& color, int segments = 12 );
	void DrawLine( const ImVec2& begin, const ImVec2& end, const Color& color );
	void DrawRect( const ImVec2& position, const ImVec2& size, const Color& color );

	void DrawBox( const ImVec2& position, const ImVec2& size, float thickness, const Color& color );
	void DrawBoxBorder( const ImVec2& position, const ImVec2& size, float thickness, const Color& color, const Color& border = Color::Black );

	void DrawCornerBox( const ImVec2& position, const ImVec2& size, float thickness, const Color& color );
	void DrawCornerBoxBorder( const ImVec2& position, const ImVec2& size, float thickness, const Color& color, const Color& border = Color::Black );

	void DrawText( const ImVec2& position, TextLayout layout, const Color& color, const char* const format, ... );
	void DrawTextShadow( const ImVec2& position, TextLayout layout, const Color& color, const char* const format, ... );
	void DrawTextBorder( const ImVec2& position, TextLayout layout, const Color& color, const Color& border, const char* const format, ... );

	void DrawTexture( ImTextureID texture_id, const ImVec2& position, const ImVec2& size, const Color& color = Color::White );

protected:
	void DrawTextFormat( ImVec2 position, TextLayout layout, const Color& color, const Color& border, const char* const format, va_list arg_pack );

protected:
	IDirect3DDevice9* m_device = nullptr;
	HWND m_window = nullptr;
	
	D3DDEVICE_CREATION_PARAMETERS m_creation_parameters = { };
	D3DVIEWPORT9 m_viewport = { };

	ImGuiContext* m_context = nullptr;
	ImDrawList* m_background = nullptr;
};

} // namespace atom::graphics