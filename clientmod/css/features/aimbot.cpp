#include "aimbot.hpp"

namespace css::features
{

float DirectionToRadius( const QAngle& angles, const Vector& begin, const Vector& end )
{
	auto direction = ( end - begin );
	direction.Normalize();

	auto forward = angles.ToVector();
	forward.Normalize();

	const auto dot = std::clamp( forward.Dot( direction ), -1.f, 1.f );
	return RAD2DEG( std::acos( dot ) );
}

void Aimbot::Present()
{
	const auto& aimbot = configuration::m_aimbot;

	if( !aimbot.m_mode )
	{
		return;
	}

	m_player = C_CSPlayer::GetLocalCSPlayer();

	if( !memory::IsUserMode( m_player ) )
	{
		return;
	}

	if( m_player->IsDead() )
	{
		return;
	}

	m_eye_position = m_player->EyePosition();

	m_engine_client->GetViewAngles( m_eye_angles );
	m_eye_angles.Normalize();
	
	if( aimbot.m_mode == 2 )
	{
		if( !GetAsyncKeyState( aimbot.m_key ) )
		{
			m_target_cycle = true;
			m_target_timer.Reset();
			return;
		}
	}

	if( m_target_cycle )
	{
		TargetUpdate();
	}

	if( TargetValidate( m_target_index ) )
	{
		if( aimbot.m_time_delay )
		{
			if( m_target_timer.Elapsed() < aimbot.m_time_delay )
			{
				return;
			}
		}

		if( aimbot.m_time_duration )
		{
			if( m_target_timer.Elapsed() > aimbot.m_time_duration )
			{
				return;
			}
		}

		auto direction = ( m_target_position - m_eye_position );
		direction.Normalize();

		auto angles = direction.ToEulerAngles();
		angles.Normalize();

		if( aimbot.m_rcs )
		{
			auto angles_weapon = m_player->m_vecPunchAngle();
			angles_weapon.Normalize();

			if( aimbot.m_rcs_pitch > 0.f )
			{
				// 
				// compensate pitch
				// 
				angles[ 0 ] -= ( angles_weapon[ 0 ] * ( aimbot.m_rcs_pitch * 0.01f ) );
			}

			if( aimbot.m_rcs_yaw > 0.f )
			{
				// 
				// compensate yaw
				// 
				angles[ 1 ] -= ( angles_weapon[ 1 ] * ( aimbot.m_rcs_yaw * 0.01f ) );
			}

			angles.Normalize();
		}

		if( aimbot.m_step_mode == 1 )
		{
			StepConstant( angles );
		}
		else if( aimbot.m_step_mode == 2 )
		{
			StepDecreasing( angles );
		}

		angles.Normalize();
		angles.Clamp();

		angles[ 2 ] = 0.f;
		m_engine_client->SetViewAngles( angles );
	}
	else
	{
		if( aimbot.m_auto_cycle )
		{
			if( aimbot.m_time_delay_cycle )
			{
				if( m_target_timer.Elapsed() < aimbot.m_time_delay_cycle )
				{
					return;
				}
			}

			m_target_cycle = true;
		}
	}
}

float Aimbot::GetRadius() const
{
	return m_target_radius;
}

bool Aimbot::TargetValidate( int index )
{
	const auto& aimbot = configuration::m_aimbot;
	const auto player = C_CSPlayer::GetCSPlayer( index );

	if( !memory::IsUserMode( player ) )
	{
		return false;
	}

	if( player->IsLocal() )
	{
		return false;
	}

	if( player->IsDormant() )
	{
		return false;
	}

	if( player->IsDead() )
	{
		return false;
	}

	if( !aimbot.m_target_team )
	{
		const auto team = player->m_iTeamNum();
		const auto team_player = m_player->m_iTeamNum();

		if( team == team_player )
		{
			return false;
		}
	}

	matrix3x4_t transform[ MAXSTUDIOBONES ] = { };

	if( !player->SetupBones( transform, MAXSTUDIOBONES, 0x00000100, GetCurtime() ) )
	{
		return false;
	}

	MatrixGetColumn( transform[ aimbot.m_target_bone ], 3, m_target_position );
	const auto distance = m_eye_position.DistTo( m_target_position );
	
	if( distance > aimbot.m_target_distance )
	{
		return false;
	}

	m_target_distance = distance;
	const auto radius = DirectionToRadius( m_eye_angles, m_eye_position, m_target_position );
	
	if( radius > aimbot.m_target_radius )
	{
		return false;
	}

	m_target_radius = radius;
	return true;
}

void Aimbot::TargetUpdate()
{
	const auto& aimbot = configuration::m_aimbot;

	auto best_distance = 8192.f;

	if( aimbot.m_target_priority == 1 )
	{
		best_distance = aimbot.m_target_distance;
	}
	else if( aimbot.m_target_priority == 2 )
	{
		best_distance = aimbot.m_target_radius;
	}

	const auto highest_entity_index = m_client_entity_list->GetHighestEntityIndex();

	for( int index = 0; index <= highest_entity_index; index++ )
	{
		if( TargetValidate( index ) )
		{
			if( aimbot.m_target_priority == 0 )
			{
				m_target_cycle = false;
				m_target_index = index;
				break;
			}
			else if( aimbot.m_target_priority == 1 )
			{
				if( m_target_distance < best_distance )
				{
					best_distance = m_target_distance;
					m_target_cycle = false;
					m_target_index = index;
				}
			}
			else if( aimbot.m_target_priority == 2 )
			{
				if( m_target_radius < best_distance )
				{
					best_distance = m_target_radius;
					m_target_cycle = false;
					m_target_index = index;
				}
			}
		}
	}
}

int Aimbot::GetTargetIndex() const
{
	return m_target_index;
}

void Aimbot::StepConstant( QAngle& angles )
{
	const auto& aimbot = configuration::m_aimbot;

	auto angles_step = ( angles - m_eye_angles );
	angles_step.Normalize();

	if( aimbot.m_step_pitch > 0.f )
	{
		auto factor_pitch = ( aimbot.m_step_pitch / 100.f );

		if( angles_step[ 0 ] < 0.f )
		{
			if( factor_pitch > std::abs( angles_step[ 0 ] ) )
			{
				factor_pitch = std::abs( angles_step[ 0 ] );
			}

			angles[ 0 ] = m_eye_angles[ 0 ] - factor_pitch;
		}
		else
		{
			if( factor_pitch > angles_step[ 0 ] )
			{
				factor_pitch = angles_step[ 0 ];
			}

			angles[ 0 ] = m_eye_angles[ 0 ] + factor_pitch;
		}
	}

	if( aimbot.m_step_yaw > 0.f )
	{
		auto factor_yaw = ( aimbot.m_step_yaw / 100.f );

		if( angles_step[ 1 ] < 0.f )
		{
			if( factor_yaw > std::abs( angles_step[ 1 ] ) )
			{
				factor_yaw = std::abs( angles_step[ 1 ] );
			}

			angles[ 1 ] = m_eye_angles[ 1 ] - factor_yaw;
		}
		else
		{
			if( factor_yaw > angles_step[ 1 ] )
			{
				factor_yaw = angles_step[ 1 ];
			}

			angles[ 1 ] = m_eye_angles[ 1 ] + factor_yaw;
		}
	}
}

void Aimbot::StepDecreasing( QAngle& angles )
{
	const auto& aimbot = configuration::m_aimbot;

	auto angles_step = ( angles - m_eye_angles );
	angles_step.Normalize();

	if( aimbot.m_step_pitch > 0.f )
	{
		angles[ 0 ] = m_eye_angles[ 0 ] + ( angles_step[ 0 ] / aimbot.m_step_pitch );
	}

	if( aimbot.m_step_yaw > 0.f )
	{
		angles[ 1 ] = m_eye_angles[ 1 ] + ( angles_step[ 1 ] / aimbot.m_step_yaw );
	}
}

} // namespace css::features