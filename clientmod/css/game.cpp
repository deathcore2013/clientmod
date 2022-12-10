#include "game.hpp"

#include "hooks/present.hpp"
#include "hooks/reset.hpp"
#include "hooks/process_message.hpp"

namespace stdcall
{

template< typename ReturnType >
__declspec( naked ) ReturnType __cdecl Call( std::uintptr_t pointer,						// + 4
																						 std::size_t number_of_parameters,	// + 8
																						 std::uintptr_t gadget,							// + 12
																						 ... )
{
	__asm mov edx, esp;
	__asm push esp;
	__asm push ebp;
	__asm push esi;

	__asm lea esi, [ edx + 8 ];
	__asm mov ecx, [ esi ];
	__asm inc ecx;

PushParameters:
	__asm mov eax, [ esi + ecx * 4 ];
	__asm push eax;
	__asm loop PushParameters;

	__asm mov ebp, ReturnLocation;
	__asm mov eax, [ edx + 4 ];
	__asm jmp eax;

ReturnLocation:
	__asm add esp, 4;
	__asm pop esi;
	__asm pop ebp;
	__asm pop esp;
	__asm retn;
}

} // namespace fastcall

extern void* FindClassInstance( std::uintptr_t image_base, const char* class_name );
extern void DumpRunTimeTypeInformation( std::uintptr_t image_base );

namespace css
{

std::atomic< bool > m_execute = true;

std::uintptr_t m_image_process = 0;
std::uintptr_t m_image_client = 0;
std::uintptr_t m_image_engine = 0;
std::uintptr_t m_image_shaderapidx9 = 0;

CHLClient* m_client = nullptr;
CClientEntityList* m_client_entity_list = nullptr;
CEngineClient* m_engine_client = nullptr;
CEngineTraceClient* m_engine_trace_client = nullptr;
CRender* m_render = nullptr;

IDirect3DDevice9* m_device = nullptr;

PresentFn m_present = nullptr;
ResetFn m_reset = nullptr;

WNDPROC m_process_message = nullptr;

IDirect3DDevice9* GetDevice()
{
	static std::uintptr_t pointer = 0;
	static std::uintptr_t xor_key = 0;

	/*if( !memory::IsUserMode( pointer ) || !xor_key )
	{
		std::vector< std::uintptr_t > occurrences = { };

		if( !memory::SearchSection( m_image_shaderapidx9, ".text", "8B 0D ? ? ? ? 81 F1 ? ? ? ? 6A 00 57", occurrences ) )
		{
			TRACE( "%s: IDirect3DDevice9 instance and xor key are not found!", __FUNCTION__ );
			return nullptr;
		}

		pointer = *reinterpret_cast< std::uintptr_t* >( occurrences[ 0 ] + 2 );
		xor_key = *reinterpret_cast< std::uintptr_t* >( occurrences[ 0 ] + 8 );

		if( !memory::IsUserMode( pointer ) || !xor_key )
		{
			TRACE( "%s: IDirect3DDevice9 instance and xor key are not valid!", __FUNCTION__ );
			return nullptr;
		}
	}*/

	pointer = ( m_image_shaderapidx9 + 0x00258C50 );
	const auto device = *reinterpret_cast< std::uintptr_t* >( pointer );

	if( !device )
	{
		TRACE( "%s: IDirect3DDevice9 instance is not valid!", __FUNCTION__ );
		return nullptr;
	}

	xor_key = 0xBBDC0B80;
	return reinterpret_cast< IDirect3DDevice9* >( device ^ xor_key );
}

InterfaceReg* GetInterfaceList( std::uintptr_t image_base )
{
	const auto create_interface = win32::GetImageExport( image_base, "CreateInterface" );

	if( !memory::IsUserMode( create_interface ) )
	{
		return nullptr;
	}

	const auto code = reinterpret_cast< std::uint8_t* >( create_interface );

	for( std::size_t index = 0; index < 256; index++ )
	{
		if( code[ index + 0 ] == 0x8B && code[ index + 1 ] == 0x3D &&	// mov esi,
				code[ index + 6 ] == 0x33 && code[ index + 7 ] == 0xC9 )	// xor ecx, ecx
		{
			return **reinterpret_cast< InterfaceReg*** >( &code[ index + 2 ] );
		}
	}

	return nullptr;
}

void* GetInterface( std::uintptr_t image_base, const char* name )
{
	auto reg = GetInterfaceList( image_base );

	while( memory::IsUserMode( reg ) )
	{
		const auto reg_name = reg->GetName();

		if( memory::IsUserMode( reg_name ) )
		{
			const auto reg_name_length = ( strlen( reg_name ) - 3 );

			if( _strnicmp( reg_name, name, reg_name_length ) == 0 )
			{
				return reg->Create();
			}
		}

		reg = reg->GetNext();
	}

	return nullptr;
}

void DumpInterfaceLinkedList( std::uintptr_t image_base )
{
	auto reg = GetInterfaceList( image_base );
	TRACE( "reg = 0x%08X", memory::ToUIntPointer( reg ) );

	while( memory::IsUserMode( reg ) )
	{
		const auto name = reg->GetName();

		if( memory::IsUserMode( name ) )
		{
			const auto instance = reg->Create();
			TRACE( "%s = 0x%08X", name, memory::ToUIntPointer( instance ) );
		}

		reg = reg->GetNext();
	}
}

bool Create()
{
	auto& renderer = graphics::Renderer::Instance();
	m_execute.store( true );

	auto status = MH_Initialize();

	if( status != MH_OK )
	{
		TRACE( "%s: MH_Initialize() error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
		return false;
	}
	
	m_image_process = win32::GetImageBase( nullptr );
	TRACE( "m_image_process = 0x%08X", m_image_process );

	if( !m_image_process )
	{
		TRACE( "%s: m_image_process is not valid!", __FUNCTION__ );
		return false;
	}

	m_image_client = win32::GetImageBase( L"client.dll" );
	TRACE( "m_image_client = 0x%08X", m_image_client );

	if( !m_image_client )
	{
		TRACE( "%s: m_image_client is not valid!", __FUNCTION__ );
		return false;
	}

	m_image_engine = win32::GetImageBase( L"engine.dll" );
	TRACE( "m_image_engine = 0x%08X", m_image_engine );

	if( !m_image_engine )
	{
		TRACE( "%s: m_image_engine is not valid!", __FUNCTION__ );
		return false;
	}

	m_image_shaderapidx9 = win32::GetImageBase( L"shaderapidx9.dll" );
	TRACE( "m_image_shaderapidx9 = 0x%08X", m_image_shaderapidx9 );

	if( !m_image_shaderapidx9 )
	{
		TRACE( "%s: m_image_shaderapidx9 is not valid!", __FUNCTION__ );
		return false;
	}

	TRACE( "============================================================" );
	DumpInterfaceLinkedList( m_image_client );
	TRACE( "============================================================" );
	DumpInterfaceLinkedList( m_image_engine );
	TRACE( "============================================================" );

	m_client = reinterpret_cast< CHLClient* >( GetInterface( m_image_client, "VClient" ) );
	TRACE( "m_client = 0x%08X", memory::ToUIntPointer( m_client ) );

	if( !memory::IsUserMode( m_client ) )
	{
		TRACE( "%s: m_client is not valid!", __FUNCTION__ );
		return false;
	}

	m_client_entity_list = reinterpret_cast< CClientEntityList* >( GetInterface( m_image_client, "VClientEntityList" ) );
	TRACE( "m_client_entity_list = 0x%08X", memory::ToUIntPointer( m_client_entity_list ) );

	if( !memory::IsUserMode( m_client_entity_list ) )
	{
		TRACE( "%s: m_client_entity_list is not valid!", __FUNCTION__ );
		return false;
	}

	m_engine_client = reinterpret_cast< CEngineClient* >( GetInterface( m_image_engine, "VEngineClient" ) );
	TRACE( "m_engine_client = 0x%08X", memory::ToUIntPointer( m_engine_client ) );

	if( !memory::IsUserMode( m_engine_client ) )
	{
		TRACE( "%s: m_engine_client is not valid!", __FUNCTION__ );
		return false;
	}

	m_engine_trace_client = reinterpret_cast< CEngineTraceClient* >( GetInterface( m_image_engine, "EngineTraceClient" ) );
	TRACE( "m_engine_trace_client = 0x%08X", memory::ToUIntPointer( m_engine_trace_client ) );

	if( !memory::IsUserMode( m_engine_trace_client ) )
	{
		TRACE( "%s: m_engine_trace_client is not valid!", __FUNCTION__ );
		return false;
	}

	m_render = reinterpret_cast< CRender* >( FindClassInstance( m_image_engine, ".?AVCRender@@" ) );
	TRACE( "m_render = 0x%08X", memory::ToUIntPointer( m_render ) );

	if( !memory::IsUserMode( m_render ) )
	{
		TRACE( "%s: m_render is not valid!", __FUNCTION__ );
		return false;
	}

	m_device = GetDevice();
	TRACE( "m_device = 0x%08X", memory::ToUIntPointer( m_device ) );

	if( !memory::IsUserMode( m_device ) )
	{
		TRACE( "%s: m_device is not valid!", __FUNCTION__ );
		return false;
	}

	if( !renderer.Create( m_device ) )
	{
		TRACE( "%s: atom::graphics::Renderer::Create(...) error!", __FUNCTION__ );
		return false;
	}

	const auto vmt = *reinterpret_cast< void*** >( m_device );
	TRACE( "vmt = 0x%08X", memory::ToUIntPointer( vmt ) );

	status = MH_CreateHook( vmt[ 16 ], &hooks::Reset, reinterpret_cast< void** >( &m_reset ) );

	if( status != MH_OK )
	{
		TRACE( "%s: MH_CreateHook(...) error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
		return false;
	}

	status = MH_QueueEnableHook( vmt[ 16 ] );

	if( status != MH_OK )
	{
		TRACE( "%s: MH_QueueEnableHook(...) error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
		return false;
	}

	status = MH_CreateHook( vmt[ 17 ], &hooks::Present, reinterpret_cast< void** >( &m_present ) );

	if( status != MH_OK )
	{
		TRACE( "%s: MH_CreateHook(...) error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
		return false;
	}

	status = MH_QueueEnableHook( vmt[ 17 ] );

	if( status != MH_OK )
	{
		TRACE( "%s: MH_QueueEnableHook(...) error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
		return false;
	}

	status = MH_ApplyQueued();

	if( status != MH_OK )
	{
		TRACE( "%s: MH_ApplyQueued() error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
		return false;
	}

	const auto window = renderer.GetWindow();
	TRACE( "window = 0x%08X", memory::ToUIntPointer( window ) );
	
	m_process_message = reinterpret_cast< WNDPROC >( GetWindowLongPtrA( window, GWLP_WNDPROC ) );
	TRACE( "m_process_message = 0x%08X", memory::ToUIntPointer( m_process_message ) );

	SetWindowLongPtrA( window, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( &hooks::ProcessMessage ) );
	return true;
}

void Destroy()
{
	auto& renderer = graphics::Renderer::Instance();

	auto status = MH_OK;

	if( m_process_message )
	{
		const auto window = renderer.GetWindow();
		SetWindowLongPtrA( window, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( m_process_message ) );
		
		// 
		// Wait 250 ms for hook to complete
		// 
		std::this_thread::sleep_for( 250ms );
		m_process_message = nullptr;
	}

	if( memory::IsUserMode( m_device ) )
	{
		const auto vmt = *reinterpret_cast< void*** >( m_device );

		if( memory::IsUserMode( vmt ) )
		{
			status = MH_QueueDisableHook( vmt[ 16 ] );

			if( status != MH_OK )
			{
				TRACE( "%s: MH_QueueDisableHook(...) error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
			}

			status = MH_QueueDisableHook( vmt[ 17 ] );

			if( status != MH_OK )
			{
				TRACE( "%s: MH_QueueDisableHook(...) error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
			}

			status = MH_ApplyQueued();

			if( status != MH_OK )
			{
				TRACE( "%s: MH_ApplyQueued() error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
			}

			// 
			// Wait 250 ms for hooks to complete
			// 
			std::this_thread::sleep_for( 250ms );

			// 
			// Destroy d3d9 rendering system
			// 
			renderer.Destroy();
		}

		m_device = nullptr;
	}

	status = MH_Uninitialize();

	if( status != MH_OK )
	{
		TRACE( "%s: MH_Uninitialize() error! (%s)", __FUNCTION__, MH_StatusToString( status ) );
	}
}

bool WorldToScreen( const Vector& origin, ImVec2& screen )
{
	const auto& io = ImGui::GetIO();
	const auto& transform = m_render->WorldToScreenMatrix();

	auto x = transform[ 0 ][ 0 ] * origin[ 0 ] + transform[ 0 ][ 1 ] * origin[ 1 ] + transform[ 0 ][ 2 ] * origin[ 2 ] + transform[ 0 ][ 3 ];
	auto y = transform[ 1 ][ 0 ] * origin[ 0 ] + transform[ 1 ][ 1 ] * origin[ 1 ] + transform[ 1 ][ 2 ] * origin[ 2 ] + transform[ 1 ][ 3 ];

	auto w = transform[ 2 ][ 0 ] * origin[ 0 ] + transform[ 2 ][ 1 ] * origin[ 1 ] + transform[ 2 ][ 2 ] * origin[ 2 ] + transform[ 2 ][ 3 ];

	if( w < 0.001f )
	{
		return false;
	}

	auto normal_w = 1.f / w;

	x *= normal_w;
	y *= normal_w;

	const auto center_x = ( io.DisplaySize[ 0 ] * 0.5f );
	const auto center_y = ( io.DisplaySize[ 1 ] * 0.5f );

	screen[ 0 ] = center_x + 0.5f * x * io.DisplaySize[ 0 ] + 0.5f;
	screen[ 1 ] = center_y - 0.5f * y * io.DisplaySize[ 1 ] + 0.5f;

	return true;
}

double GetCurtime()
{
	auto globals = *reinterpret_cast< std::uintptr_t* >( m_image_client + 0x005D2B74 );

	auto key_array = reinterpret_cast< int* >( m_image_client + 0x004BDB18 );

	auto curtime = *reinterpret_cast< std::uint64_t* >( globals + 0x0108 );
	auto curtime_key = key_array[ ( unsigned __int8 )curtime ^ 235 ] ^ ( ( ( int )curtime ^ 0x1DE315EB ) >> 8 );

	curtime ^= 0xDC1E084E;

	return *reinterpret_cast< double* >( &curtime );
}

} // namespace css