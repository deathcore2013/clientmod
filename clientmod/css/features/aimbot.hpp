#pragma once

#include "../game.hpp"

namespace css::features
{

class Aimbot : public core::Singleton< Aimbot >
{
public:
	void Present();

	float GetRadius() const;

protected:
	bool TargetValidate( int index );
	void TargetUpdate();

	int GetTargetIndex() const;

	void StepConstant( QAngle& angles );
	void StepDecreasing( QAngle& angles );

protected:
	C_CSPlayer* m_player = nullptr;

	Vector m_eye_position = { };
	QAngle m_eye_angles = { };

	bool m_target_cycle = true;
	int m_target_index = -1;
	Vector m_target_position = { };
	float m_target_distance = 0.f;
	float m_target_radius = 0.f;
	core::Timer m_target_timer = { };
};

} // namespace css::features