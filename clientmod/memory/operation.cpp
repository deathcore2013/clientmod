#include "operation.hpp"
#include "base.hpp"

#include "../win32/trace.hpp"

namespace atom::memory
{

std::uintptr_t ToUIntPointer( const void* pointer )
{
	return reinterpret_cast< std::uintptr_t >( pointer );
}

void* ToPointer( std::uintptr_t pointer )
{
	return reinterpret_cast< void* >( pointer );
}

const void* ToConstantPointer( std::uintptr_t pointer )
{
	return reinterpret_cast< const void* >( pointer );
}

bool IsUserMode( std::uintptr_t pointer )
{
	return ( pointer >= MinimumUserModeAddress &&
					 pointer <= MaximumUserModeAddress );
}

bool IsUserMode( const void* pointer )
{
	return IsUserMode( ToUIntPointer( pointer ) );
}

bool QueryPage( std::uintptr_t pointer, MEMORY_BASIC_INFORMATION* information )
{
	if( VirtualQuery( ToConstantPointer( pointer ), information, sizeof( MEMORY_BASIC_INFORMATION ) ) != sizeof( MEMORY_BASIC_INFORMATION ) )
	{
		TRACE( "%s: VirtualQuery(...) error! (0x%08X)", __FUNCTION__, GetLastError() );
		return false;
	}

	return true;
}

bool QueryPage( const void* pointer, MEMORY_BASIC_INFORMATION* information )
{
	return QueryPage( ToUIntPointer( pointer ), information );
}

bool ProtectPage( std::uintptr_t pointer, std::size_t number_of_bytes, std::uint32_t protect, std::uint32_t* old_protect /*= nullptr*/ )
{
	ULONG protection = 0;
	
	if( VirtualProtect( ToPointer( pointer ), number_of_bytes, protect, &protection ) != TRUE )
	{
		TRACE( "%s: VirtualProtect(...) error! (0x%08X)", __FUNCTION__, GetLastError() );
		return false;
	}

	if( old_protect )
	{
		*old_protect = static_cast< std::uint32_t >( protection );
	}

	return true;
}

bool ProtectPage( const void* pointer, std::size_t number_of_bytes, std::uint32_t protect, std::uint32_t* old_protect /*= nullptr*/ )
{
	return ProtectPage( ToUIntPointer( pointer ), number_of_bytes, protect, old_protect );
}

} // namespace atom::memory