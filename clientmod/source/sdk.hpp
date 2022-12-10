#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "../core/enum_operator.hpp"
#include "../core/no_copy.hpp"
#include "../core/no_move.hpp"
#include "../core/singleton.hpp"
#include "../core/timer.hpp"

#include "../graphics/color.hpp"
#include "../graphics/renderer.hpp"

#include "../memory/base.hpp"
#include "../memory/operation.hpp"
#include "../memory/search.hpp"

#include "../minhook/minhook.hpp"

#include "../win32/image.hpp"
#include "../win32/trace.hpp"

using namespace atom;

using atom::graphics::Color;
using atom::graphics::TextLayout;

#include "shared/math_pfns.hpp"
#include "shared/mathlib.hpp"
#include "shared/matrix.hpp"
#include "shared/qangle.hpp"
#include "shared/vector.hpp"
#include "shared/vector2d.hpp"
#include "shared/vector4d.hpp"

#include "tier0/basetypes.hpp"
#include "tier0/commonmacros.hpp"
#include "tier0/dbg.hpp"
#include "tier0/platform.hpp"

// 
// [ macro ] definition
// 
#if defined( DispatchMessage )
#undef DispatchMessage
#endif // DispatchMessage

#define MAX_PLAYER_NAME_LENGTH																	( 32 )
#define MAX_MAP_NAME																						( 32 )
#define MAX_NETWORKID_LENGTH																		( 64 )

#define MAX_EDICT_BITS																					( 11 )
#define MAX_EDICTS																							( 1 << MAX_EDICT_BITS )

#define MAX_SERVER_CLASS_BITS																		( 9 )
#define MAX_SERVER_CLASSES																			( 1 << MAX_SERVER_CLASS_BITS )

#define SIGNED_GUID_LEN																					( 32 )

#define NUM_ENT_ENTRY_BITS																			( MAX_EDICT_BITS + 1 )
#define NUM_ENT_ENTRIES																					( 1 << NUM_ENT_ENTRY_BITS )
#define ENT_ENTRY_MASK																					( NUM_ENT_ENTRIES - 1 )
#define INVALID_EHANDLE_INDEX																		( 0xFFFFFFFF )

#define NUM_SERIAL_NUM_BITS																			( 32 - NUM_ENT_ENTRY_BITS )

#define MAX_CUSTOM_FILES																				( 4 )
#define MAX_CUSTOM_FILE_SIZE																		( 131072 )

#define LIFE_ALIVE																							( 0 )
#define LIFE_DYING																							( 1 )
#define LIFE_DEAD																								( 2 )
#define LIFE_RESPAWNABLE																				( 3 )
#define LIFE_DISCARDBODY																				( 4 )

#define MAX_USER_MSG_DATA																				( 255 )
#define MAX_ENTITY_MSG_DATA																			( 255 )

#define MAXSTUDIOSKINS																					( 32 )
#define MAXSTUDIOBONES																					( 128 )
#define MAXSTUDIOBLENDS																					( 32 )
#define MAXSTUDIOPOSEPARAM																			( 24 )
#define MAXSTUDIOBONECTRLS																			( 4 )

#define FLOW_OUTGOING																						( 0 )
#define FLOW_INCOMING																						( 1 )
#define MAX_FLOWS																								( 2 )

#define IN_ATTACK																								( 1 << 0 )
#define IN_JUMP																									( 1 << 1 )
#define IN_DUCK																									( 1 << 2 )
#define IN_FORWARD																							( 1 << 3 )
#define IN_BACK																									( 1 << 4 )
#define IN_USE																									( 1 << 5 )
#define IN_CANCEL																								( 1 << 6 )
#define IN_LEFT																									( 1 << 7 )
#define IN_RIGHT																								( 1 << 8 )
#define IN_MOVELEFT																							( 1 << 9 )
#define IN_MOVERIGHT																						( 1 << 10 )
#define IN_ATTACK2																							( 1 << 11 )
#define IN_RUN																									( 1 << 12 )
#define IN_RELOAD																								( 1 << 13 )
#define IN_ALT1																									( 1 << 14 )
#define IN_ALT2																									( 1 << 15 )
#define IN_SCORE																								( 1 << 16 )
#define IN_SPEED																								( 1 << 17 )
#define IN_WALK																									( 1 << 18 )
#define IN_ZOOM																									( 1 << 19 )
#define IN_WEAPON1																							( 1 << 20 )
#define IN_WEAPON2																							( 1 << 21 )
#define IN_BULLRUSH																							( 1 << 22 )
#define IN_GRENADE1																							( 1 << 23 )
#define IN_GRENADE2																							( 1 << 24 )

#define MULTIPLAYER_BACKUP																			( 90 )

// 
// [ enum ] definition
// 
enum SendPropType
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_NUMSendPropTypes,
};
// sizeof( SendPropType ) = 0x0004 ( 4 )

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
};
// sizeof( MoveType_t ) = 0x0004

enum MoveCollide_t
{
	MOVECOLLIDE_DEFAULT = 0,
	MOVECOLLIDE_FLY_BOUNCE,
	MOVECOLLIDE_FLY_CUSTOM,
	MOVECOLLIDE_FLY_SLIDE,
	MOVECOLLIDE_COUNT,
	MOVECOLLIDE_MAX_BITS = 3,
};
// sizeof( MoveCollide_t ) = 0x0004

enum SolidType_t
{
	SOLID_NONE = 0,
	SOLID_BSP = 1,
	SOLID_BBOX = 2,
	SOLID_OBB = 3,
	SOLID_OBB_YAW = 4,
	SOLID_CUSTOM = 5,
	SOLID_VPHYSICS = 6,
	SOLID_LAST,
};
// sizeof( SolidType_t ) = 0x0004

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};
// sizeof( TraceType_t ) = 0x0004

// 
// [ struct ] declaration
// 
struct model_t;

struct mstudiobone_t;
struct mstudiobbox_t;
struct mstudiohitboxset_t;
struct studiohdr_t;
struct player_info_t;
struct Ray_t;
struct cplane_t;
struct csurface_t;

// 
// [ class ] declaration
// 
class IMoveHelper;

class IHandleEntity;
class IClientUnknown;
class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientThinkable;
class IClientEntity;

class C_BaseEntity;
class C_BaseAnimating;
class C_BaseAnimatingOverlay;
class C_BaseCombatCharacter;
class C_BasePlayer;
class C_CSPlayer;
class C_BaseCombatWeapon;
class C_WeaponCSBase;
class C_WeaponCSBaseGun;

class InterfaceReg;
class RecvTable;
class RecvProp;
class ClientClass;
class CBaseHandle;
class CEntInfo;
class CStudioHdr;
class CBaseTrace;
class CGameTrace;
class ITraceFilter;
class CTraceFilter;
class CTraceFilterSimple;
class CTraceFilterSkipTwoEntities;

class IBaseClientDLL;
class CHLClient;

class IClientEntityList;
class CClientEntityList;

class IVEngineClient;
class CEngineClient;

class IEngineTrace;
class CEngineTraceClient;

class IRender;
class CRender;

// 
// [ data ] type declaration
// 
using CRC32_t = unsigned long;
using ClientRenderHandle_t = unsigned short;
using ModelInstanceHandle_t = unsigned short;
using ClientShadowHandle_t = unsigned short;
using SpatialPartitionHandle_t = unsigned short;
using ClientThinkHandle_t = void*;
using string_t = const char*;
using EntityHandle = CBaseHandle;
using trace_t = CGameTrace;

// 
// [ function ] type declaration
// 
using CreateInterfaceFn = void* ( API_CDECL* )( const char*, int* );
using InstantiateInterfaceFn = void* ( API_CDECL* )();

using CreateClientClassFn = IClientNetworkable* ( API_CDECL* )( int, int );
using CreateEventFn = IClientNetworkable* ( API_CDECL* )();

using PresentFn = HRESULT ( API_STDCALL* )( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* );
using ResetFn = HRESULT ( API_STDCALL* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );

using DispatchMessageFn = LRESULT( API_STDCALL* )( const MSG* );

std::uint32_t Hash( const char* data );

// 
// [ struct ] definition
// 
struct mstudiobone_t
{
public:
	const char* pszName() const;

public:
	int sznameindex = 0;																// 0x0000
	int parent = 0;																			// 0x0004
	int bonecontroller[ 6 ] = { };											// 0x0008
	Vector pos = { };																		// 0x0020
	Quaternion quat = { };															// 0x002C
	RadianEuler rot = { };															// 0x003C
	Vector posscale = { };															// 0x0048
	Vector rotscale = { };															// 0x0054
	matrix3x4_t poseToBone = { };												// 0x0060
	Quaternion qAlignment = { };												// 0x0090
	int flags = 0;																			// 0x00A0
	int proctype = 0;																		// 0x00A4
	int procindex = 0;																	// 0x00A8
	int physicsbone = 0;																// 0x00AC
	int surfacepropidx = 0;															// 0x00B0
	int contents = 0;																		// 0x00B4
	int unused[ 8 ] = { };															// 0x00BC
};
// sizeof( mstudiobone_t ) = 0x00D8 ( 216 )

struct mstudiobbox_t
{
public:
	const char* pszHitboxName() const;

public:
	int bone = 0;																				// 0x0000
	int group = 0;																			// 0x0004
	Vector bbmin = { };																	// 0x0008
	Vector bbmax = { };																	// 0x0014
	int szhitboxnameindex = 0;													// 0x0020
	int unused[ 8 ] = { };															// 0x0024
};
// sizeof( mstudiobbox_t ) = 0x0044 ( 68 )

struct mstudiohitboxset_t
{
public:
	const char* pszName() const;
	mstudiobbox_t* pHitbox( int index ) const;

public:
	int sznameindex = 0;																// 0x0000
	int numhitboxes = 0;																// 0x0004
	int hitboxindex = 0;																// 0x0008
};
// sizeof( mstudiohitboxset_t ) = 0x000C ( 12 )

struct studiohdr_t
{
public:
	const char* pszName() const;
	mstudiobone_t* pBone( int index ) const;
	mstudiohitboxset_t* pHitboxSet( int index ) const;

public:
	int id = 0;																					// 0x0000
	int version = 0;																		// 0x0004
	long checksum = 0;																	// 0x0008
	char name[ 64 ] = { };															// 0x000C
	int length = 0;																			// 0x004C
	Vector eyeposition = { };														// 0x0050
	Vector illumposition = { };													// 0x005C
	Vector hull_min = { };															// 0x0068
	Vector hull_max = { };															// 0x0074
	Vector view_bbmin = { };														// 0x0080
	Vector view_bbmax = { };														// 0x008C
	int flags = 0;																			// 0x0098
	int numbones = 0;																		// 0x009C
	int boneindex = 0;																	// 0x00A0
	int numbonecontrollers = 0;													// 0x00A4
	int bonecontrollerindex = 0;												// 0x00A8
	int numhitboxsets = 0;															// 0x00AC
	int hitboxsetindex = 0;															// 0x00B0
	int numlocalanim = 0;																// 0x00B4
	int localanimindex = 0;															// 0x00B8
	int numlocalseq = 0;																// 0x00BC
	int localseqindex = 0;															// 0x00C0
	int activitylistversion = 0;												// 0x00C4
	int eventsindexed = 0;															// 0x00C8
	int numtextures = 0;																// 0x00CC
	int textureindex = 0;																// 0x00D0
	int numcdtextures = 0;															// 0x00D4
	int cdtextureindex = 0;															// 0x00D8
	int numskinref = 0;																	// 0x00DC
	int numskinfamilies = 0;														// 0x00E0
	int skinindex = 0;																	// 0x00E4
	int numbodyparts = 0;																// 0x00E8
	int bodypartindex = 0;															// 0x00EC
	int numlocalattachments = 0;												// 0x00F0
	int localattachmentindex = 0;												// 0x00F4
	int numlocalnodes = 0;															// 0x00F8
	int localnodeindex = 0;															// 0x00FC
	int localnodenameindex = 0;													// 0x0100
	int numflexdesc = 0;																// 0x0104
	int flexdescindex = 0;															// 0x0108
	int numflexcontrollers = 0;													// 0x010C
	int flexcontrollerindex = 0;												// 0x0110
	int numflexrules = 0;																// 0x0114
	int flexruleindex = 0;															// 0x0118
	int numikchains = 0;																// 0x011C
	int ikchainindex = 0;																// 0x0120
	int nummouths = 0;																	// 0x0124
	int mouthindex = 0;																	// 0x0128
	int numlocalposeparameters = 0;											// 0x012C
	int localposeparamindex = 0;												// 0x0130
	int surfacepropindex = 0;														// 0x0134
	int keyvalueindex = 0;															// 0x0138
	int keyvaluesize = 0;																// 0x013C
	int numlocalikautoplaylocks = 0;										// 0x0140
	int localikautoplaylockindex = 0;										// 0x0144
	float mass = 0.f;																		// 0x0148
	int contents = 0;																		// 0x014C
	int numincludemodels = 0;														// 0x0150
	int includemodelindex = 0;													// 0x0154
	void* virtualModel = nullptr;												// 0x0158
	int szanimblocknameindex = 0;												// 0x015C
	int numanimblocks = 0;															// 0x0160
	int animblockindex = 0;															// 0x0164
	void* animblockModel = nullptr;											// 0x0168
	int bonetablebynameindex = 0;												// 0x016C
	void* pVertexBase = nullptr;												// 0x0170
	void* pIndexBase = nullptr;													// 0x0174
	byte constdirectionallightdot = 0;									// 0x0178
	byte rootLOD = 0;																		// 0x0179
	byte unused[ 2 ] = { };															// 0x017A
	int zeroframecacheindex = 0;												// 0x017C
	int unused2[ 6 ] = { };															// 0x0180
};
// sizeof( studiohdr_t ) = 0x0198 ( 408 )

struct player_info_t
{
	char name[ 32 ] = { };																				// 0x0000
	int userID = 0;																								// 0x0020
	char guid[ 33 ] = { };																				// 0x0024
	FIELD_PAD( 0x0003 );																					// 0x0045
	unsigned int friendsID = 0;																		// 0x0048
	char friendsName[ 32 ] = { };																	// 0x004C
	bool fakeplayer = false;																			// 0x006C
	bool ishltv = false;																					// 0x006D
	FIELD_PAD( 0x0002 );																					// 0x006E
	unsigned long customFiles[ 4 ] = { };													// 0x0070
	unsigned char filesDownloaded = 0;														// 0x0080
	FIELD_PAD( 0x0003 );																					// 0x0081
};
// sizeof( player_info_t ) = 0x0084 ( 132 )

struct Ray_t
{
public:
	void Init( const Vector& start, const Vector& end )
	{
		m_Delta = ( end - start );
		m_IsSwept = ( m_Delta.LengthSqr() != 0.f );
		m_Extents.Init();
		m_IsRay = true;

		m_StartOffset.Init();
		m_Start = start;
	}

	void Init( const Vector& start, const Vector& end, const Vector& mins, const Vector& maxs )
	{
		m_Delta = ( end - start );
		m_IsSwept = ( m_Delta.LengthSqr() != 0.f );

		m_Extents = ( maxs - mins );
		m_Extents *= 0.5f;
		m_IsRay = ( m_Extents.LengthSqr() < 1e-6f );

		m_StartOffset = ( mins + maxs );
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.f;
	}

public:
	Vector m_Start = { };																					// 0x0000
	Vector m_Delta = { };																					// 0x000C
	Vector m_StartOffset = { };																		// 0x0018
	Vector m_Extents = { };																				// 0x0024
	bool m_IsRay = false;																					// 0x0030
	FIELD_PAD( 0x0003 );																					// 0x0031
	bool m_IsSwept = false;																				// 0x0034
	FIELD_PAD( 0x0003 );																					// 0x0035
};
// sizeof( Ray_t ) = 0x0038 ( 56 )

struct cplane_t
{
	Vector normal = { };																					// 0x0000
	float dist = 0.f;																							// 0x000C
	byte type = 0;																								// 0x0010
	byte signbits = 0;																						// 0x0011
	byte pad[ 2 ] = { };																					// 0x0012
};
// sizeof( cplane_t ) = 0x0014 ( 20 )

struct csurface_t
{
	const char* name = nullptr;																		// 0x0000
	short surfaceProps = 0;																				// 0x0004
	unsigned short flags = 0;																			// 0x0006
};
// sizeof( csurface_t ) = 0x0008 ( 8 )

// 
// [ class ] definition
// 
class InterfaceReg
{
public:
	void* Create();
	const char* GetName();
	InterfaceReg* GetNext();

public:
	InstantiateInterfaceFn m_CreateFn = nullptr;									// 0x0000
	const char* m_pName = nullptr;																// 0x0004
	InterfaceReg* m_pNext = nullptr;															// 0x0008
};
// sizeof( InterfaceReg ) = 0x000C ( 12 )

class RecvTable
{ };

class RecvProp
{ };

class ClientClass
{
public:
	CreateClientClassFn m_pCreateFn = nullptr;										// 0x0000
	CreateEventFn m_pCreateEventFn = nullptr;											// 0x0004
	const char* m_pNetworkName = nullptr;													// 0x0008
	RecvTable* m_pRecvTable = nullptr;														// 0x000C
	ClientClass* m_pNext = nullptr;																// 0x0010
	int m_ClassID = 0;																						// 0x0014
};
// sizeof( ClientClass ) = 0x0018 ( 24 )

class CBaseHandle
{
public:
	bool IsValid() const;

public:
	unsigned long m_Index = INVALID_EHANDLE_INDEX;								// 0x0000
};
// sizeof( CBaseHandle ) = 0x0004 ( 4 )

class CEntInfo
{
public:
	IHandleEntity* m_pEntity = nullptr;														// 0x0000
	int m_SerialNumber = 0;																				// 0x0004
	CEntInfo* m_pPrev = nullptr;																	// 0x0008
	CEntInfo* m_pNext = nullptr;																	// 0x000C
};
// sizeof( CEntInfo ) = 0x0010 ( 16 )

class CStudioHdr
{
public:
	studiohdr_t* m_pStudioHdr = nullptr;													// 0x0000
};
// sizeof( CStudioHdr ) = 0x0004 ( 4 )

class CBaseTrace
{
public:
	Vector startpos = { };																				// 0x0000
	Vector endpos = { };																					// 0x000C
	cplane_t plane = { };																					// 0x0018
	float fraction = 0.f;																					// 0x002C
	int contents = 0;																							// 0x0030
	unsigned short dispFlags = 0;																	// 0x0034
	bool allsolid = false;																				// 0x0036
	bool startsolid = false;																			// 0x0037
};
// sizeof( CBaseTrace ) = 0x0038 ( 56 )

class CGameTrace : public CBaseTrace
{
public:
	float fractionleftsolid = 0.f;																// 0x0038
	csurface_t surface = { };																			// 0x003C
	int hitgroup = 0;																							// 0x0044
	short physicsbone = 0;																				// 0x0048
	FIELD_PAD( 0x0002 );																					// 0x004A
	C_BaseEntity* m_pEnt = nullptr;																// 0x004C
	int hitbox = 0;																								// 0x0050
};
// sizeof( CGameTrace ) = 0x0054 ( 84 )

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity( IHandleEntity* pEntity, int contentsMask ) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	TraceType_t GetTraceType() const override
	{
		return TRACE_EVERYTHING;
	}
};

class CTraceFilterSimple : public CTraceFilter
{
public:
	CTraceFilterSimple( const IHandleEntity* passentity, int collisionGroup )
		: m_pPassEnt( passentity )
	{ }

public:
	bool ShouldHitEntity( IHandleEntity* pHandleEntity, int contentsMask ) override
	{
		return !( pHandleEntity != m_pPassEnt );
	}

public:
	const IHandleEntity* m_pPassEnt = nullptr;
};

class CTraceFilterSkipTwoEntities : public CTraceFilter
{
public:
	CTraceFilterSkipTwoEntities( const IHandleEntity* passentity,
															 const IHandleEntity* passentity2,
															 int collisionGroup )
		: m_pPassEnt( passentity )
		, m_pPassEnt2( passentity2 )
	{ }

public:
	bool ShouldHitEntity( IHandleEntity* pHandleEntity, int contentsMask ) override
	{
		return !( pHandleEntity != m_pPassEnt ||
							pHandleEntity != m_pPassEnt2 );
	}

public:
	const IHandleEntity* m_pPassEnt = nullptr;
	const IHandleEntity* m_pPassEnt2 = nullptr;
};

class IBaseClientDLL
{
public:
	ClientClass* GetAllClasses();
};

class CHLClient : public IBaseClientDLL
{ };

class IClientEntityList
{
public:
	IClientEntity* GetClientEntity( int entnum );
	IClientEntity* GetClientEntityFromHandle( CBaseHandle hEnt );
	int GetHighestEntityIndex();
};

class CClientEntityList : public IClientEntityList
{ };

class IVEngineClient
{
public:
	int GetLocalPlayer();
	void GetViewAngles( QAngle& va );
	void SetViewAngles( const QAngle& va );
};

class CEngineClient : public IVEngineClient
{ };

class IEngineTrace
{
public:
	void TraceRay( const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace );
};

class CEngineTraceClient : public IEngineTrace
{ };

class IRender
{
public:
	const VMatrix& WorldToScreenMatrix();
};

class CRender : public IRender
{ };