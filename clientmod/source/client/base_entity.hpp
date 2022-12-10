#pragma once

#include "../sdk.hpp"

class IHandleEntity
{
public:
	void SetRefEHandle( const CBaseHandle& handle );
	const CBaseHandle& GetRefEHandle();
};

class IClientUnknown
{
public:
	IClientThinkable* GetClientThinkable();
	IClientEntity* GetIClientEntity();
	C_BaseEntity* GetBaseEntity();
	IClientNetworkable* GetClientNetworkable();
	ICollideable* GetCollideable();
	IClientRenderable* GetClientRenderable();
};

class ICollideable
{
public:
	IHandleEntity* GetEntityHandle();
	const Vector& OBBMins();
	const Vector& OBBMaxs();
};

class IClientNetworkable
{
public:
	bool IsDormant();
	int entindex();
	ClientClass* GetClientClass();
};

class IClientRenderable
{
public:
	IClientUnknown* GetIClientUnknown();
	bool SetupBones( matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, double currentTime );
};

class IClientThinkable
{ };

class IClientEntity : public IClientUnknown
{
public:
	const Vector& GetAbsOrigin();
	const QAngle& GetAbsAngles();

public:
	IHandleEntity* GetEntityHandle();
	const Vector& OBBMins();
	const Vector& OBBMaxs();

public:
	bool IsDormant();
	int entindex();
	ClientClass* GetClientClass();

public:
	IClientUnknown* GetIClientUnknown();
	bool SetupBones( matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, double currentTime );
};

class C_BaseEntity : public IClientEntity
{
public:
	static C_BaseEntity* GetBaseEntity( int index );
	static C_BaseEntity* GetBaseEntity( const CBaseHandle& handle );

public:
	bool IsPlayer();

public:
	bool IsDead();
	bool IsLocal();
	bool IsEnemy( C_BaseEntity* base_entity );
	bool IsDormant();

	CStudioHdr* GetModelPtr();

public:
	FIELD( char, m_lifeState, 0x04D1 );
	FIELD( bool, m_bDormant, 0x0596 );

	FIELD( int, m_iTeamNum, 0x04DC );
	FIELD( int, m_iHealth, 0x4F90 );
	FIELD( int, m_nTickBase, 0x5028 );

	FIELD( float, m_flMaxspeed, 0x4FB4 );
	FIELD( float, m_flWaterJumpTime, 0x4FFC );
	FIELD( float, m_flStepSoundTime, 0x4FA4 );
	FIELD( float, m_flSwimSoundTime, 0x5004 );
	FIELD( float, m_flPhysics, 0x5024 );
	
	FIELD( Vector, m_vecViewOffset, 0x0528 );
	FIELD( Vector, m_vecVelocity, 0x0534 );
	FIELD( Vector, m_vecBaseVelocity, 0x0570 );

	FIELD( QAngle, m_angRotation, 0x066C );
	FIELD( Vector, m_vecOrigin, 0x076C );

	FIELD( Vector, m_vecPunchAngle, 0x4B8C );
	FIELD( Vector, m_vecPunchAngleVel, 0x4BD0 );

	FIELD( Vector, m_vecNetworkOrigin, 0x0908 );
	FIELD( QAngle, m_angNetworkAngles, 0x0914 );

	// m_Local @ 0x4B18
	// m_vecPunchAngle @ 0x0074
	// m_vecPunchAngle @ 0x00B8

	FIELD( matrix3x4_t, m_rgflCoordinateFrame, 0x0880 );
	FIELD( CStudioHdr*, m_pStudioHdr, 0x1034 );
};

class C_BaseAnimating : public C_BaseEntity
{
public:
	bool GetBone( int bone, Vector& output );
};

class C_BaseAnimatingOverlay : public C_BaseAnimating
{ };

class C_BaseCombatCharacter : public C_BaseAnimatingOverlay
{
public:
	FIELD( bool, m_bFOVChanged, 0x2B14 );

	FIELD( int, m_iAmmo, 0x2344 );
	FIELD( int, m_iFOV, 0x2B00 );
	FIELD( int, m_iFOVStart, 0x2B04 );
	
	FIELD( float, m_flNextAttack, 0x2730 );
	FIELD( float, m_flFOVTime, 0x2B08 );

	FIELD( CBaseHandle, m_hActiveWeapon, 0x4AC8 );
};