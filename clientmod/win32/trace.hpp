#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

namespace atom::win32
{

void TraceFormat( const char* format, va_list arg_pack );
void Trace( const char* format, ... );

} // namespace atom::win32

#define TRACE( Format, ... )													atom::win32::Trace( "[cm] " Format##"\n", __VA_ARGS__ )