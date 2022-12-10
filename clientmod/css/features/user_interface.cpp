#include "user_interface.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace css::features
{

void UserInterface::Present()
{
	auto& io = ImGui::GetIO();
	io.MouseDrawCursor = m_state;

	if( !m_state )
	{
		return;
	}

	const auto x = ( ( io.DisplaySize[ 0 ] * 0.5f ) - 200.f );
	const auto y = ( ( io.DisplaySize[ 1 ] * 0.5f ) - 120.f );

	ImGui::SetNextWindowPos( { x, y }, ImGuiCond_FirstUseEver );
	ImGui::SetNextWindowSize( { 400.f, 240.f }, ImGuiCond_FirstUseEver );

	const auto flags = ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings );

	ImGui::Begin( "enigma (" __DATE__ ", " __TIME__ ")", nullptr, flags );
	{
		if( ImGui::BeginTabBar( "page_array", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_NoTooltip ) )
		{
			RenderAimbot();
			RenderVisuals();

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void UserInterface::SetState( bool state )
{
	m_state = state;
}

bool UserInterface::GetState() const
{
	return m_state;
}

void UserInterface::RenderAimbot()
{
	auto& aimbot = configuration::m_aimbot;

	if( ImGui::BeginTabItem( "aimbot" ) )
	{
		ImGui::Combo( "mode##aimbot_mode", &aimbot.m_mode, "off\0auto\0key\0" );
		// ImGui::KeyBind( "key##key_bind", &aimbot.m_key );
		ImGui::InputInt( "key##aimbot_key_input", &aimbot.m_key );
		ImGui::Checkbox( "auto fire##aimbot_auto_fire", &aimbot.m_auto_fire );
		ImGui::Checkbox( "auto cycle##aimbot_auto_cycle", &aimbot.m_auto_cycle );

		ImGui::SliderInt( "bone##aimbot_target_bone", &aimbot.m_target_bone, 0, 127 );
		ImGui::Combo( "priority##aimbot_target_priority", &aimbot.m_target_priority, "random\0distance\0radius\0" );
		ImGui::SliderFloat( "radius##aimbot_target_radius", &aimbot.m_target_radius, 0.f, 180.f );
		ImGui::SliderFloat( "distance##aimbot_target_distance", &aimbot.m_target_distance, 0.f, 8192.f );
		ImGui::Checkbox( "team##aimbot_target_team", &aimbot.m_target_team );

		ImGui::Combo( "step##aimbot_step_mode", &aimbot.m_step_mode, "off\0constant\0decreasing\0" );
		ImGui::SliderFloat( "pitch##aimbot_step_pitch", &aimbot.m_step_pitch, 0.f, 100.f );
		ImGui::SliderFloat( "yaw##aimbot_step_yaw", &aimbot.m_step_yaw, 0.f, 100.f );

		ImGui::Checkbox( "rcs##aimbot_rcs", &aimbot.m_rcs );
		ImGui::SliderFloat( "pitch##aimbot_rcs_pitch", &aimbot.m_rcs_pitch, 0.f, 100.f, "%.0f%%" );
		ImGui::SliderFloat( "yaw##aimbot_rcs_yaw", &aimbot.m_rcs_yaw, 0.f, 100.f, "%.0f%%" );

		ImGui::EndTabItem();
	}
}

void UserInterface::RenderVisuals()
{
	auto& visuals = configuration::m_visuals;

	if( ImGui::BeginTabItem( "visuals" ) )
	{
		ImGui::Combo( "box mode##visuals_box_mode", &visuals.m_box_mode, "off\0solid\0corners\0" );
		ImGui::Checkbox( "box border##visuals_box_border", &visuals.m_box_border );

		ImGui::Checkbox( "health##visuals_health", &visuals.m_health );
		ImGui::Checkbox( "weapon##visuals_weapon", &visuals.m_weapon );
		ImGui::Checkbox( "distance##visuals_distance", &visuals.m_distance );
		ImGui::Checkbox( "skeleton##visuals_skeleton", &visuals.m_skeleton );

		ImGui::EndTabItem();
	}
}

bool UserInterface::ProcessMessage( HWND window, UINT message, WPARAM wparam, LPARAM lparam )
{
	auto& user_interface = UserInterface::Instance();

	switch( message )
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			switch( wparam )
			{
				case VK_INSERT:
				{
					const auto state = user_interface.GetState();
					user_interface.SetState( !state );
					return true;
				}
			}
			break;
		}
	}

	if( user_interface.GetState() )
	{
		return ( ImGui_ImplWin32_WndProcHandler( window, message, wparam, lparam ) != 0 );
	}

	return false;
}

} // namespace css::features