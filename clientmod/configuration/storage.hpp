#pragma once

#include "../source/sdk.hpp"

namespace configuration
{

struct CategoryAimbot
{
	int m_mode = 2;																								// 0 - off, 1 - auto, 2 - key
	int m_key = 1;																								// 
	bool m_auto_fire = false;																			// 
	bool m_auto_cycle = true;																			// 

	int m_target_bone = 14;																				// 14 - head
	int m_target_priority = 2;																		// 0 - random, 1 - distance, 2 - radius
	float m_target_distance = 8192.f;															// 0.f - 8192.f
	float m_target_radius = 180.f;																// 0.f - 180.f
	bool m_target_team = false;																		// 
	
	int m_step_mode = 0;																					// 0 - off, 1 - constant, 2 - decreasing
	float m_step_pitch = 0.f;																			// 0.f - 100.f
	float m_step_yaw = 0.f;																				// 0.f - 100.f

	bool m_rcs = true;																						// 
	float m_rcs_pitch = 100.f;																		// 0.f - 100.f
	float m_rcs_yaw = 100.f;																			// 0.f - 100.f

	int m_time_delay = 0;																					// 0 - 5000
	int m_time_delay_cycle = 0;																		// 0 - 5000
	int m_time_duration = 0;																			// 0 - 5000
};

struct CategoryVisuals
{
	int m_box_mode = 2;																						// 0 - off, 1 - solid, 2 - corners
	bool m_box_border = true;																			// 

	bool m_health = true;																					// 
	bool m_weapon = false;																				// 
	bool m_distance = false;																			// 
	bool m_skeleton = true;																				// 
};

extern CategoryAimbot m_aimbot;
extern CategoryVisuals m_visuals;

} // namespace configuration