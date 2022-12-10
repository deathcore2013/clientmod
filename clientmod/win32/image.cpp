#include "image.hpp"
#include "trace.hpp"

#include "../memory/base.hpp"
#include "../memory/operation.hpp"

namespace atom::win32
{

std::uintptr_t GetImageBase( const wchar_t* name )
{
	const auto image_base = GetModuleHandleW( name );

	if( !memory::IsUserMode( image_base ) )
	{
		TRACE( "%s: GetModuleHandleW(...) error! (0x%08X)", __FUNCTION__, GetLastError() );
		return 0;
	}

	return memory::ToUIntPointer( image_base );
}

IMAGE_DOS_HEADER* GetImageDosHeader( std::uintptr_t image_base )
{
	if( !memory::IsUserMode( image_base ) )
	{
		TRACE( "%s: image_base is not valid!", __FUNCTION__ );
		return nullptr;
	}
	
	const auto image_dos_header = reinterpret_cast< IMAGE_DOS_HEADER* >( image_base );

	if( image_dos_header->e_magic != IMAGE_DOS_SIGNATURE )
	{
		TRACE( "%s: image_dos_header is not valid!", __FUNCTION__ );
		return nullptr;
	}

	return image_dos_header;
}

IMAGE_NT_HEADERS* GetImageNtHeaders( std::uintptr_t image_base )
{
	const auto image_dos_header = GetImageDosHeader( image_base );

	if( !image_dos_header )
	{
		TRACE( "%s: image_dos_header is not valid!", __FUNCTION__ );
		return nullptr;
	}

	const auto image_nt_headers = reinterpret_cast< IMAGE_NT_HEADERS* >( image_base + image_dos_header->e_lfanew );

	if( image_nt_headers->Signature != IMAGE_NT_SIGNATURE )
	{
		TRACE( "%s: image_nt_headers is not valid!", __FUNCTION__ );
		return nullptr;
	}

	return image_nt_headers;
}

IMAGE_SECTION_HEADER* GetImageSectionHeader( std::uintptr_t image_base, const char* name )
{
	const auto image_nt_headers = GetImageNtHeaders( image_base );

	if( !memory::IsUserMode( image_nt_headers ) )
	{
		TRACE( "%s: image_nt_headers is not valid!", __FUNCTION__ );
		return nullptr;
	}

	const auto image_section_header = IMAGE_FIRST_SECTION( image_nt_headers );

	for( std::uint16_t index = 0; index < image_nt_headers->FileHeader.NumberOfSections; index++ )
	{
		const auto image_section = &image_section_header[ index ];

		char image_section_name[ 16 ] = { };
		std::memcpy( image_section_name, image_section->Name, sizeof( image_section->Name ) );

		if( _stricmp( image_section_name, name ) == 0 )
		{
			return image_section;
		}
	}

	return nullptr;
}

std::uintptr_t GetImageExport( std::uintptr_t image_base, const char* name )
{
	if( !memory::IsUserMode( image_base ) )
	{
		TRACE( "%s: image_base is not valid!", __FUNCTION__ );
		return 0;
	}

	const auto image_export = GetProcAddress( reinterpret_cast< HMODULE >( image_base ), name );

	if( !memory::IsUserMode( image_export ) )
	{
		TRACE( "%s: GetProcAddress(...) error! (0x%08X)", __FUNCTION__, GetLastError() );
		return 0;
	}

	return memory::ToUIntPointer( image_export );
}

} // namespace atom::win32