#include "reset.hpp"

namespace css::hooks
{

HRESULT API_STDCALL Reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters )
{
	auto& renderer = graphics::Renderer::Instance();
	renderer.Lost();

	const auto status = m_reset( device, presentation_parameters );

	if( SUCCEEDED( status ) )
	{
		renderer.Reset();
	}

	return status;
}

} // namespace css::hooks