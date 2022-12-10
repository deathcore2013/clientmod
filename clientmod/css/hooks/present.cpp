#include "present.hpp"

#include "../features/aimbot.hpp"
#include "../features/user_interface.hpp"

struct TypeDescriptor;
struct PMD;

struct RTTICompleteObjectLocator;
struct RTTIClassHierarchyDescriptor;
struct RTTIBaseClassDescriptor;

#pragma warning( push )
#pragma warning( disable : 4200 )
struct TypeDescriptor
{
	std::uintptr_t vftable;
	const char* spare;
	char name[ 0 ];
};
#pragma warning( pop )

#include <type_traits>

struct PMD
{
	int mdisp;
	int pdisp;
	int vdisp;
};

struct RTTICompleteObjectLocator
{
	std::uint32_t signature;
	std::uint32_t offset;
	std::uint32_t cd_offset;
	TypeDescriptor* type_descriptor;
	RTTIClassHierarchyDescriptor* class_descriptor;
};

struct RTTIClassHierarchyDescriptor
{
	std::uint32_t signature;
	std::uint32_t attributes;
	std::uint32_t num_base_classes;
	RTTIBaseClassDescriptor** base_class_array;
};

struct RTTIBaseClassDescriptor
{
	TypeDescriptor* type_descriptor;
	std::uint32_t num_base_classes;
	PMD pmd;
	std::uint32_t attributes;
	RTTIClassHierarchyDescriptor* class_hierarchy_descriptor;
};

void* FindClassInstance( std::uintptr_t image_base, const char* class_name )
{
	const auto image_data_section = win32::GetImageSectionHeader( image_base, ".data" );
	const auto image_rdata_section = win32::GetImageSectionHeader( image_base, ".rdata" );

	if( !memory::IsUserMode( image_data_section ) ||
			!memory::IsUserMode( image_rdata_section ) )
	{
		return nullptr;
	}

	const auto is_in_data = [ &image_base, &image_data_section ]( void* pointer )
	{
		return ( memory::ToUIntPointer( pointer ) >= ( image_base + image_data_section->VirtualAddress ) &&
						 memory::ToUIntPointer( pointer ) < ( image_base + image_data_section->VirtualAddress + image_data_section->Misc.VirtualSize ) );
	};

	const auto is_in_rdata = [ &image_base, &image_rdata_section ]( void* pointer )
	{
		return ( memory::ToUIntPointer( pointer ) >= ( image_base + image_rdata_section->VirtualAddress ) &&
						 memory::ToUIntPointer( pointer ) < ( image_base + image_rdata_section->VirtualAddress + image_rdata_section->Misc.VirtualSize ) );
	};

	for( auto pointer = ( image_base + image_data_section->VirtualAddress );
			 pointer < ( image_base + image_data_section->VirtualAddress + image_data_section->Misc.VirtualSize );
			 pointer += sizeof( std::uintptr_t ) )
	{
		const auto vmt = *reinterpret_cast< std::uintptr_t** >( pointer );

		if( is_in_rdata( vmt ) )
		{
			const auto complete_object_locator = reinterpret_cast< RTTICompleteObjectLocator* >( vmt[ -1 ] );

			if( is_in_rdata( complete_object_locator ) )
			{
				const auto type_descriptor = complete_object_locator->type_descriptor;

				if( is_in_data( type_descriptor ) )
				{
					if( std::strcmp( type_descriptor->name, class_name ) == 0 )
					{
						TRACE( "type \"%s\" at 0x%08X", type_descriptor->name, pointer );
						return memory::ToPointer( pointer );
					}
				}
			}
		}
	}

	return nullptr;
}

void DumpRunTimeTypeInformation( std::uintptr_t image_base )
{
	const auto image_data_section = win32::GetImageSectionHeader( image_base, ".data" );
	const auto image_rdata_section = win32::GetImageSectionHeader( image_base, ".rdata" );

	if( !memory::IsUserMode( image_data_section ) ||
			!memory::IsUserMode( image_rdata_section ) )
	{
		return;
	}

	const auto is_in_data = [ &image_base, &image_data_section ]( void* pointer )
	{
		return ( memory::ToUIntPointer( pointer ) >= ( image_base + image_data_section->VirtualAddress ) &&
						 memory::ToUIntPointer( pointer ) < ( image_base + image_data_section->VirtualAddress + image_data_section->Misc.VirtualSize ) );
	};

	const auto is_in_rdata = [ &image_base, &image_rdata_section ]( void* pointer )
	{
		return ( memory::ToUIntPointer( pointer ) >= ( image_base + image_rdata_section->VirtualAddress ) &&
						 memory::ToUIntPointer( pointer ) < ( image_base + image_rdata_section->VirtualAddress + image_rdata_section->Misc.VirtualSize ) );
	};

	for( auto pointer = ( image_base + image_data_section->VirtualAddress );
			 pointer < ( image_base + image_data_section->VirtualAddress + image_data_section->Misc.VirtualSize );
			 pointer += sizeof( std::uintptr_t ) )
	{
		const auto vmt = *reinterpret_cast< std::uintptr_t** >( pointer );

		if( is_in_rdata( vmt ) )
		{
			const auto complete_object_locator = reinterpret_cast< RTTICompleteObjectLocator* >( vmt[ -1 ] );

			if( is_in_rdata( complete_object_locator ) )
			{
				const auto type_descriptor = complete_object_locator->type_descriptor;
				
				if( is_in_data( type_descriptor ) )
				{
					TRACE( "type \"%s\" at 0x%08X", type_descriptor->name, pointer );
				}
			}
		}
	}
}

namespace css::hooks
{

bool ComputeBoundingBox( C_BaseEntity* base_entity, Vector4D& output )
{
	const auto& io = ImGui::GetIO();
	const auto& transform = base_entity->m_rgflCoordinateFrame();

	const auto& origin = base_entity->GetAbsOrigin();

	const auto& mins = base_entity->OBBMins();
	const auto maxs = base_entity->OBBMaxs() + Vector( 0.f, 0.f, 10.f );

	const Vector bounds[ 8 ] =
	{
		{ mins[ 0 ], mins[ 1 ], mins[ 2 ] },
		{ mins[ 0 ], maxs[ 1 ], mins[ 2 ] },
		{ maxs[ 0 ], maxs[ 1 ], mins[ 2 ] },
		{ maxs[ 0 ], mins[ 1 ], mins[ 2 ] },
		{ maxs[ 0 ], maxs[ 1 ], maxs[ 2 ] },
		{ mins[ 0 ], maxs[ 1 ], maxs[ 2 ] },
		{ mins[ 0 ], mins[ 1 ], maxs[ 2 ] },
		{ maxs[ 0 ], mins[ 1 ], maxs[ 2 ] },
	};

	output[ 0 ] = io.DisplaySize[ 0 ];
	output[ 1 ] = io.DisplaySize[ 1 ];
	output[ 2 ] = -1.f;
	output[ 3 ] = -1.f;

	for( std::size_t index = 0; index < RTL_NUMBER_OF( bounds ); index++ )
	{
		Vector corner = { };
		VectorTransform( bounds[ index ], transform, corner );

		ImVec2 edge = { };
		if( !WorldToScreen( corner, edge ) )
		{
			return false;
		}

		output[ 0 ] = std::min( output[ 0 ], edge[ 0 ] );
		output[ 1 ] = std::min( output[ 1 ], edge[ 1 ] );

		output[ 2 ] = std::max( output[ 2 ], edge[ 0 ] );
		output[ 3 ] = std::max( output[ 3 ], edge[ 1 ] );
	}

	return true;
}

void RenderWorldSpaceLine( const Vector& begin, const Vector& end, const Color& color )
{
	auto& renderer = graphics::Renderer::Instance();

	ImVec2 screen[ 2 ] = { };

	if( !WorldToScreen( begin, screen[ 0 ] ) )
	{
		return;
	}

	if( !WorldToScreen( end, screen[ 1 ] ) )
	{
		return;
	}

	renderer.DrawLine( screen[ 0 ], screen[ 1 ], color );
}

void RenderSkeleton( C_CSPlayer* player, const Color& color )
{
	const auto model = player->GetModelPtr();

	if( memory::IsUserMode( model ) )
	{
		const auto studio = model->m_pStudioHdr;

		if( memory::IsUserMode( studio ) )
		{
			matrix3x4_t transform[ MAXSTUDIOBONES ] = { };

			if( player->SetupBones( transform, MAXSTUDIOBONES, 0x00000100, GetCurtime() ) )
			{
				for( int index = 0; index < studio->numbones; index++ )
				{
					const auto bone = studio->pBone( index );

					if( memory::IsUserMode( bone ) )
					{
						if( bone->parent != -1 && bone->flags & 0x00000100 )
						{
							Vector begin = { };
							MatrixGetColumn( transform[ index ], 3, begin );
							
							Vector end = { };
							MatrixGetColumn( transform[ bone->parent ], 3, end );

							RenderWorldSpaceLine( begin, end, color );
						}
					}
				}
			}
		}
	}
}

void RenderPlayer( C_CSPlayer* player )
{
	const auto& visuals = configuration::m_visuals;
	auto& renderer = graphics::Renderer::Instance();

	if( player->IsDead() )
	{
		return;
	}

	if( visuals.m_skeleton )
	{
		RenderSkeleton( player, Color::White );
	}

	Vector4D output = { };
	if( ComputeBoundingBox( player, output ) )
	{
		const auto width = ( output[ 2 ] - output[ 0 ] );
		const auto height = ( output[ 3 ] - output[ 1 ] );

		auto color = Color::White;

		if( player->m_iTeamNum() == 2 )
		{
			color = Color::Red;
		}
		else if( player->m_iTeamNum() == 3 )
		{
			color = Color::Cyan;
		}

		if( visuals.m_box_mode == 1 )
		{
			if( visuals.m_box_border )
			{
				renderer.DrawBoxBorder( { output[ 0 ], output[ 1 ] }, { width, height }, 1.f, color );
			}
			else
			{
				renderer.DrawBox( { output[ 0 ], output[ 1 ] }, { width, height }, 1.f, color );
			}
		}
		else if( visuals.m_box_mode == 2 )
		{
			if( visuals.m_box_border )
			{
				renderer.DrawCornerBoxBorder( { output[ 0 ], output[ 1 ] }, { width, height }, 1.f, color );
			}
			else
			{
				renderer.DrawCornerBox( { output[ 0 ], output[ 1 ] }, { width, height }, 1.f, color );
			}
		}

		Vector2D bottom = { };

		if( visuals.m_health )
		{
			const auto health = static_cast< float >( player->m_iHealth() );
			const auto health_max = 100.f;
			const auto health_percentage = std::clamp( ( health / health_max ), 0.f, 1.f );

			const auto bar_width = ( width + 1.f );
			const auto bar_health = ( ( bar_width * health ) / health_max );

			const Color bar_color =
			{
				1.f - health_percentage,
				health_percentage,
				0.f,
				1.f,
			};

			renderer.DrawRect( { output[ 0 ] - 1.f, output[ 1 ] + height + 3.f + bottom[ 1 ] }, { bar_width + 2.f, 3.f }, Color::Black );
			renderer.DrawRect( { output[ 0 ], output[ 1 ] + height + 4.f + bottom[ 1 ] }, { bar_health, 1.f }, bar_color ); bottom[ 1 ] += 8.f;
		}

		if( visuals.m_weapon )
		{
			// 
			// #TODO: implement this
			// 
		}

		if( visuals.m_distance )
		{
			const auto local_player = C_CSPlayer::GetLocalCSPlayer();

			if( memory::IsUserMode( local_player ) )
			{
				const auto& origin = local_player->GetAbsOrigin();
				const auto distance = origin.DistTo( player->GetAbsOrigin() );

				renderer.DrawTextShadow( { output[ 0 ] + width * 0.5f, output[ 1 ] + height + 3.f + bottom[ 1 ] }, TextLayout::CenterH, Color::White, "[%.2fm]", distance ); bottom[ 1 ] += 12.f;
			}
		}
	}
}

void RenderBaseEntity( C_BaseEntity* base_entity )
{
	if( !memory::IsUserMode( base_entity ) )
	{
		return;
	}

	if( base_entity->IsLocal() )
	{
		return;
	}

	if( base_entity->IsDormant() )
	{
		return;
	}

	if( base_entity->IsPlayer() )
	{
		const auto player = static_cast< C_CSPlayer* >( base_entity );
		RenderPlayer( player );
	}
}

void TriggerBot( C_CSPlayer* player )
{
	if( player->IsDead() )
	{
		return;
	}

	if( player->IsDormant() )
	{
		return;
	}

	const auto weapon_handle = player->m_hActiveWeapon();

	if( !weapon_handle.IsValid() )
	{
		return;
	}

	const auto weapon = m_client_entity_list->GetClientEntityFromHandle( weapon_handle );

	if( !memory::IsUserMode( weapon ) )
	{
		return;
	}

	const auto origin = player->m_vecNetworkOrigin();
	const auto view_offset = player->m_vecViewOffset();
	const auto angles = player->m_angRotation();

	const auto begin = ( origin + view_offset );
	
	Vector forward = { };
	AngleVectors( angles, &forward );
	const auto end = ( begin + ( forward * 8192.f ) );

	/*
	Ray_t ray = { };
	ray.Init( begin, end );

	// CTraceFilterSkipTwoEntities ignore( ( const IHandleEntity* )player, ( const IHandleEntity* )nullptr, 0 );
	uint ignore[ 4 ] = { };
	ignore[ 0 ] = ( uint )( m_image_client + 0x005D3F64 );
	ignore[ 1 ] = ( uint )player;
	ignore[ 2 ] = 0;
	ignore[ 3 ] = 0;

	trace_t trace = { };
	m_engine_trace_client->TraceRay( ray, 0x4600400B, ( ITraceFilter* )ignore, &trace );
	*/
	using Fn = void( __fastcall* )( const Vector& start, const Vector& end, unsigned int fMask, C_BaseEntity* pIgnoreEntity, int contentsMask, trace_t* pTrace );
	const auto TraceLine = Fn( m_image_client + 0x000796B8 );
	
	trace_t trace = { };
	TraceLine( begin, end, 0x4600400B, player, 0, &trace );

	if( !memory::IsUserMode( trace.m_pEnt ) )
	{
		// TRACE( "[0x%08X] not valid", uint( trace.m_pEnt ) );
		return;
	}

	const auto target = ToCSPlayer( trace.m_pEnt );
	// TRACE( "m_pEnt = 0x%08X", uint( trace.m_pEnt ) );

	if( !memory::IsUserMode( target ) )
	{
		// TRACE( "[0x%08X] not a player", uint( trace.m_pEnt ) );
		return;
	}

	if( target->IsDead() || target->IsDormant() || !target->IsEnemy( player ) )
	{
		// TRACE( "[0x%08X] dead/dormant/not enemy", uint( trace.m_pEnt ) );
		return;
	}

	if( !GetAsyncKeyState( VK_SPACE ) )
	{
		return;
	}

	INPUT input = { };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput( 1, &input, sizeof( input ) );

	std::memset( &input, 0, sizeof( input ) );
	
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput( 1, &input, sizeof( input ) );
}

void RenderWorld()
{
	auto& renderer = graphics::Renderer::Instance();

	Vector2D output( 10.f, 10.f ); output[ 1 ] += 12.f;
	
	const auto local_player = C_CSPlayer::GetLocalCSPlayer();
	// renderer.DrawTextShadow( { output[ 0 ], output[ 1 ] }, TextLayout::Left, Color::White, "local_player = 0x%08X", memory::ToUIntPointer( local_player ) ); output[ 1 ] += 12.f;

	if( !memory::IsUserMode( local_player ) )
	{
		return;
	}

	TriggerBot( local_player );

	// const auto& va = *reinterpret_cast< QAngle* >( local_player + 0x09B0 );
	// renderer.DrawTextShadow( { output[ 0 ], output[ 1 ] }, TextLayout::Left, Color::White, "va = (%f, %f)", va[ 0 ], va[ 1 ] ); output[ 1 ] += 12.f;
	/*
	QAngle va = { };
	m_engine_client->GetViewAngles( va );

	const auto eye_position = local_player->EyePosition();
	// renderer.DrawTextShadow( { output[ 0 ], output[ 1 ] }, TextLayout::Left, Color::White, "eye_position = (%f, %f, %f)", eye_position[ 0 ], eye_position[ 1 ], eye_position[ 2 ] ); output[ 1 ] += 12.f;
	
	const auto highest_entity_index = m_client_entity_list->GetHighestEntityIndex();
	// renderer.DrawTextShadow( { output[ 0 ], output[ 1 ] }, TextLayout::Left, Color::White, "highest_entity_index = %d", highest_entity_index ); output[ 1 ] += 12.f;

	for( int index = 0; index <= highest_entity_index; index++ )
	{
		const auto base_entity = C_BaseEntity::GetBaseEntity( index );
		RenderBaseEntity( base_entity );
	}
	*/
}

HRESULT API_STDCALL Present( IDirect3DDevice9* device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region )
{
	SEH2_TRY()
	{
		auto& renderer = graphics::Renderer::Instance();
		auto& user_interface = features::UserInterface::Instance();
		auto& aimbot = features::Aimbot::Instance();
		
		if( renderer.Begin() )
		{
			RenderWorld();

			// aimbot.Present();
			// user_interface.Present();

			// renderer.DrawTextShadow( { 10.f, 10.f }, TextLayout::Left, Color::White, "[enigma] 0.6 (alpha)" );
			renderer.End();
		}
	}
	SEH2_EXCEPT( EXCEPTION_EXECUTE_HANDLER )
	{ }

	return m_present( device, source_rect, dest_rect, dest_window_override, dirty_region );
}

} // namespace css::hooks