#include "search.hpp"
#include "operation.hpp"

#include "../win32/image.hpp"
#include "../win32/trace.hpp"

#if !defined( IN_RANGE )
#define IN_RANGE( x, a, b )														( x >= a && x <= b )
#endif // !IN_RANGE

#if !defined( GET_BITS )
#define GET_BITS( x )																	( IN_RANGE( x, '0', '9' ) ? ( x - '0' ) : ( ( x & ( ~0x20 ) ) - 'A' + 0x0A ) )
#endif // !GET_BITS

#if !defined( GET_BYTE )
#define GET_BYTE( x )																	( GET_BITS( x[ 0 ] ) << 4 | GET_BITS( x[ 1 ] ) )
#endif // !GET_BYTE

namespace atom::memory
{

std::size_t GetNumberOfBytes( const char* signature )
{
	auto number_of_bytes = static_cast< std::size_t >( 0 );
	const auto length = std::strlen( signature );

	for( std::size_t number = 0; number < length; number++ )
	{
		if( signature[ number ] == '?' )
		{
			number_of_bytes++;
		}
		else if( signature[ number ] != ' ' )
		{
			number++;
			number_of_bytes++;
		}
	}

	return number_of_bytes;
}

namespace implementation
{

std::uintptr_t Search( std::uintptr_t begin, std::uintptr_t end, const char* signature )
{
	const auto number_of_bytes = GetNumberOfBytes( signature );

	MEMORY_BASIC_INFORMATION information = { };

	for( auto pointer = begin;
			 pointer < end;
			 pointer = ( ToUIntPointer( information.BaseAddress ) + information.RegionSize ) )
	{
		if( QueryPage( pointer, &information ) )
		{
			if( information.State != MEM_FREE &&
					information.State != MEM_RESERVE &&
					information.Protect != PAGE_NOACCESS )
			{
				auto bytes = reinterpret_cast< const std::uint8_t* >( signature );
				auto occurrence = static_cast< std::uint8_t* >( nullptr );

				auto page_begin = reinterpret_cast< std::uint8_t* >( information.BaseAddress );
				auto page_end = ( page_begin + information.RegionSize - number_of_bytes );

				for( auto page_pointer = page_begin; page_pointer < page_end; page_pointer++ )
				{
					if( bytes[ 0 ] == '?' || page_pointer[ 0 ] == GET_BYTE( bytes ) )
					{
						if( !occurrence )
						{
							occurrence = page_pointer;
						}

						const auto word = *reinterpret_cast< const std::uint16_t* >( bytes );

						if( word == '??' || bytes[ 0 ] != '?' )
						{
							bytes += sizeof( std::uint16_t );
						}
						else
						{
							bytes += sizeof( std::uint8_t );
						}

						if( !bytes[ 0 ] )
						{
							return ToUIntPointer( occurrence );
						}

						bytes++;

						if( !bytes[ 0 ] )
						{
							return ToUIntPointer( occurrence );
						}
					}
					else if( occurrence )
					{
						page_pointer = occurrence;
						bytes = reinterpret_cast< const std::uint8_t* >( signature );
						occurrence = nullptr;
					}
				}
			}
		}
		else
		{
			break;
		}
	}

	return 0;
}

} // namespace implementation

std::size_t Search( std::uintptr_t begin, std::uintptr_t end, const char* signature, std::vector< std::uintptr_t >& output )
{
	output.clear();

	const auto number_of_bytes = GetNumberOfBytes( signature );

	for( ; begin < end; )
	{
		const auto occurrence = implementation::Search( begin, end, signature );

		if( IsUserMode( occurrence ) )
		{
			begin = ( occurrence + number_of_bytes );
			output.emplace_back( occurrence );
		}
		else
		{
			break;
		}
	}

	return output.size();
}

std::size_t SearchSection( std::uintptr_t image_base, const char* name, const char* signature, std::vector< std::uintptr_t >& output )
{
	const auto image_nt_headers = win32::GetImageNtHeaders( image_base );

	if( image_nt_headers )
	{
		const auto image_section_header = IMAGE_FIRST_SECTION( image_nt_headers );

		for( std::uint16_t index = 0; index = image_nt_headers->FileHeader.NumberOfSections; index++ )
		{
			const auto image_section = &image_section_header[ index ];

			char section_name[ 16 ] = { };
			std::memcpy( section_name, image_section->Name, sizeof( image_section->Name ) );

			if( _strnicmp( section_name, name, sizeof( section_name ) ) == 0 )
			{
				const auto begin = ( image_base + image_section->VirtualAddress );
				const auto end = ( begin + image_section->Misc.VirtualSize );

				const auto count = Search( begin, end, signature, output );

				if( count )
				{
					return count;
				}
			}
		}
	}

	return 0;
}

} // namespace atom::memory