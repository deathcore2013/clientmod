#pragma once

#include "../game.hpp"

namespace css::hooks
{

HRESULT API_STDCALL Present( IDirect3DDevice9* device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region );

} // namespace css::hooks