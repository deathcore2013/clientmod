#pragma once

#include "../game.hpp"

namespace css::hooks
{

LRESULT API_STDCALL ProcessMessage( HWND window, UINT message, WPARAM wparam, LPARAM lparam );

} // namespace css::hooks