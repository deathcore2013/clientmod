#include "trace.hpp"

namespace atom::win32
{

void TraceFormat( const char* format, va_list arg_pack )
{
	char output[ 2048 ] = { };
	vsprintf_s( output, format, arg_pack );

	OutputDebugStringA( output );
}

void Trace( const char* format, ... )
{
	va_list arg_pack = { };
	va_start( arg_pack, format );
	TraceFormat( format, arg_pack );
	va_end( arg_pack );
}

} // namespace atom::win32