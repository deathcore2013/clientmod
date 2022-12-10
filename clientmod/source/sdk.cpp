#include "sdk.hpp"

#include "../css/game.hpp"

// 
// [ function ] implementation
// 
std::uint32_t Hash( const char* data )
{
	auto hash = 0x811C9DC5U;
	auto character = data[ 0 ];

	if( character )
	{
		auto current = ( data + 1 );

		do
		{
			auto v15 = character;
			auto v16 = character | 0x20;
			auto v10 = uint8_t( character - 0x41 ) <= 0x19;
			character = current[ 0 ];
			if( !v10 )
			{
				v16 = v15;
			}
			hash = 0x01000193U * ( hash ^ v16 );
			current++;
		}
		while( character );
	}

	return hash;
}

// 
// [ mstudiobone_t ] implementation
// 
const char* mstudiobone_t::pszName() const
{
	if( sznameindex == 0 )
	{
		return "";
	}

	return ( reinterpret_cast< const char* >( this ) + sznameindex );
}

// 
// [ mstudiobbox_t ] implementation
// 
const char* mstudiobbox_t::pszHitboxName() const
{
	if( szhitboxnameindex == 0 )
	{
		return "";
	}

	return ( reinterpret_cast< const char* >( this ) + szhitboxnameindex );
}

// 
// [ mstudiohitboxset_t ] implementation
// 
const char* mstudiohitboxset_t::pszName() const
{
	if( sznameindex == 0 )
	{
		return "";
	}

	return ( reinterpret_cast< const char* >( this ) + sznameindex );
}

mstudiobbox_t* mstudiohitboxset_t::pHitbox( int index ) const
{
	if( index < 0 || index >= numhitboxes )
	{
		return nullptr;
	}

	const auto base = const_cast< mstudiohitboxset_t* >( this );
	return ( reinterpret_cast< mstudiobbox_t* >( reinterpret_cast< byte* >( base ) + hitboxindex ) + index );
}

// 
// [ studiohdr_t ] implementation
// 
const char* studiohdr_t::pszName() const
{
	return name;
}

mstudiobone_t* studiohdr_t::pBone( int index ) const
{
	if( index < 0 || index >= numbones )
	{
		return nullptr;
	}

	const auto base = const_cast< studiohdr_t* >( this );
	return ( reinterpret_cast< mstudiobone_t* >( reinterpret_cast< byte* >( base ) + boneindex ) + index );
}

mstudiohitboxset_t* studiohdr_t::pHitboxSet( int index ) const
{
	if( index < 0 || index >= numhitboxsets )
	{
		return nullptr;
	}

	const auto base = const_cast< studiohdr_t* >( this );
	return ( reinterpret_cast< mstudiohitboxset_t* >( reinterpret_cast< byte* >( base ) + hitboxsetindex ) + index );
}

// 
// [ InterfaceReg ] implementation
// 
void* InterfaceReg::Create()
{
	const auto name_hash = Hash( m_pName );
	return InstantiateInterfaceFn( memory::ToUIntPointer( m_CreateFn ) + name_hash )();
}

const char* InterfaceReg::GetName()
{
	return m_pName;
}

InterfaceReg* InterfaceReg::GetNext()
{
	const auto name_hash = Hash( m_pName );
	return reinterpret_cast< InterfaceReg* >( memory::ToUIntPointer( m_pNext ) + name_hash );
}

// 
// [ CBaseHandle ] implementation
// 
bool CBaseHandle::IsValid() const
{
	return ( m_Index != INVALID_EHANDLE_INDEX );
}

// 
// [ IBaseClientDLL ] implementation
// 
ClientClass* IBaseClientDLL::GetAllClasses()
{
	return memory::CallMethod< 6, ClientClass* >( this );
}

// 
// [ IClientEntityList ] implementation
// 
IClientEntity* IClientEntityList::GetClientEntity( int entnum )
{
	const auto key_array = reinterpret_cast< int* >( css::m_image_client + 0x006E2550 );
	
	entnum ^= 0xDD7F30B5;
	const auto key = key_array[ uint8( entnum ) ^ 237 ] ^ ( ( entnum ^ 0x1DA454ED ) >> 8 );

	return memory::CallMethod< 33, IClientEntity* >( this, entnum, key );
}

IClientEntity* IClientEntityList::GetClientEntityFromHandle( CBaseHandle hEnt )
{
	return memory::CallMethod< 39, IClientEntity* >( this, hEnt );
}

int IClientEntityList::GetHighestEntityIndex()
{
	// return memory::CallMethod< 6, int >( this );
	const auto max_user_server_index = *reinterpret_cast< int64* >( memory::ToUIntPointer( this ) + 0x00E0 );
	return static_cast< int >( max_user_server_index );
}

// 
// [ IVEngineClient ] implementation
// 
int IVEngineClient::GetLocalPlayer()
{
	return memory::CallMethod< 13, int >( this );
}

void IVEngineClient::GetViewAngles( QAngle& va )
{
	// return memory::CallMethod< 60, void >( this, std::ref( va ) );
}

void IVEngineClient::SetViewAngles( const QAngle& va )
{
	auto key_array = reinterpret_cast< int* >( css::m_image_engine + 0x00352DF8 );

	auto x = *reinterpret_cast< const int* >( &va.x );
	x ^= 0xDC1E084E;

	auto y = *reinterpret_cast< const int* >( &va.y );
	y ^= 0xDC1E084E;

	auto z = *reinterpret_cast< const int* >( &va.z );
	z ^= 0xDC1E084E;

	auto key_x = key_array[ ( unsigned __int8 )( int )*( float* )&x ^ 235 ] ^ ( ( ( int )*( float* )&x ^ 0x1DE315EB ) >> 8 );
	auto key_y = key_array[ ( unsigned __int8 )( int )*( float* )&y ^ 235 ] ^ ( ( ( int )*( float* )&y ^ 0x1DE315EB ) >> 8 );
	auto key_z = key_array[ ( unsigned __int8 )( int )*( float* )&z ^ 235 ] ^ ( ( ( int )*( float* )&z ^ 0x1DE315EB ) >> 8 );

	int arg_pack[ 6 ] =
	{
		x, key_x,
		y, key_y,
		z, key_z,
	};

	return memory::CallMethod< 107, void >( this, arg_pack );


	/*

	auto x = *reinterpret_cast< const int* >( &va.x );
	x ^= 0xDC1E084E;
	
	auto y = *reinterpret_cast< const int* >( &va.y );
	y ^= 0xDC1E084E;

	auto z = *reinterpret_cast< const int* >( &va.z );
	z ^= 0xDC1E084E;

	auto x_key = key_array[ ( unsigned __int8 )x ^ 235 ] ^ ( ( x ^ 0x1DE315EB ) >> 8 );
	auto y_key = key_array[ ( unsigned __int8 )y ^ 235 ] ^ ( ( y ^ 0x1DE315EB ) >> 8 );
	auto z_key = key_array[ ( unsigned __int8 )z ^ 235 ] ^ ( ( z ^ 0x1DE315EB ) >> 8 );

	auto viewangles = reinterpret_cast< int* >( css::m_image_engine + 0x0051816C );

	viewangles[ 0 ] = x;
	viewangles[ 1 ] = x_key;

	viewangles[ 2 ] = y;
	viewangles[ 3 ] = y_key;

	viewangles[ 4 ] = z;
	viewangles[ 5 ] = z_key;
	*/
}

// 
// [ IEngineTrace ] implementation
// 
void IEngineTrace::TraceRay( const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace )
{
	return memory::CallMethod< 5, void >( this, std::cref( ray ), fMask, pTraceFilter, pTrace );
}

// 
// [ IRender ] implementation
// 
const VMatrix& IRender::WorldToScreenMatrix()
{
	const auto view_stack = *reinterpret_cast< std::uintptr_t* >( memory::ToUIntPointer( this ) + 0x00DC );
	return *reinterpret_cast< const VMatrix* >( view_stack + ( 0x018C * 2 ) - 0xC4 );
}