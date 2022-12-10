#include "../css/game.hpp"

/*
using CreateInterfaceFn = void* ( __cdecl* )( const char*, int* );
using InstantiateInterfaceFn = void* ( __cdecl* )();

#define SEH2_TRY()																		__try
#define SEH2_EXCEPT( ... )														__except( __VA_ARGS__ )

namespace atom::memory
{

#define INRANGE( x, a, b )														(x >= a && x <= b) 
#define getBits( x )																	(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )																	(getBits(x[0]) << 4 | getBits(x[1]))

PBYTE findPattern( const PBYTE rangeStart, const PBYTE rangeEnd, const char* pattern )
{
	const unsigned char* pat = reinterpret_cast< const unsigned char* >( pattern );
	PBYTE firstMatch = 0;
	for( PBYTE pCur = rangeStart; pCur < rangeEnd; ++pCur )
	{
		if( *( PBYTE )pat == ( BYTE )'\?' || *pCur == getByte( pat ) )
		{
			if( !firstMatch )
			{
				firstMatch = pCur;
			}
			pat += ( *( PWORD )pat == ( WORD )'\?\?' || *( PBYTE )pat != ( BYTE )'\?' ) ? 2 : 1;
			if( !*pat )
			{
				return firstMatch;
			}
			pat++;
			if( !*pat )
			{
				return firstMatch;
			}
		}
		else if( firstMatch )
		{
			pCur = firstMatch;
			pat = reinterpret_cast< const unsigned char* >( pattern );
			firstMatch = 0;
		}
	}
	return NULL;
}

std::uintptr_t ScanRegion( std::uintptr_t begin, std::uintptr_t end, const char* signature )
{
	return std::uintptr_t( findPattern( ( const PBYTE )begin, ( const PBYTE )end, signature ) );
}

std::uintptr_t Scan( std::uintptr_t image_base, const char* signature )
{
	const auto image_dos_header = PIMAGE_DOS_HEADER( image_base );

	if( image_dos_header->e_magic != IMAGE_DOS_SIGNATURE )
	{
		return 0;
	}

	const auto image_nt_headers = PIMAGE_NT_HEADERS( image_base + image_dos_header->e_lfanew );

	if( image_nt_headers->Signature != IMAGE_NT_SIGNATURE )
	{
		return 0;
	}

	return ScanRegion( image_base, ( image_base + image_nt_headers->OptionalHeader.SizeOfImage ), signature );
}

std::uintptr_t Scan( const wchar_t* image_name, const char* signature )
{
	const auto image_base = std::uintptr_t( GetModuleHandleW( image_name ) );

	if( !image_base )
	{
		return 0;
	}

	return Scan( image_base, signature );
}

template< typename T >
__declspec( naked ) T __cdecl Call( void* Target, UINT NumberOfParams, void* ProxyFFD5, ... )
{
	__asm mov edx, esp; // stackframe on edx - edx points now to the return adress - Target, NumberOfParams, ProxyFFD5, Params after.
	__asm push esp; // save esp
	__asm push ebp; // save ebp
	__asm push esi; // save esi

	__asm lea esi, [ edx + 8 ]; // Points to NumberOfParams
	__asm mov ecx, [ esi ]; // Get Number Of Params
	__asm inc ecx; // Also push ProxyFFD5

PushParams:
	__asm mov eax, [ esi + ecx * 4 ]; // Get Parameter from right to left
	__asm push eax; // and push them on the stack
	__asm loop PushParams; // Repeat until all params pushed (ecx != 0) - Stack is done after this

	__asm mov ebp, ReturnHere; // Mov Returnadress into ebp to be called by proxy
	__asm mov eax, [ edx + 4 ]; // Get Target
	__asm jmp eax; // jump to target

ReturnHere:
	__asm add esp, 4; // remove return address from Proxy
	__asm pop esi; // restore esi
	__asm pop ebp; // restore ebp
	__asm pop esp; // restore stack
	__asm retn; // return
}

} // namespace atom::memory

class InterfaceReg
{
public:
	InstantiateInterfaceFn m_CreateFn = nullptr;
	const char* m_pName = nullptr;
	InterfaceReg* m_pNext = nullptr;
};

using CreateClientClassFn = void* ( __cdecl* )( int entnum, int serialNum );
using CreateEventFn = void* ( __cdecl* )();

class Vector
{
public:
	float x, y, z;
};

class QAngle
{
public:
	float x, y, z;
};

class matrix3x4_t
{
public:
	float* operator[]( int index )
	{
		return m_flMatVal[ index ];
	}

	const float* operator[]( int index ) const
	{
		return m_flMatVal[ index ];
	}

public:
	float m_flMatVal[ 3 ][ 4 ] = { };
};

class VMatrix
{
public:
	float* operator[]( int index )
	{
		return m[ index ];
	}

	const float* operator[]( int index ) const
	{
		return m[ index ];
	}

public:
	float m[ 4 ][ 4 ] = { };
};

struct mstudiobone_t
{
	int			sznameindex;
	int			parent;
	int			bonecontroller[ 6 ];
	Vector		pos;
	float		quat[ 4 ];
	Vector		rot;
	Vector		posscale;
	Vector		rotscale;
	matrix3x4_t	poseToBone;
	float		qAlignment[ 4 ];
	int			flags;
	int			proctype;
	int			procindex;
	mutable int	physicsbone;
	int			surfacepropidx;
	int			contents;
	int			unused[ 8 ];

	const char* GetName() const
	{
		return ( ( const char* )this ) + sznameindex;
	}
};

struct studiohdr_t
{
	int			id;
	int			version;
	long		checksum;
	char		name[ 64 ];
	int			length;
	Vector		eyeposition;
	Vector		illumposition;
	Vector		hull_min;
	Vector		hull_max;
	Vector		view_bbmin;
	Vector		view_bbmax;
	int			flags;
	int			numbones;
	int			boneindex;
	int			numbonecontrollers;
	int			bonecontrollerindex;
	int			numhitboxsets;
	int			hitboxsetindex;
	int			numlocalanim;
	int			localanimindex;
	mutable int	activitylistversion;
	mutable int	eventsindexed;
	int			numtextures;
	int			textureindex;

	const char* pszName() const
	{
		return name;
	}

	mstudiobone_t* pBone( int i ) const
	{
		return ( mstudiobone_t* )( ( ( byte* )this ) + boneindex ) + i;
	}
};

class CStudioHdr
{
public:
	studiohdr_t* m_pStudioHdr = nullptr;
};

class CUserCmd
{
public:
	std::uint8_t __pad0[ 0x0064 ] = { };			// 0x0000
	unsigned int command_number = 0;					// 0x0064
	unsigned int command_number_key = 0;			// 0x0068
	std::uint8_t __pad1[ 0x0054 ] = { };			// 0x006C
	unsigned int random_seed = 0;							// 0x00C0
	unsigned int random_seed_key = 0;					// 0x00C4
	std::uint8_t __pad2[ 0x000C ] = { };			// 0x00C8
};
// sizeof( CUserCmd ) = 0x00D4

class CVerifiedUserCmd
{
public:
	CUserCmd m_cmd = { };											// 0x0000
	std::uint32_t m_crc = 0;									// 0x00D4
};
// sizeof( CUserCmd ) = 0x00D8

class CEngineClient
{
public:
	virtual void __pad0() = 0;
	virtual void __pad1() = 0;
	virtual void __pad2() = 0;
	virtual void __pad3() = 0;
	virtual void __pad4() = 0;
	virtual void __pad5() = 0;													// 5
	virtual void __pad6() = 0;
	virtual int GetLocalPlayer() = 0;
	virtual void __pad8() = 0;
	virtual void __pad9() = 0;
	virtual void __pad10() = 0;													// 10
	virtual void __pad11() = 0;
	virtual void __pad12() = 0;
	virtual void __pad13() = 0;
	virtual void __pad14() = 0;
	virtual void __pad15() = 0;													// 15
	virtual void __pad16() = 0;
	virtual void __pad17() = 0;
	virtual void __pad18() = 0;
	virtual void __pad19() = 0;
	virtual void __pad20() = 0;													// 20
	virtual void __pad21() = 0;
	virtual void __pad22() = 0;
	virtual void __pad23() = 0;
	virtual void __pad24() = 0;
	virtual void __pad25() = 0;													// 25
	virtual void __pad26() = 0;
	virtual void __pad27() = 0;
	virtual const VMatrix& WorldToScreenMatrix() = 0;
	virtual const VMatrix& WorldToViewMatrix() = 0;
};

class CRender
{
public:
	virtual void __pad0() = 0;
	virtual void __pad1() = 0;
	virtual void __pad2() = 0;
	virtual void __pad3() = 0;
	virtual void __pad4() = 0;
	virtual void __pad5() = 0;													// 5
	virtual void __pad6() = 0;
	virtual void __pad7() = 0;
	virtual void __pad8() = 0;
	virtual void __pad9() = 0;
	virtual void __pad10() = 0;													// 10
	virtual void __pad11() = 0;
	virtual void __pad12() = 0;
	virtual const VMatrix& WorldToScreenMatrix() = 0;		// 13
	virtual void __pad14() = 0;
	virtual void __pad15() = 0;
	virtual void __pad16() = 0;
	virtual void __pad17() = 0;
	virtual void __pad18() = 0;
	virtual void __pad19() = 0;
	virtual void __pad20() = 0;
	virtual void __pad21() = 0;
	virtual bool ScreenTransform( const Vector& point, Vector* pScreen ) = 0;
};

class CHLClient
{
public:
	virtual void __pad0() = 0;
	virtual void __pad1() = 0;
	virtual void __pad2() = 0;
	virtual void __pad3() = 0;
	virtual void __pad4() = 0;
	virtual void __pad5() = 0;
	virtual ClientClass* GetAllClasses() = 0;
};

class CInput
{
public:
	std::uint8_t __pad0[ 0x0110 ];											// 0x0000
	CUserCmd* m_pCommands = nullptr;										// 0x0110
	CVerifiedUserCmd* m_pVerifiedCommands = nullptr;		// 0x0114
};

class IHandleEntity;
class IClientUnknown;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;

class IHandleEntity
{
public:
	virtual void __pad0() = 0;
	virtual void __pad1() = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual void __pad2() = 0;
	virtual void __pad3() = 0;
	virtual void __pad4() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;	// 5
	virtual C_BaseEntity* GetBaseEntity() = 0;			// 6
	virtual void __pad7() = 0;
};

class IClientEntity : public IClientUnknown
{
public:
	virtual void __pad8() = 0;
	virtual const Vector& GetAbsOrigin() const = 0;			// 9
	virtual const QAngle& GetAbsAngles() const = 0;			// 10
	virtual void __pad11() = 0;
};

class C_BaseEntity : public IClientEntity
{
public:
	bool GetBoneCache( matrix3x4_t* bone_to_world, int max_bones )
	{
		const auto bone_count = *( int* )( std::uintptr_t( this ) + 0x10B8 );

		if( max_bones >= bone_count )
		{
			const auto bone_array = *( const void** )( std::uintptr_t( this ) + 0x10B4 );

			if( bone_array )
			{
				std::memcpy( bone_to_world, bone_array, sizeof( matrix3x4_t ) * bone_count );
				return true;
			}
		}

		return false;
	}

	CStudioHdr*& m_pStudioHdr()
	{
		return *( CStudioHdr** )( std::uintptr_t( this ) + 0x1F8C );
	}

	char& m_lifeState()
	{
		return *( char* )( std::uintptr_t( this ) + 0x439 );
	}

	int& m_iTeamNum()
	{
		return *( int* )( std::uintptr_t( this ) + 0x444 );
	}

	bool& m_bDormant()
	{
		return *( bool* )( std::uintptr_t( this ) + 0x4F6 );
	}
};

class CEntInfo
{
public:
	IHandleEntity* m_pEntity = nullptr;
	int m_SerialNumber = 0;
	CEntInfo* m_pPrev = nullptr;
	CEntInfo* m_pNext = nullptr;
};

namespace css::hooks
{

CUserCmd* __fastcall GetUserCmd( void* ecx, void* edx, int sequence_number, int a3, bool a4, int a5 );

HRESULT D3DAPI Present( IDirect3DDevice9* device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region );

} // namespace css::hooks

namespace css
{

std::uintptr_t m_image_process = 0;
std::uintptr_t m_image_client = 0;
std::uintptr_t m_image_engine = 0;
std::uintptr_t m_image_shaderapidx9 = 0;

CRender* m_render = nullptr;
CEngineClient* m_engine_client = nullptr;
CInput* m_input = nullptr;

IDirect3DDevice9* m_device = nullptr;
IDirect3DStateBlock9* m_state_block = nullptr;

using PresentFn = HRESULT( D3DAPI* )( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* );
PresentFn m_present = nullptr;

using GetUserCmdFn = CUserCmd* ( __thiscall* )( void*, int, int, bool, int );
GetUserCmdFn m_get_user_cmd = nullptr;

bool Create()
{
	auto status = MH_Initialize();

	if( status != MH_OK )
	{
		return false;
	}

	m_image_process = std::uintptr_t( GetModuleHandleW( nullptr ) );
	m_image_client = std::uintptr_t( GetModuleHandleW( L"client.dll" ) );
	m_image_engine = std::uintptr_t( GetModuleHandleW( L"engine.dll" ) );
	m_image_shaderapidx9 = std::uintptr_t( GetModuleHandleW( L"shaderapidx9.dll" ) );

	if( !m_image_process ||
			!m_image_client ||
			!m_image_engine ||
			!m_image_shaderapidx9 )
	{
		return false;
	}

	m_render = *( CRender** )( m_image_engine + 0x00448568 );

	if( !m_render )
	{
		return false;
	}

	m_engine_client = *( CEngineClient** )( m_image_engine + 0x0043A3B4 );

	if( !m_engine_client )
	{
		return false;
	}

	m_input = *( CInput** )( m_image_client + 0x005DBAD0 );

	if( !m_input )
	{
		return false;
	}

	const auto device_encrypted = *( std::uintptr_t* )( m_image_shaderapidx9 + 0x002360D8 );

	if( !device_encrypted )
	{
		return false;
	}

	m_device = ( IDirect3DDevice9* )( device_encrypted ^ 0xAA05351F );

	if( !m_device )
	{
		return false;
	}

	if( FAILED( m_device->CreateStateBlock( D3DSBT_ALL, &m_state_block ) ) )
	{
		return false;
	}

	auto vmt = *( void*** )( m_device );

	if( !vmt )
	{
		return false;
	}

	status = MH_CreateHook( vmt[ 17 ], &hooks::Present, ( void** )&m_present );

	if( status != MH_OK )
	{
		return false;
	}

	status = MH_EnableHook( vmt[ 17 ] );

	if( status != MH_OK )
	{
		return false;
	}

	return true;
}

void Destroy()
{
	MH_Uninitialize();
}

CEntInfo* GetEntInfo( int entnum )
{
	const auto base = ( m_image_client + 0x626ED4 );

	if( entnum >= 0 && entnum < 4096 )
	{
		return ( CEntInfo* )( base + sizeof( CEntInfo ) * entnum - 0x100E8 );
	}

	return nullptr;
}

IClientEntity* GetClientEntity( int entnum )
{
	const auto ent_info = GetEntInfo( entnum );

	if( ent_info )
	{
		const auto entity = ( IClientUnknown* )ent_info->m_pEntity;

		if( entity )
		{
			return entity->GetIClientEntity();
		}
	}

	return nullptr;
}

} // namespace css

namespace css::hooks
{

struct Vertex
{
	Vertex( float _x, float _y, float _z, D3DCOLOR _color )
		: x( _x )
		, y( _y )
		, z( _z )
		, color( _color )
	{ }

	float x;
	float y;
	float z;
	float rhw = 1.f;
	D3DCOLOR color;

	enum
	{
		FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
	};
};

void DrawLine( int x0, int y0, int x1, int y1, D3DCOLOR color )
{
	Vertex vertices[ 2 ] =
	{
		Vertex( x0, y0, 1.0f, color ),
		Vertex( x1, y1, 1.0f, color )
	};

	m_device->SetFVF( Vertex::FVF );
	m_device->SetTexture( 0, nullptr );

	m_device->DrawPrimitiveUP( D3DPT_LINELIST, 2, vertices, sizeof( Vertex ) );
}

void DrawLine( const Vector& begin, const Vector& end, D3DCOLOR color )
{
	DrawLine( static_cast< int >( begin.x ),
						static_cast< int >( begin.y ),
						static_cast< int >( end.x ),
						static_cast< int >( end.y ),
						color );
}

void DrawRect( int x, int y, int w, int h, D3DCOLOR color )
{
	Vertex vertices[ 4 ] =
	{
		Vertex( x, y + h, 1.0f, color ),
		Vertex( x, y, 1.0f, color ),
		Vertex( x + w, y + h, 1.0f, color ),
		Vertex( x + w, y, 1.0f, color )
	};

	m_device->SetFVF( Vertex::FVF );
	m_device->SetTexture( 0, nullptr );
	m_device->SetPixelShader( nullptr );

	m_device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertices, sizeof( Vertex ) );
}

void DrawRect( const Vector& position, float w, float h, D3DCOLOR color )
{
	DrawRect( static_cast< int >( position.x ),
						static_cast< int >( position.y ),
						static_cast< int >( w ),
						static_cast< int >( h ),
						color );
}

void DrawBox( int x, int y, int w, int h, D3DCOLOR color )
{
	DrawRect( x, y, w, 1, color );
	DrawRect( x, y, 1, h, color );

	DrawRect( x + w, y, 1, h, color );
	DrawRect( x, y + h, w + 1, 1, color );
}

void DrawBoxBorder( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR border = D3DCOLOR_RGBA( 0, 0, 0, 255 ) )
{
	DrawBox( x, y, w, h, color );
	DrawBox( x - 1, y - 1, w + 2, h + 2, border );
	DrawBox( x + 1, y + 1, w - 2, h - 2, border );
}

void DrawBoxBorder( const Vector& position, float w, float h, D3DCOLOR color, D3DCOLOR border = D3DCOLOR_RGBA( 0, 0, 0, 255 ) )
{
	DrawBoxBorder( static_cast< int >( position.x ),
								 static_cast< int >( position.y ),
								 static_cast< int >( w ),
								 static_cast< int >( h ),
								 color,
								 border );
}

bool WorldToScreen( const Vector& point, Vector& screen )
{
	auto ptr = 0x18C * 2 + *( std::uintptr_t* )( std::uintptr_t( m_render ) + 0xDC ) - 0x44;
	auto& mat = *( VMatrix* )( ptr );

	screen.x = mat[ 0 ][ 0 ] * point.x + mat[ 0 ][ 1 ] * point.y + mat[ 0 ][ 2 ] * point.z + mat[ 0 ][ 3 ];
	screen.y = mat[ 1 ][ 0 ] * point.x + mat[ 1 ][ 1 ] * point.y + mat[ 1 ][ 2 ] * point.z + mat[ 1 ][ 3 ];

	auto w = mat[ 3 ][ 0 ] * point.x + mat[ 3 ][ 1 ] * point.y + mat[ 3 ][ 2 ] * point.z + mat[ 3 ][ 3 ];

	if( w < 0.001f )
		return false;

	auto invw = 1.0f / w;

	screen.x *= invw;
	screen.y *= invw;

	D3DVIEWPORT9 viewport = { };
	m_device->GetViewport( &viewport );

	auto x = viewport.Width * 0.5f;
	auto y = viewport.Height * 0.5f;

	x += 0.5f * screen.x * viewport.Width + 0.5f;
	y -= 0.5f * screen.y * viewport.Height + 0.5f;

	screen.x = x;
	screen.y = y;

	return true;
}

void MatrixOrigin( const matrix3x4_t& transform, Vector& origin )
{
	origin.x = transform.m_flMatVal[ 0 ][ 3 ];
	origin.y = transform.m_flMatVal[ 1 ][ 3 ];
	origin.z = transform.m_flMatVal[ 2 ][ 3 ];
}

void RenderBoneArray( C_BaseEntity* entity )
{
	const auto model = entity->m_pStudioHdr();

	if( model )
	{
		const auto studio = model->m_pStudioHdr;

		if( studio )
		{
			matrix3x4_t bone_transform[ 128 ] = { };

			if( entity->GetBoneCache( bone_transform, 128 ) )
			{
				for( int j = 0; j < studio->numbones; j++ )
				{
					const auto bone = studio->pBone( j );

					if( bone->parent != -1 && bone->flags & 0x00000100 )
					{
						Vector origin_begin = { };
						MatrixOrigin( bone_transform[ j ], origin_begin );

						Vector origin_end = { };
						MatrixOrigin( bone_transform[ bone->parent ], origin_end );

						Vector begin = { };
						Vector end = { };

						if( WorldToScreen( origin_begin, begin ) &&
								WorldToScreen( origin_end, end ) )
						{
							DrawLine( begin, end, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
						}
					}
				}
			}
		}
	}
}

std::uint32_t GetChecksum( CUserCmd* cmd )
{
	using GetChecksumFn = std::uint32_t( __thiscall* )( CUserCmd* );
	const auto get_checksum = GetChecksumFn( m_image_client + 0x002C7E00 );
	return get_checksum( cmd );
}

CUserCmd* __fastcall GetUserCmd( void* ecx, void* edx, int sequence_number, int a3, bool a4, int a5 )
{
	auto cmd = m_get_user_cmd( ecx, sequence_number, a3, a4, a5 );

	if( cmd )
	{
		const auto seed = ( 141 ^ 0xDDD4B780 );
		const auto seed_array = ( unsigned int* )( m_image_client + 0x005197D8 );

		cmd->random_seed = seed;
		cmd->random_seed_key = seed_array[ ( std::uint8_t )seed ^ 235 ] ^ ( ( int )( seed ^ 0x1DE315EB ) >> 8 );

		auto number = ( cmd->command_number ^ 0xDDD4B780 );

		while( true )
		{
			const auto seed_number = ( MD5_PseudoRandom( number ) & 255 ) ^ 0xDDD4B780;

			if( seed_number == seed )
			{
				break;
			}

			number++;

			cmd->command_number = ( number ^ 0xDDD4B780 );
			cmd->command_number_key = seed_array[ ( std::uint8_t )cmd->command_number ^ 235 ] ^ ( ( cmd->command_number ^ 0x1DE315EB ) >> 8 );
		}

		// TRACE( "num = %d", int( sequence_number ^ 0xDDD4B780 ) );

		auto verified_cmd = &m_input->m_pVerifiedCommands[ (( sequence_number ^ 0xDDD4B780 ) - 1)% 90 ];

		if( verified_cmd )
		{
			std::memcpy( &verified_cmd->m_cmd, cmd, sizeof( CUserCmd ) );
			verified_cmd->m_crc = GetChecksum( cmd );
		}

		// TRACE( "command_number = %u", cmd->command_number );
		// TRACE( "random_seed = %u", ( cmd->random_seed ^ 0xDDD4B780 ) & 255 );
		// TRACE( "random_seed_key = %u", cmd->random_seed_key );
	}

	return cmd;
}

void RenderScene()
{
	const auto local_index = m_engine_client->GetLocalPlayer();

	if( local_index )
	{
		const auto local_player = ( C_BaseEntity* )GetClientEntity( local_index );

		if( local_player )
		{
			for( int index = 1; index <= 64; index++ )
			{
				const auto entity = ( C_BaseEntity* )GetClientEntity( index );

				if( !entity )
				{
					continue;
				}

				if( entity->m_lifeState() != 0 )
				{
					continue;
				}

				if( entity->m_bDormant() )
				{
					continue;
				}

				if( entity->m_iTeamNum() == local_player->m_iTeamNum() )
				{
					continue;
				}

				RenderBoneArray( entity );
			}
		}
	}
}

HRESULT D3DAPI Present( IDirect3DDevice9* device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region )
{
	m_state_block->Capture();

	m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_device->SetRenderState( D3DRS_ZENABLE, FALSE );
	m_device->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	m_device->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_device->SetRenderState( D3DRS_ALPHAREF, 0x08 );
	m_device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	m_device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_device->SetRenderState( D3DRS_STENCILENABLE, FALSE );
	m_device->SetRenderState( D3DRS_CLIPPING, TRUE );
	m_device->SetRenderState( D3DRS_CLIPPLANEENABLE, FALSE );
	m_device->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );
	m_device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
	m_device->SetRenderState( D3DRS_FOGENABLE, FALSE );
	m_device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
	m_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	m_device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_device->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	m_device->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	m_device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	m_device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
	m_device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	m_device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	m_device->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	m_device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
	m_device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, NULL );
	m_device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
	m_device->SetPixelShader( nullptr );
	m_device->SetVertexShader( nullptr );

	SEH2_TRY()
	{
		RenderScene();
	}
	SEH2_EXCEPT( EXCEPTION_EXECUTE_HANDLER )
	{

	}

	m_state_block->Apply();

	return m_present( device, source_rect, dest_rect, dest_window_override, dirty_region );
}

} // namespace css::hooks
*/

namespace atom
{

ULONG WINAPI ImageExecute( void* parameter )
{
	const auto image = static_cast< HMODULE >( parameter );
	TRACE( "image = 0x%08X", std::uintptr_t( image ) );

	SEH2_TRY()
	{
		if( css::Create() )
		{
			while( css::m_execute.load() )
			{
				if( GetAsyncKeyState( VK_F11 ) )
				{
					css::m_execute.store( false );
					break;
				}

				std::this_thread::sleep_for( 100ms );
			}
		}
	}
	SEH2_EXCEPT( EXCEPTION_EXECUTE_HANDLER )
	{
		TRACE( "Exception occurred! (0x%08X)", GetExceptionCode() );
	}

	TRACE( "Going to unload..." );
	css::Destroy();
	
	TRACE( "Unloading..." );
	FreeLibraryAndExitThread( image, EXIT_SUCCESS );
	return EXIT_SUCCESS;
}

void ImageLoad( void* instance )
{
	const auto thread = CreateThread( nullptr, 0, &ImageExecute, instance, 0, nullptr );
	TRACE( "thread = 0x%08X", __FUNCTION__, std::uintptr_t( thread ) );

	if( thread && thread != INVALID_HANDLE_VALUE )
	{
		CloseHandle( thread );
	}
}

void ImageFree( void* reserved )
{ }

int Dispatch( void* instance, std::uint32_t reason, void* reserved )
{
	switch( reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			ImageLoad( instance );
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			ImageFree( reserved );
			break;
		}
	}

	return TRUE;
}

} // namespace atom

int WINAPI DllMain( void* instance, unsigned long reason, void* reserved )
{
	return atom::Dispatch( instance, reason, reserved );
}