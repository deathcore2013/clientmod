#pragma once

#include "../game.hpp"

namespace css::features
{

class UserInterface : public core::Singleton< UserInterface >
{
public:
	static bool ProcessMessage( HWND window, UINT message, WPARAM wparam, LPARAM lparam );

public:
	void Present();

	void SetState( bool state );
	bool GetState() const;

protected:
	void RenderAimbot();
	void RenderVisuals();

protected:
	bool m_state = false;
};

} // namespace css::features