#include "base_entity.hpp"
#include "player.hpp"

#include "../../css/game.hpp"

// 
// [ IHandleEntity ] implementation
// 
void IHandleEntity::SetRefEHandle( const CBaseHandle& handle )
{
	return memory::CallMethod< 1, void >( this, std::cref( handle ) );
}

const CBaseHandle& IHandleEntity::GetRefEHandle()
{
	return memory::CallMethod< 2, const CBaseHandle& >( this );
}

// 
// [ IClientUnknown ] implementation
// 
IClientThinkable* IClientUnknown::GetClientThinkable()
{
	return memory::CallMethod< 3, IClientThinkable* >( this );
}

IClientEntity* IClientUnknown::GetIClientEntity()
{
	return memory::CallMethod< 4, IClientEntity* >( this );
}

C_BaseEntity* IClientUnknown::GetBaseEntity()
{
	return memory::CallMethod< 5, C_BaseEntity* >( this );
}

IClientNetworkable* IClientUnknown::GetClientNetworkable()
{
	return memory::CallMethod< 6, IClientNetworkable* >( this );
}

ICollideable* IClientUnknown::GetCollideable()
{
	return memory::CallMethod< 7, ICollideable* >( this );
}

IClientRenderable* IClientUnknown::GetClientRenderable()
{
	return memory::CallMethod< 8, IClientRenderable* >( this );
}

// 
// [ ICollideable ] implementation
// 
IHandleEntity* ICollideable::GetEntityHandle()
{
	return memory::CallMethod< 0, IHandleEntity* >( this );
}

const Vector& ICollideable::OBBMins()
{
	return memory::CallMethod< 14, const Vector& >( this );
}

const Vector& ICollideable::OBBMaxs()
{
	return memory::CallMethod< 15, const Vector& >( this );
}

// 
// [ IClientRenderable ] implementation
// 
IClientUnknown* IClientRenderable::GetIClientUnknown()
{
	return memory::CallMethod< 0, IClientUnknown* >( this );
}

bool IClientRenderable::SetupBones( matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, double currentTime )
{
	const auto key_array = reinterpret_cast< int* >( css::m_image_client + 0x006E1970 );

	nMaxBones ^= 0xDDBBC76A;
	const auto nMaxBonesKey = key_array[ ( unsigned __int8 )nMaxBones ^ 32 ] ^ ( ( nMaxBones ^ 0x1DC5F820 ) >> 8 );

	boneMask ^= 0xDDBBC76A;
	const auto boneMaskKey = key_array[ ( unsigned __int8 )boneMask ^ 32 ] ^ ( ( boneMask ^ 0x1DC5F820 ) >> 8 );

	return memory::CallMethod< 65, bool >( this,
																				 pBoneToWorldOut,
																				 nMaxBones,
																				 nMaxBonesKey,
																				 boneMask,
																				 boneMaskKey,
																				 currentTime );
}

// 
// [ IClientNetworkable ] implementation
// 
bool IClientNetworkable::IsDormant()
{
	return memory::CallMethod< 45, bool >( this );
}

int IClientNetworkable::entindex()
{
	return memory::CallMethod< 6, int >( this );
}

ClientClass* IClientNetworkable::GetClientClass()
{
	return memory::CallMethod< 10, ClientClass* >( this );
}

// 
// [ IClientEntity ] implementation
// 
const Vector& IClientEntity::GetAbsOrigin()
{
	return memory::CallMethod< 9, const Vector& >( this );
}

const QAngle& IClientEntity::GetAbsAngles()
{
	return memory::CallMethod< 10, const QAngle& >( this );
}

IHandleEntity* IClientEntity::GetEntityHandle()
{
	return GetCollideable()->GetEntityHandle();
}

const Vector& IClientEntity::OBBMins()
{
	return GetCollideable()->OBBMins();
}

const Vector& IClientEntity::OBBMaxs()
{
	return GetCollideable()->OBBMaxs();
}

bool IClientEntity::IsDormant()
{
	// return GetClientNetworkable()->IsDormant();
	return true;
}

int IClientEntity::entindex()
{
	// return GetClientNetworkable()->entindex();
	return -1;
}

ClientClass* IClientEntity::GetClientClass()
{
	return GetClientNetworkable()->GetClientClass();
}

IClientUnknown* IClientEntity::GetIClientUnknown()
{
	return GetClientRenderable()->GetIClientUnknown();
}

bool IClientEntity::SetupBones( matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, double currentTime )
{
	return GetClientRenderable()->SetupBones( pBoneToWorldOut, nMaxBones, boneMask, currentTime );
}

// 
// [ C_BaseEntity ] implementation
// 
C_BaseEntity* C_BaseEntity::GetBaseEntity( int index )
{
	if( index < 0 )
	{
		return nullptr;
	}

	const auto client_entity = css::m_client_entity_list->GetClientEntity( index );

	if( !memory::IsUserMode( client_entity ) )
	{
		return nullptr;
	}

	return client_entity->GetBaseEntity();
}

C_BaseEntity* C_BaseEntity::GetBaseEntity( const CBaseHandle& handle )
{
	if( !handle.IsValid() )
	{
		return nullptr;
	}

	const auto client_entity = css::m_client_entity_list->GetClientEntityFromHandle( handle );

	if( !memory::IsUserMode( client_entity ) )
	{
		return nullptr;
	}

	return client_entity->GetBaseEntity();
}

bool C_BaseEntity::IsPlayer()
{
	return memory::CallMethod< 295, bool >( this );
}

bool C_BaseEntity::IsDead()
{
	return ( m_lifeState() != LIFE_ALIVE );
}

bool C_BaseEntity::IsLocal()
{
	const auto local_player = C_CSPlayer::GetLocalCSPlayer();
	return ( this == local_player );
}

bool C_BaseEntity::IsEnemy( C_BaseEntity* base_entity )
{
	const auto team = base_entity->m_iTeamNum();
	return ( team != m_iTeamNum() );
}

bool C_BaseEntity::IsDormant()
{
	return m_bDormant();
}

CStudioHdr* C_BaseEntity::GetModelPtr()
{
	return m_pStudioHdr();
}

// 
// [ C_BaseAnimating ] implementation
// 
bool C_BaseAnimating::GetBone( int bone, Vector& output )
{
	matrix3x4_t transform[ MAXSTUDIOBONES ] = { };

	if( SetupBones( transform, MAXSTUDIOBONES, 0x00000100, 0.f ) )
	{
		MatrixGetColumn( transform[ bone ], 3, output );
		return true;
	}

	return false;
}