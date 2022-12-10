#include "player.hpp"

#include "../../css/game.hpp"

// 
// [ C_BasePlayer ] implementation
// 
C_BasePlayer* C_BasePlayer::GetBasePlayer( int index )
{
	const auto base_entity = C_BaseEntity::GetBaseEntity( index );
	return ToBasePlayer( base_entity );
}

C_BasePlayer* C_BasePlayer::GetBasePlayer( const CBaseHandle& handle )
{
	const auto base_entity = C_BaseEntity::GetBaseEntity( handle );
	return ToBasePlayer( base_entity );
}

Vector C_BasePlayer::EyePosition()
{
	return ( GetAbsOrigin() + m_vecViewOffset() );
}

QAngle C_BasePlayer::EyeAngles()
{
	return GetAbsAngles();
}

// 
// [ C_BasePlayer ] implementation
// 
C_CSPlayer* C_CSPlayer::GetCSPlayer( int index )
{
	const auto base_player = C_BasePlayer::GetBasePlayer( index );
	return static_cast< C_CSPlayer* >( base_player );
}

C_CSPlayer* C_CSPlayer::GetCSPlayer( const CBaseHandle& handle )
{
	const auto base_player = C_BasePlayer::GetBasePlayer( handle );
	return static_cast< C_CSPlayer* >( base_player );
}

C_CSPlayer* C_CSPlayer::GetLocalCSPlayer()
{
	static std::uintptr_t pointer = 0;
	static std::uintptr_t xor_key = 0;

	/*if( !memory::IsUserMode( pointer ) || !xor_key )
	{
		std::vector< std::uintptr_t > occurrences = { };

		if( memory::SearchSection( css::m_image_client, ".text", "8B 1D ? ? ? ? 81 F3 ? ? ? ? 0F 84 ? ? ? ? 83 BB", occurrences ) )
		{
			pointer = *reinterpret_cast< std::uintptr_t* >( occurrences[ 0 ] + 2 );
			xor_key = *reinterpret_cast< std::uintptr_t* >( occurrences[ 0 ] + 8 );

			if( !memory::IsUserMode( pointer ) || !xor_key )
			{
				TRACE( "%s: Local player occurences are not valid!", __FUNCTION__ );
				return nullptr;
			}
		}
		else
		{
			TRACE( "%s: Could not find local player occurrences!", __FUNCTION__ );
			return nullptr;
		}
	}*/

	pointer = ( css::m_image_client + 0x008137C8 );
	const auto local_player = *reinterpret_cast< std::uintptr_t* >( pointer );

	if( local_player == 0 )
	{
		return nullptr;
	}

	xor_key = 0x1DA6C2EB;
	return reinterpret_cast< C_CSPlayer* >( local_player ^ xor_key );
}