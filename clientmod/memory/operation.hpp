#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

namespace atom::memory
{

std::uintptr_t ToUIntPointer( const void* pointer );
void* ToPointer( std::uintptr_t pointer );
const void* ToConstantPointer( std::uintptr_t pointer );

bool IsUserMode( std::uintptr_t pointer );
bool IsUserMode( const void* pointer );

bool QueryPage( std::uintptr_t pointer, MEMORY_BASIC_INFORMATION* information );
bool QueryPage( const void* pointer, MEMORY_BASIC_INFORMATION* information );

bool ProtectPage( std::uintptr_t pointer, std::size_t number_of_bytes, std::uint32_t protect, std::uint32_t* old_protect = nullptr );
bool ProtectPage( const void* pointer, std::size_t number_of_bytes, std::uint32_t protect, std::uint32_t* old_protect = nullptr );

template< std::size_t Index, typename DataType = std::uintptr_t >
FORCEINLINE DataType GetMethod( void* instance )
{
	const auto pointer = *reinterpret_cast< std::uintptr_t** >( instance );
	return DataType( pointer[ Index ] );
}

template< std::size_t Index, typename DataType, typename... ArgPack >
FORCEINLINE DataType CallMethod( void* instance, ArgPack... arg_pack )
{
	using MethodType = DataType( API_THISCALL* )( void*, ArgPack... );

	const auto method = GetMethod< Index, MethodType >( instance );
	return method( instance, arg_pack... );
}

} // namespace atom::memory