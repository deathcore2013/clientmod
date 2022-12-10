#include "process_message.hpp"

#include "../features/user_interface.hpp"

namespace css::hooks
{

LRESULT API_STDCALL ProcessMessage( HWND window, UINT message, WPARAM wparam, LPARAM lparam )
{
	auto& user_interface = features::UserInterface::Instance();
	// TRACE( "%s(0x%08X, 0x%08X, %u, %d)", __FUNCTION__, memory::ToUIntPointer( window ), message, wparam, lparam );

	if( user_interface.ProcessMessage( window, message, wparam, lparam ) )
	{
		return FALSE;
	}

	if( m_process_message )
	{
		return CallWindowProcA( m_process_message, window, message, wparam, lparam );
	}

	return DefWindowProcA( window, message, wparam, lparam );
}

} // namespace css::hooks