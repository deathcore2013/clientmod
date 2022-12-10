#pragma once

#include "../sdk.hpp"

#include "base_entity.hpp"

class C_BasePlayer : public C_BaseCombatCharacter
{
public:
	static C_BasePlayer* GetBasePlayer( int index );
	static C_BasePlayer* GetBasePlayer( const CBaseHandle& handle );

public:
	Vector EyePosition();
	QAngle EyeAngles();
};

class C_CSPlayer : public C_BasePlayer
{
public:
	static C_CSPlayer* GetCSPlayer( int index );
	static C_CSPlayer* GetCSPlayer( const CBaseHandle& handle );
	static C_CSPlayer* GetLocalCSPlayer();
};

FORCEINLINE C_BasePlayer* ToBasePlayer( C_BaseEntity* base_entity )
{
	if( !memory::IsUserMode( base_entity ) )
	{
		return nullptr;
	}

	if( !base_entity->IsPlayer() )
	{
		return nullptr;
	}

	return static_cast< C_BasePlayer* >( base_entity );
}

FORCEINLINE C_CSPlayer* ToCSPlayer( C_BaseEntity* base_entity )
{
	if( !memory::IsUserMode( base_entity ) )
	{
		return nullptr;
	}

	if( !base_entity->IsPlayer() )
	{
		return nullptr;
	}

	return static_cast< C_CSPlayer* >( base_entity );
}