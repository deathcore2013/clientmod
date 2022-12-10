#pragma once

#include "../game.hpp"

namespace css::hooks
{

HRESULT API_STDCALL Reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters );

} // namespace css::hooks