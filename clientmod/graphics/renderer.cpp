#include "renderer.hpp"

#include "../win32/trace.hpp"

namespace atom::graphics
{

Renderer::~Renderer()
{
	Destroy();
}

bool Renderer::Create( IDirect3DDevice9* device )
{
	m_device = device;

	if( !m_device )
	{
		TRACE( "%s: m_device is not valid!", __FUNCTION__ );
		return false;
	}

	auto status = m_device->GetCreationParameters( &m_creation_parameters );

	if( FAILED( status ) )
	{
		TRACE( "%s: IDirect3DDevice9::GetCreationParameters(...) error! (0x%08X)", __FUNCTION__, status );
		return false;
	}
	
	status = m_device->GetViewport( &m_viewport );

	if( FAILED( status ) )
	{
		TRACE( "%s: IDirect3DDevice9::GetViewport(...) error! (0x%08X)", __FUNCTION__, status );
		return false;
	}

	m_window = m_creation_parameters.hFocusWindow;

	if( !m_window )
	{
		TRACE( "%s: m_window is not valid!", __FUNCTION__ );
		return false;
	}

	m_context = ImGui::CreateContext();

	if( !m_context )
	{
		TRACE( "%s: ImGui::CreateContext(...) error!", __FUNCTION__ );
		return false;
	}

	if( !ImGui_ImplWin32_Init( m_window ) )
	{
		TRACE( "%s: ImGui_ImplWin32_Init(...) error!", __FUNCTION__ );
		return false;
	}

	if( !ImGui_ImplDX9_Init( device ) )
	{
		TRACE( "%s: ImGui_ImplDX9_Init(...) error!", __FUNCTION__ );
		return false;
	}

	m_background = ImGui::GetBackgroundDrawList();

	if( !m_background )
	{
		TRACE( "%s: ImGui::GetBackgroundDrawList(...) error!", __FUNCTION__ );
		return false;
	}

	return true;
}

void Renderer::Destroy()
{
	Lost();

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	if( m_context )
	{
		ImGui::DestroyContext( m_context );
	}

	m_device = nullptr;
	m_window = nullptr;

	m_context = nullptr;
	m_background = nullptr;
}

bool Renderer::Begin()
{
	const auto status = m_device->GetViewport( &m_viewport );

	if( FAILED( status ) )
	{
		TRACE( "%s: IDirect3DDevice9::GetViewport(...) error! (0x%08X)", __FUNCTION__, status );
		return false;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	m_background = ImGui::GetBackgroundDrawList();
	
	if( !m_background )
	{
		TRACE( "%s: ImGui::GetBackgroundDrawList(...) error!", __FUNCTION__ );
		return false;
	}

	return true;
}

void Renderer::End()
{
	ImGui::Render();

	const auto data = ImGui::GetDrawData();

	if( data )
	{
		ImGui_ImplDX9_RenderDrawData( data );
	}
}

void Renderer::Lost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

bool Renderer::Reset()
{
	if( !ImGui_ImplDX9_CreateDeviceObjects() )
	{
		TRACE( "%s: ImGui_ImplDX9_CreateDeviceObjects(...) error!", __FUNCTION__ );
		return false;
	}

	return true;
}

IDirect3DDevice9* Renderer::GetDevice() const
{
	return m_device;
}

HWND Renderer::GetWindow() const
{
	return m_window;
}

const D3DDEVICE_CREATION_PARAMETERS& Renderer::GetCreationParameters() const
{
	return m_creation_parameters;
}

const D3DVIEWPORT9& Renderer::GetViewport() const
{
	return m_viewport;
}

void Renderer::DrawCircle( const ImVec2& center_screen, float radius, const Color& color, int segments /*= 12*/ )
{
	m_background->AddCircle( { center_screen[ 0 ], center_screen[ 1 ] }, radius, color, segments );
}

void Renderer::DrawLine( const ImVec2& begin, const ImVec2& end, const Color& color )
{
	m_background->AddLine( { begin[ 0 ], begin[ 1 ] }, { end[ 0 ], end[ 1 ] }, color );
}

void Renderer::DrawRect( const ImVec2& position, const ImVec2& size, const Color& color )
{
	m_background->AddRectFilled( { position[ 0 ], position[ 1 ] }, { position[ 0 ] + size[ 0 ], position[ 1 ] + size[ 1 ] }, color, 0.f, ImDrawFlags_None );
}

void Renderer::DrawBox( const ImVec2& position, const ImVec2& size, float thickness, const Color& color )
{
	DrawRect( position, { size[ 0 ], thickness }, color );
	DrawRect( position, { thickness, size[ 1 ] }, color );

	DrawRect( { position[ 0 ] + size[ 0 ], position[ 1 ] }, { thickness, size[ 1 ] }, color );
	DrawRect( { position[ 0 ], position[ 1 ] + size[ 1 ] }, { size[ 0 ] + thickness, thickness }, color );
}

void Renderer::DrawBoxBorder( const ImVec2& position, const ImVec2& size, float thickness, const Color& color, const Color& border /*= Color::Black*/ )
{
	DrawBox( position, size, thickness, color );
	DrawBox( position - ImVec2( thickness, thickness ), size + ImVec2( thickness * 2.f, thickness * 2.f ), 1.f, border );
	DrawBox( position + ImVec2( thickness, thickness ), size - ImVec2( thickness * 2.f, thickness * 2.f ), 1.f, border );
}

void Renderer::DrawCornerBox( const ImVec2& position, const ImVec2& size, float thickness, const Color& color )
{
	const auto line_size = size * 0.25f;

	DrawRect( position, { line_size[ 0 ], thickness }, color ); // top left -> right
	DrawRect( position, { thickness, line_size[ 1 ] }, color ); // top left -> bottom

	DrawRect( { position[ 0 ], position[ 1 ] + size[ 1 ] - line_size[ 1 ] + thickness }, { thickness, line_size[ 1 ] }, color ); // bottom left -> top
	DrawRect( { position[ 0 ], position[ 1 ] + size[ 1 ] }, { line_size[ 0 ], thickness }, color ); // bottom left -> right

	DrawRect( { position[ 0 ] + size[ 0 ] - line_size[ 0 ] + thickness, position[ 1 ] }, { line_size[ 0 ], thickness }, color ); // top right -> left
	DrawRect( { position[ 0 ] + size[ 0 ], position[ 1 ] }, { thickness, line_size[ 1 ] }, color ); // top right -> bottom

	DrawRect( { position[ 0 ] + size[ 0 ] - line_size[ 0 ] + thickness, position[ 1 ] + size[ 1 ] }, { line_size[ 0 ], thickness }, color ); // bottom right -> left
	DrawRect( { position[ 0 ] + size[ 0 ], position[ 1 ] + size[ 1 ] - line_size[ 1 ] + thickness }, { thickness, line_size[ 1 ] }, color ); // bottom right -> top
}

void Renderer::DrawCornerBoxBorder( const ImVec2& position, const ImVec2& size, float thickness, const Color& color, const Color& border /*= Color::Black*/ )
{
	const auto line_size = size * 0.25f;

	DrawRect( { position[ 0 ] - thickness, position[ 1 ] - thickness }, { line_size[ 0 ] + thickness * 2.f, 1.f + thickness * 2.f }, border ); // top left -> right
	DrawRect( { position[ 0 ] - thickness, position[ 1 ] - thickness }, { 1.f + thickness * 2.f, line_size[ 1 ] + thickness * 2.f }, border ); // top left -> bottom
	DrawRect( { position[ 0 ] - thickness, position[ 1 ] + size[ 1 ] - line_size[ 1 ] }, { 1.f + thickness * 2.f, line_size[ 1 ] + thickness * 2.f }, border ); // bottom left -> top
	DrawRect( { position[ 0 ] - thickness, position[ 1 ] + size[ 1 ] - thickness }, { line_size[ 0 ] + thickness * 2.f, 1.f + thickness * 2.f }, border ); // bottom left -> right
	DrawRect( { position[ 0 ] + size[ 0 ] - line_size[ 0 ], position[ 1 ] - thickness }, { line_size[ 0 ] + thickness * 2.f, 1.f + thickness * 2.f }, border ); // top right -> left
	DrawRect( { position[ 0 ] + size[ 0 ] - thickness, position[ 1 ] - thickness }, { 1.f + thickness * 2.f, line_size[ 1 ] + thickness * 2.f }, border ); // top right -> bottom
	DrawRect( { position[ 0 ] + size[ 0 ] - line_size[ 0 ], position[ 1 ] + size[ 1 ] - thickness }, { line_size[ 0 ] + thickness * 2.f, 1.f + thickness * 2.f }, border ); // bottom right -> left
	DrawRect( { position[ 0 ] + size[ 0 ] - thickness, position[ 1 ] + size[ 1 ] - line_size[ 1 ] }, { 1.f + thickness * 2.f, line_size[ 1 ] + thickness * 2.f }, border ); // bottom right -> top

	DrawCornerBox( position, size, thickness, color );
}

void Renderer::DrawText( const ImVec2& position, TextLayout layout, const Color& color, const char* const format, ... )
{
	va_list arg_pack = { };
	va_start( arg_pack, format );
	DrawTextFormat( position, layout, color, Color::Transparent, format, arg_pack );
	va_end( arg_pack );
}

void Renderer::DrawTextShadow( const ImVec2& position, TextLayout layout, const Color& color, const char* const format, ... )
{
	const Color shadow( 0.f, 0.f, 0.f, color[ 3 ] );

	va_list arg_pack = { };
	va_start( arg_pack, format );
	DrawTextFormat( position + ImVec2( 1.f, 1.f ), layout, shadow, Color::Transparent, format, arg_pack );
	DrawTextFormat( position, layout, color, Color::Transparent, format, arg_pack );
	va_end( arg_pack );
}

void Renderer::DrawTextBorder( const ImVec2& position, TextLayout layout, const Color& color, const Color& border, const char* const format, ... )
{
	va_list arg_pack = { };
	va_start( arg_pack, format );
	DrawTextFormat( position, layout, color, border, format, arg_pack );
	va_end( arg_pack );
}

void Renderer::DrawTexture( ImTextureID texture_id, const ImVec2& position, const ImVec2& size, const Color& color /*= Color::White*/ )
{
	m_background->AddImage( texture_id, { position[ 0 ], position[ 1 ] }, { position[ 0 ] + size[ 0 ], position[ 1 ] + size[ 1 ] }, { 0.f, 0.f }, { 1.f, 1.f }, color );
}

void Renderer::DrawTextFormat( ImVec2 position, TextLayout layout, const Color& color, const Color& border, const char* const format, va_list arg_pack )
{
	const auto output_length = static_cast< std::size_t >( _vsnprintf_l( nullptr, 0, format, nullptr, arg_pack ) );

	if( !output_length )
	{
		return;
	}

	const auto output = std::make_unique< char[] >( output_length + 1 );
	_vsnprintf_l( output.get(), output_length, format, nullptr, arg_pack );

	const auto size = ImGui::CalcTextSize( output.get() );

	if( core::HasAnyFlags( layout, TextLayout::Right ) )
	{
		position[ 0 ] -= size[ 0 ];
	}
	else if( core::HasAnyFlags( layout, TextLayout::CenterH ) )
	{
		position[ 0 ] -= size[ 0 ] * 0.5f;
	}

	if( core::HasAnyFlags( layout, TextLayout::CenterV ) )
	{
		position[ 1 ] -= size[ 1 ] * 0.5f;
	}

	DrawRect( position, { size[ 0 ], size[ 1 ] }, border );
	m_background->AddText( { position[ 0 ], position[ 1 ] }, color, output.get() );
}

} // namespace atom::graphics